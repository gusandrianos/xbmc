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

#include "Subjects.h"
#include "Demands.h"
#include "TaskQueues.h"
#include "Tasks.h"

#include <algorithm>

using namespace KODI;
using namespace RETRO;

TaskVector SubjectUtils::FindDoableTasks(const Subject &subject, TaskVector givenTasks)
{
  TaskVector doableTasks;

  for (TaskPtr &task : givenTasks)
  {
    if (AllDemandsMatch(task->Demands(), subject))
      doableTasks.emplace_back(std::move(task));
  }

  return doableTasks;
}

TaskVector SubjectUtils::FindImpossibleTasks(const Subject &subject, TaskVector givenTasks)
{
  TaskVector impossibleTasks;

  for (TaskPtr &task : givenTasks)
  {
    if (AnyDemandDoesntMatch(task->Demands(), subject))
      impossibleTasks.emplace_back(std::move(task));
  }

  return impossibleTasks;
}

TaskVector SubjectUtils::PurgeImpossibleTasks(const Subject &subject, TaskVector givenTasks)
{
  TaskVector impossibleTasks = FindImpossibleTasks(subject, givenTasks);

  TaskVector remainingTasks;

  for (TaskPtr &task : givenTasks)
  {
    if (std::find(impossibleTasks.begin(), impossibleTasks.end(), task) == impossibleTasks.end())
      remainingTasks.emplace_back(std::move(task));
  }

  return remainingTasks;
}

TaskVector SubjectUtils::FindConflictingTasks(TaskVector givenTasks)
{
  TaskVector conflictingTasks;

  // Look through 2-permutations of given tasks
  TaskPairVector permutations = Get2Permutations(givenTasks);
  for (TaskPair &taskPair : permutations)
  {
    if (taskPair.first != taskPair.second &&
        Conflicts(*taskPair.first, *taskPair.second))
    {
      conflictingTasks.push_back(taskPair.first);
    }
  }

  return conflictingTasks;
}

TaskVector SubjectUtils::PurgeConflictingTasks(TaskVector tasks)
{
  TaskVector conflictingTasks = FindConflictingTasks(tasks);

  TaskVector remainingTasks;

  for (TaskPtr &task : tasks)
  {
    if (std::find(conflictingTasks.begin(), conflictingTasks.end(), task) == conflictingTasks.end())
      remainingTasks.emplace_back(std::move(task));
  }

  return remainingTasks;
}
/*
bool SubjectUtils::AllTasks(const TaskVector &tasks, std::function<bool(const Subject &subject)> predicate)
{
  return std::find_if(tasks.begin(), tasks.end(), [predicate](const TaskPtr &subject)
    {
      return !predicate(subject);
    }) == tasks.end();
}

bool SubjectUtils::AnyTask(const TaskVector &tasks, std::function<bool(const Subject &subject)> predicate)
{
  return std::find_if(tasks.begin(), tasks.end(), predicate) != tasks.end();
}
*/
bool SubjectUtils::AllDemandsMatch(const DemandVector &demands, const Subject &subject)
{
  return std::find_if(demands.begin(), demands.end(), [&subject](const DemandPtr &demand)
    {
      return !demand->Matches(subject); //! @todo Matches Demands::Match enum?
    }) == demands.end();
}

bool SubjectUtils::AnyDemandDoesntMatch(const DemandVector &demands, const Subject &subject)
{
  return std::find_if(demands.begin(), demands.end(), [&subject](const DemandPtr &demand)
    {
      return demand->MatchesSubject(subject) == Demand::Match::NEVER;
    }) != demands.end();
}

bool SubjectUtils::Conflicts(const Task &thiz, const Task &that)
{
  /*! @todo
   for (auto ss : thiz.supply)
   {
   for (auto os : that.supply)
   {
   if (ss.Conflicts(os))
   return true;
   }
   }
   */

  return false;
}

Object SubjectUtils::IsolateIfSingle(const Object &object)
{
  if (object.isNull() || object.empty())
    return Object();
  else if (object.isArray() && object.size() == 1)
    return *object.begin_array();
  else
    return object;
}

Subject::Subject(Class clazz, Metadata metadata) :
  m_clazz(std::move(clazz))
{
  for (auto it : metadata)
    m_metadata[it.first].push_back(std::move(it.second));
}

Object Subject::GetItem(const Predicate &name) const
{
  auto it = m_metadata.find(name);
  if (it != m_metadata.end())
    return SubjectUtils::IsolateIfSingle(it->second);

  return Object();
}

void Subject::Emit(const Predicate &predicate, Object object)
{
  if (!object.empty())
    m_metadata[predicate].push_back(std::move(object));
}

void Subject::Replace(const Predicate &predicate, Object object)
{
  auto it = m_metadata.find(predicate);
  if (it != m_metadata.end())
    m_metadata.erase(it);

  if (!object.empty())
    m_metadata[predicate].push_back(std::move(object));
}

SubjectTaskDispatcherCallback::SubjectTaskDispatcherCallback(SubjectTaskDispatcher &dispatcher) :
  m_dispatcher(dispatcher)
{
}

void SubjectTaskDispatcherCallback::OnDone(TaskPtr task, int error, TaskResultPtr result)
{
  m_dispatcher.OnDone(std::move(task), error, std::move(taskResult));
}

SubjectTaskDispatcher::SubjectTaskDispatcher(SubjectPtr subject, TaskVector subjectTasks, TaskQueuePtr taskQueue, TaskCallback callback) :
  m_subject(std::move(subject)),
  m_availableTasks(std::move(subjectTasks)),
  m_taskQueue(std::move(taskQueue)),
  m_callback(callback),
{
}

void SubjectTaskDispatcher::ScheduleNonConflictingTasks()
{
  TaskVector possibleTasks = SubjectUtils::PurgeImpossibleTasks(*m_subject, std::move(m_availableTasks));
  TaskVector doableTasks = SubjectUtils::FindDoableTasks(*m_subject, std::move(possibleTasks));
  doableTasks = SubjectUtils::PurgeConflictingTasks(std::move(doableTasks));

  if (!doableTasks.empty())
  {
    for (TaskPtr &task : doableTasks)
    {
      //createdTask = t(self.subject)
      auto createdTask = task->Run(Object(m_subject->GetMetadata())); //! @todo: Metadata -> Object
      //self.runningTasks.append(createdTask)
      m_runningTasks.emplace_back(createdTask);
      //self.taskQueue.addTask(createdTask, self.onDone)
      auto onDone = [this](TaskPtr task, int error, TaskResultPtr result)
        {
          this->OnDone(std::move(task), error, std::move(taskResult));
        };
      m_taskQueue->AddTask(std::move(createdTask), onDone)
    }

    m_taskPath.emplace_back(std::move(doableTasks));
  }
  else
  {
    //! @todo Debug log: Final scheduling order became <taskPath>
    //m_callback->Callback(0, m_subject);
  }
}

void SubjectTaskDispatcher::OnDone(TaskPtr task, int error, TaskResultPtr result)
{
  if (error != 0)
    m_callback->OnError(error, nullptr);
  else
  {
    CSingleLock lock(m_mutex);

    // Remove task from running tasks
    m_runningTasks.erase(
      std::remove(m_runningTasks.begin(), m_runningTasks.end(),task),
      m_runningTasks.end()
    );

    if (m_runningTasks.empty())
      ScheduleNonConflictingTasks();
  }
}
