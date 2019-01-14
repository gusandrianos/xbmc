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
#include "Tasks.h"
#include "threads/Event.h"
#include "threads/CriticalSection.h"

namespace KODI
{
namespace RETRO
{
  class SubjectUtils
  {
  private:
    //static bool AllTasks(const TaskVector &tasks, std::function<bool(const Subject &subject)> predicate);
    //static bool AnyTask(const TaskVector &tasks, std::function<bool(const Subject &subject)> predicate);
    static bool AllDemandsMatch(const DemandVector &demands, const Subject &subject);
    static bool AnyDemandDoesntMatch(const DemandVector &demands, const Subject &subject);
    static bool Conflicts(const Task &thiz, const Task &that);
    // Get 2-permutations of given tasks
    static TaskPairVector Get2Permutations(TaskVector tasks);

  public:
    static TaskVector FindDoableTasks(const Subject &subject, TaskVector givenTasks);
    static TaskVector FindImpossibleTasks(const Subject &subject, TaskVector givenTasks);
    static TaskVector PurgeImpossibleTasks(const Subject &subject, TaskVector givenTasks);
    static TaskVector FindConflictingTasks(TaskVector givenTasks);
    static TaskVector PurgeConflictingTasks(TaskVector tasks);

    static Object IsolateIfSingle(const Object &object);
  };

  class Subject
  {
  public:
    Subject(Class clazz, Metadata metadata);

    const Class &GetClass() const { return m_clazz; }
    const Metadata &GetMetadata() const { return m_metadata; }

    Object GetItem(const Predicate &name) const;

    void Emit(const Predicate &predicate, Object object);

    void Replace(const Predicate &predicate, Object object);

  private:
    Class m_clazz;
    Metadata m_metadata;
  };

  class SubjectTaskDispatcherCallback : TaskCallback
  {
  public:
    SubjectTaskDispatcherCallback(SubjectTaskDispatcher &dispatcher);
    virtual ~SubjectTaskDispatcherCallback() = default;

    void OnDone(TaskPtr task, int error, TaskResultPtr result);

    void OnError(int error, TaskResultPtr result) { } // Not used

  private:
    SubjectTaskDispatcher &m_dispatcher;
  }

  class SubjectTaskDispatcher
  {
  public:
    SubjectTaskDispatcher(SubjectPtr subject, TaskVector subjectTasks, TaskQueuePtr taskQueue, TaskCallback callback);

    void ScheduleNonConflictingTasks();

    void OnDone(TaskPtr task, int error, TaskResultPtr result);

  private:
    SubjectPtr m_subject;
    TaskVector m_runningTasks;
    TaskVector m_availableTasks;
    TaskQueuePtr m_taskQueue;
    TaskCallbackPtr m_callback;

    SubjectTaskDispatcherCallback m_dispatcherCallback;
    TaskVector m_taskPath; // For debugging purposes
    CCriticalSection m_mutex;
    CEvent m_event;
  };
}
}
