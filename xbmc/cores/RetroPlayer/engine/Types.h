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

#include "utils/Variant.h"

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <stddef.h>
#include <string>

namespace KODI
{
namespace RETRO
{
  using Class = std::string;
  using Predicate = std::string;
  using Object = CVariant;
  using Metadata = std::map<Predicate, Object>;
  
  class Requirement;
  using RequirementPtr = std::unique_ptr<Requirement>;
  using RequirementVector = std::vector<RequirementPtr>;
  using RequirementMap = std::map<Requirement*, size_t>;

  using RequirementCallback = std::function<void(std::unique_ptr<Requirement>, bool)>;

  class Runnable;
  using RunnablePtr = std::unique_ptr<Runnable>;

  class RunnableCallback
  {
  public:
    virtual ~RunnableCallback() = default;

    virtual void OnFinish(RunnablePtr runnable, bool result) = 0;
  };
  using RunnableCallbackPtr = std::unique_ptr<RunnableCallback>;


  //using Runnable = std::function<bool(std::unique_ptr<Args>)>;
  //using RunnableCallback = std::function<void(Runnable, bool)>;

  class Task;
  using TaskPtr = std::shared_ptr<Task>;
  using TaskVector = std::vector<TaskPtr>;
  using TaskPair = std::pair<TaskPtr, TaskPtr>;
  using TaskPairVector = std::vector<TaskPair>;

  class TaskData;
  using TaskDataPtr = std::unique_ptr<TaskData>;

  class TaskQueue;
  using TaskQueuePtr = std::unique_ptr<TaskQueue>;

  class TaskResult
  {
  public:
    virtual ~TaskResult() = default;
  };
  using TaskResultPtr = std::unique_ptr<TaskResult>;

  using TaskCallback = std::function<void(TaskPtr task, int error, TaskResultPtr result)>;

  class ThreadPool;
  using ThreadPoolPtr = std::unique_ptr<ThreadPool>;

  class Subject;
  using SubjectPtr = std::unique_ptr<Subject>;

  class Demand;
  using DemandPtr = std::unique_ptr<Demand>;
  using DemandVector = std::vector<DemandPtr>;

  class Supply;
  using SupplyPtr = std::unique_ptr<Supply>;
  using SupplyVector = std::vector<SupplyPtr>;

  class SubjectTaskDispatcher;
  using SubjectTaskDispatcherPtr = std::unique_ptr<SubjectTaskDispatcher>;
}
}

