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

#include "Core.h"
#include "Subjects.h"
#include "TaskQueues.h"
#include "Tasks.h"
#include "ThreadPools.h"

using namespace KODI;
using namespace RETRO;

Engine::Engine() :
  m_threadPool(new MainloopThreadPool)
{
}

void Engine::RegisterModule(TaskVector module)
{
  for (TaskPtr &task : module)
  {
    if (dynamic_cast<SubjectTask*>(task.get()) != nullptr)
      m_registeredTasks.emplace_back(std::move(task));
  }
}

SubjectTaskDispatcherPtr Engine::Get(SubjectPtr subject, TaskCallback callback)
{
  SubjectTaskDispatcherPtr dispatcher;

  TaskQueuePtr taskQueue(new TaskQueue(*m_threadPool));
  dispatcher.reset(new SubjectTaskDispatcher{std::move(subject), std::move(m_registeredTasks), std::move(taskQueue), callback});

  //! @todo Should not return, should just keep a reference so it can be paused
  return dispatcher;
}
