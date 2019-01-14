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
#pragma once

#include "Types.h"
#include "threads/Event.h"
#include "threads/CriticalSection.h"
#include "threads/SingleLock.h" //! @todo Move to .cpp
#include "threads/Thread.h"

#include <algorithm> //! @todo Move to .cpp
#include <deque>
#include <functional>
#include <memory>
#include <utility> //! @todo Move to .cpp

namespace KODI
{
namespace RETRO
{
  class WorkItem
  {
  public:
    WorkItem(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args);

    RunnablePtr runnable;
    RunnableCallbackPtr callback;
    unsigned int priority;
    Object args;
  };

  void SafeExecute(std::unique_ptr<WorkItem> workItem);

  class ThreadedWorker;

  /*
  struct IThreadPool
  {
    virtual void Append(RunnablePtr runnable, RunnableCallback callback, unsigned int priority, Object args) = 0;
  };
  */

  class ThreadPool
  {
  public:
    virtual ~ThreadPool() = default;

    virtual void Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args) = 0;

    virtual std::unique_ptr<WorkItem> GetNextWorkItem() { return std::unique_ptr<WorkItem>(); }

    virtual void OnDone(ThreadedWorker &worker) { }

    virtual void Quit() = 0;

    virtual void Join() = 0;
  };

  class ThreadedWorker : protected CThread
  {
  public:
    ThreadedWorker(ThreadPool &owner);

  protected:
    // Implementation of CThread
    void Process() override;

  private:
    ThreadPool &m_owner;
  };

  class MainloopThreadPool : public ThreadPool
  {
  public:
    ~MainloopThreadPool() override = default;

    void Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args) override;
    void Quit() override;
    void Join() override;

  private:
    using ItemQueue = std::deque<std::unique_ptr<WorkItem>>;

    ItemQueue m_queue;
    bool m_run = true;
    CCriticalSection m_mutex;
    CEvent m_event;
  };

  class SingleThreadedThreadPool : public ThreadPool
  {
  public:
    ~SingleThreadedThreadPool() override = default;

    void Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args) override;
    std::unique_ptr<WorkItem> GetNextWorkItem() override;
    void OnDone(ThreadedWorker &worker) override;
    void Quit() override;
    void Join() override;

  private:
    using ItemQueue = std::deque<std::unique_ptr<WorkItem>>;

    ItemQueue m_queue;
    bool m_run = true;
    std::unique_ptr<ThreadedWorker> m_worker;
    CCriticalSection m_mutex;
    CEvent m_event;
  };

  class OptimisticThreadPool : public ThreadPool
  {
  public:
    OptimisticThreadPool(unsigned int workerCount);

    void Append(RunnablePtr runnable, RunnableCallbackPtr callback, unsigned int priority, Object args) override;
    std::unique_ptr<WorkItem> GetNextWorkItem() override;
    void OnDone(ThreadedWorker &worker) override;
    void Quit() override;
    void Join() override;

  private:
    using ItemQueue = std::deque<std::unique_ptr<WorkItem>>;
    using WorkerQueue = std::deque<std::unique_ptr<ThreadedWorker>>;

    ItemQueue m_queue;
    bool m_acceptNewTasks = true;
    const unsigned int m_workerCount;
    WorkerQueue m_workers;
    CCriticalSection m_mutex;
    CEvent m_event;
  };
}
}
