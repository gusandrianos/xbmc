/*
 *      Copyright (C) 2018 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "ThreadPools.h"
#include "Core.h"
#include "Demands.h"
#include "Predicates.h"
#include "Supplies.h"
#include "Tasks.h"


using namespace KODI;
using namespace RETRO;

WorkItem::WorkItem(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args) :
  runnable(std::move(runnable)),
  callback(std::move(callback)),
  priority(priority),
  args(std::move(args))
{
}

void SafeExecute(std::unique_ptr<WorkItem> workItem)
{
  //! @todo Debug log: Running <runnable> in thread <thread ID> with args <args>

  bool result = workItem->runnable->Run(std::move(workItem->args));

  workItem->callback->OnFinish(std::move(workItem->runnable), result);
}

ThreadedWorker::ThreadedWorker(ThreadPool &owner) :
  CThread("ThreadedWorker"),
  m_owner(owner)
{
  Create(false);
}

void ThreadedWorker::Process()
{
  std::unique_ptr<WorkItem> workItem = m_owner.GetNextWorkItem();

  while (workItem)
  {
    SafeExecute(std::move(workItem));
    workItem = m_owner.GetNextWorkItem();
  }

  m_owner.OnDone(*this);
}

void MainloopThreadPool::Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args)
{
  //! @todo Debug log: Append <runnable> with args <args>

  CSingleLock lock(m_mutex);

  m_queue.emplace_back(new WorkItem(std::move(runnable), std::move(callback), priority, std::move(args)));
  m_event.Set();
}

void MainloopThreadPool::Quit()
{
  CSingleLock lock(m_mutex);

  m_run = false;
  m_event.Set();
}

void MainloopThreadPool::Join()
{
  CSingleLock lock(m_mutex);

  while (m_run)
  {
    if (!m_queue.empty())
    {
      ItemQueue::iterator workItem = m_queue.end();

      for (ItemQueue::iterator pi = m_queue.begin(); pi != m_queue.end(); ++pi)
      {
        if (workItem == m_queue.end() || (*pi)->priority > (*workItem)->priority)
          workItem = pi;
      }

      if (workItem != m_queue.end())
      {
        SafeExecute(std::move(*workItem));
        m_queue.erase(workItem);
      }
    }
    else
    {
      m_event.Wait();
    }
  }
}

void SingleThreadedThreadPool::Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args)
{
  CSingleLock lock(m_mutex);

  m_queue.emplace_back(new WorkItem(std::move(runnable), std::move(callback), priority, std::move(args)));

  if (!m_worker)
    m_worker.reset(new ThreadedWorker(*this));

  m_event.Set();
}

std::unique_ptr<WorkItem> SingleThreadedThreadPool::GetNextWorkItem()
{
  std::unique_ptr<WorkItem> workItem;

  CSingleLock lock(m_mutex);

  ItemQueue::iterator wi = m_queue.end();

  for (ItemQueue::iterator pi = m_queue.begin(); pi != m_queue.end(); ++pi)
  {
    if (wi == m_queue.end() || (*pi)->priority > (*wi)->priority)
      wi = pi;
  }

  if (wi != m_queue.end())
  {
    workItem = std::move(*wi);
    m_queue.erase(wi);
  }

  m_event.Set();

  return workItem;
}

void SingleThreadedThreadPool::OnDone(ThreadedWorker &worker)
{
  //! @todo Debug log: Removing worker from threadpool

  CSingleLock lock(m_mutex);

  m_worker.reset();
  m_event.Set();
}

void SingleThreadedThreadPool::Quit()
{
  //! @todo Debug log: Quiting threadpool

  CSingleLock lock(m_mutex);

  m_run = false;
  m_event.Set();
}

void SingleThreadedThreadPool::Join()
{
  CSingleLock lock(m_mutex);

  while (m_run && m_worker && !m_queue.empty())
    m_event.Wait();
}

OptimisticThreadPool::OptimisticThreadPool(unsigned int workerCount) :
  m_workerCount(workerCount)
{
}

void OptimisticThreadPool::Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args)
{
  CSingleLock lock(m_mutex);

  if (m_acceptNewTasks)
  {
    m_queue.emplace_back(new WorkItem(std::move(runnable), std::move(callback), priority, std::move(args)));

    if (m_workers.size() < m_workerCount)
      m_workers.emplace_back(new ThreadedWorker(*this));

    m_event.Set();
  }
}

std::unique_ptr<WorkItem> OptimisticThreadPool::GetNextWorkItem()
{
  std::unique_ptr<WorkItem> workItem;

  CSingleLock lock(m_mutex);

  ItemQueue::iterator wi = m_queue.end();

  for (ItemQueue::iterator pi = m_queue.begin(); pi != m_queue.end(); ++pi)
  {
    if (wi == m_queue.end() || (*pi)->priority > (*wi)->priority)
      wi = pi;
  }

  if (wi != m_queue.end())
  {
    workItem = std::move(*wi);
    m_queue.erase(wi);
  }

  m_event.Set();

  return workItem;
}

void OptimisticThreadPool::OnDone(ThreadedWorker &worker)
{
  CSingleLock lock(m_mutex);

  /* @todo
   auto WorkerMatches = [this](ThreadedWorker &worker)
   {
   return this == &worker;
   };

   m_workers.erase(std::remove_if(m_workers.begin(), m_workers.end(), WorkerMatches), m_workers.end());
   */

  m_event.Set();
}

void OptimisticThreadPool::Quit()
{
  //! @todo Debug log: Quiting threadpool

  CSingleLock lock(m_mutex);

  m_queue.clear();
  m_acceptNewTasks = false;
  m_event.Set();
}

void OptimisticThreadPool::Join()
{
  CSingleLock lock(m_mutex);

  while (!m_workers.empty() && !m_queue.empty())
    m_event.Wait();
}

