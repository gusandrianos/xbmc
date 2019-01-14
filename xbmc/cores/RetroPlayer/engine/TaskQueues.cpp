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

#include "TaskQueues.h"
#include "Core.h" //! @todo: Remove me
#include "Tasks.h"

#include "threads/SingleLock.h"

using namespace KODI;
using namespace RETRO;

DeferredRun::DeferredRun(RunnablePtr runnable, RequirementVector requirements) :
  m_runnable(std::move(runnable)),
  m_requirements(std::move(requirements))
{
}

RunnablePtr &&DeferredRun::ReleaseRunnable()
{
  return std::move(m_runnable);
}

RequirementVector &&DeferredRun::ReleaseRequirements()
{
  return std::move(m_requirements);
}

TaskData::TaskData(TaskPtr task, TaskCallbackPtr callback) :
  task(std::move(task)),
  callback(std::move(callback))
{
}

TaskQueue::TaskQueue(ThreadPool &threadPool) :
  m_threadPool(threadPool)
{
}

void TaskQueue::AddTask(TaskPtr task, TaskCallbackPtr callback)
{
  CSingleLock lock(m_mutex);

  TaskDataPtr taskData(new TaskData(task, std::move(callback)));
  m_taskDataMap[task] = std::move(taskData);

  std::unique_ptr<DeferredRun> rr = task->PreFlight();

  AddRunnable(std::move(task), rr->ReleaseRunnable(), rr->ReleaseRequirements());
}

void TaskQueue::AddRunnable(TaskPtr owner, RunnablePtr runnable, RequirementVector requirements)
{
  CSingleLock lock(m_mutex);

  m_runnableOwnerMap[runnable] = owner;

  auto it = m_taskDataMap.find(owner);
  if (it != m_taskDataMap.end())
  {
    TaskData &taskData = *it->second;

    taskData.runnable = std::move(runnable);
    taskData.requirementsMap.clear();
    taskData.requirements.clear();

    for (RequirementPtr &requirement : requirements)
    {
      taskData.requirementsMap[requirement.get()] = taskData.requirements.size();
      taskData.requirements.emplace_back(new NotFilled);
      m_requirementsOwnerMap[requirement.get()] = owner;

      RequirementCallback callback = [this](RequirementPtr requirement, TaskResultPtr result)
        {
          OnRequirementDone(std::move(requirement), std::move(result));
        };

      AddTask(std::move(requirement), callback);
    }
  }
}

void TaskQueue::OnRunnableDone(RunnablePtr runnable, TaskResultPtr result)
{
  CSingleLock lock(m_mutex);

  auto it = m_runnableOwnerMap.find(runnable);
  if (it != m_runnableOwnerMap.end())
  {
    std::shared_ptr<Task> &owner = it->second;

    auto it2 = m_taskDataMap.find(owner);
    if (it2 != m_taskDataMap.end())
    {
      std::unique_ptr<TaskData> &taskData = it2->second;

      const DeferredRun* deferredRun = dynamic_cast<const DeferredRun*>(result.get());

      if (deferredRun != nullptr)
        AddRunnable(owner, std::move(deferredRun->Runnable()), std::move(deferredRun->requirements));
      else
        OnTaskDone(taskData->task, std::move(result));
    }
  }
}

void TaskQueue::OnTaskDone(TaskPtr task, TaskResultPtr result)
{
  CSingleLock lock(m_mutex);

  auto it = m_taskDataMap.find(task);
  if (it != m_taskDataMap.end())
  {
    std::unique_ptr<TaskData> &taskData = it->second;

    taskData->callback->Callback(std::move(task), std::move(result));

    m_taskDataMap.erase(it);
  }
}

void TaskQueue::OnRequirementDone(RequirementPtr requirement, int error, TaskResultPtr result)
{
  CSingleLock lock(m_mutex);

  auto it = m_requirementsOwnerMap.find(requirement.get());
  if (it != m_requirementsOwnerMap.end())
  {
    std::shared_ptr<Task> &owner = it->second;

    auto it2 = m_taskDataMap.find(owner);
    if (it2 != m_taskDataMap.end())
    {
      TaskDataPtr &taskData = it2->second;

      if (error != 0)
      {
        taskData->callback->OnDone(std::move(taskData), error, Object());
      }

      auto it3 = taskData->requirementsMap.find(requirement.get());
      if (it3 != taskData->requirementsMap.end())
      {
        size_t i = it3->second;

        RequirementVector &requirements = taskData->requirements;

        RequirementPtr &requirement = requirements[i];

        requirement = std::move(result);

        bool x = false;
        if (x)
        {
          RunnableCallback callback = [this](Runnable runnable, std::unique_ptr<TaskResult> result)
            {
              OnRunnableDone(runnable, std::move(result));
            };

          std::unique_ptr<Args> args; //! @todo Should be requirements
          m_threadPool->Append(taskData->runnable, callback, requirements.size(), std::move(args));
        }
      }
    }
  }
}
