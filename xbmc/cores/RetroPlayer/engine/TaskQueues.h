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
#include "threads/CriticalSection.h"
#include "threads/Event.h"

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <utility>

namespace KODI
{
namespace RETRO
{
  class TaskResult;
  class ThreadPool;
  class DeferredRun;

  //! @todo
  class Requirement { };

  /*!
   * \brief A placeholder for a tasks requirement which is yet to be filled
   *
   * Internal use only.
   */
  class NotFilled : public Requirement
  {
  };

  /*!
   * \brief Structure for necessary data to manage a task
   *
   * Internal use only.
   */
  class TaskData
  {
  public:
    TaskData(TaskPtr task, TaskCallbackPtr callback);

    const TaskPtr task;
    const TaskCallbackPtr callback;
    RequirementMap requirementsMap;
    RequirementVector requirements;
    RunnablePtr runnable;
  };

  /*!
   * \brief A queue which will manage tasks, and schedule them out on the
   *        thread pool
   *
   * The queue will also manage any requirements of given tasks.
   */
  class TaskQueue
  {
  public:
    TaskQueue(ThreadPool &threadPool);

    /*!
     * \brief Method to add a task to the queue
     *
     * When it has run, the result or error will be sent to the callback
     * method.
     */
    void AddTask(TaskPtr task, TaskCallbackPtr callback);

  private:
    /*!
     * \brief Method which schedules a runnable and its requirement
     *
     * Internal use only.
     */
    void AddRunnable(TaskPtr owner, RunnablePtr runnable, RequirementVector requirements);

    /*!
     * \brief Will be called when a runnable is done
     *
     * Will finish the task if it has a result or nothing. If the runnable's
     * result is a deferedrun object its requirements and runnable will be
     * scheduled out.
     *
     * Internal use only.
     */
    void OnRunnableDone(RunnablePtr runnable, TaskResultPtr result);

    /*!
     * \brief Will be called when a task is done
     *
     * Internal use only.
     */
    void OnTaskDone(TaskPtr task, TaskResultPtr result);

    /*!
     * \brief Will be called when finishing task is a requirement to another
     *        task
     *
     * If this task was the last needed requirement the requiring task will be
     * scheduled.
     *
     * Internal use only.
     */
    void OnRequirementDone(RequirementPtr requirement, int error, TaskResultPtr result);

    using RunnableOwnerMap = std::map<RunnablePtr, TaskPtr>;
    using RequirementsOwnerMap = std::map<Requirement*, TaskPtr>;
    using TaskDataMap = std::map<TaskPtr, TaskDataPtr>;
    using TaskDataPair = std::pair<TaskDataMap::key_type, TaskDataMap::value_type>;

    ThreadPool &m_threadPool;
    RunnableOwnerMap m_runnableOwnerMap;
    RequirementsOwnerMap m_requirementsOwnerMap;
    TaskDataMap m_taskDataMap;
    CCriticalSection m_mutex;
    CEvent m_event;
  };
}
}
