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

#include <deque>
#include <functional>
#include <memory>
#include <utility> //! @todo

namespace KODI
{
namespace RETRO
{
  class Trigger
  {
  };

  /*
  class TaskCallback
  {
  public:
    virtual ~TaskCallback() = 0;

    virtual void OnDone(TaskPtr task, int error, TaskResultPtr result) = 0;

    virtual void OnError(int error, TaskResultPtr result) = 0;
  };
  */

  class Runnable
  {
  public:
    virtual ~Runnable() = default;

    /*!
     * \brief This method is called when all requirements are fulfilled in
     *        the requires return
     *
     * The function will be called with a list of the requirements
     * in the same order as the require function returned its dependencies.
     *
     * The task may return anything which
     * is meant to be sent to a triggering task
     */
    virtual bool Run(Object args) = 0;
  };

  /*!
   * \brief An object which may be returned from a run
   *
   * The object tells the taskqueue what the next stage (runnable) is of the
   * task and what requirements it has.
   */
  class DeferredRun : public TaskResult
  {
  public:
    DeferredRun(RunnablePtr runnable, RequirementVector requirements);

    RunnablePtr &&ReleaseRunnable();
    RequirementVector &&ReleaseRequirements();

  private:
    RunnablePtr m_runnable;
    RequirementVector m_requirements;
  };

  class Task : public Runnable
  {
  public:
    virtual ~Task() = default;

    /*!
     * \brief The first stage of the task
     *
     * In the default implementation, this will just return a deferred run,
     * one which requires all from function Require() and runs the method run.
     */
    std::unique_ptr<DeferredRun> PreFlight();

    /*!
     * \brief Return required tasks to run this task
     *
     * All tasks needed should be created, i.e. return objects not classes.
     * The data returned by an required task will be piped to the run method
     * in order of require.
     */
    virtual RequirementVector Require() = 0;

    //! @todo
    const DemandVector &Demands() { return m_demands; }

  private:
    DemandVector m_demands;
  };

  /*!
   * \brief Subject Tasks are run "on" a subject, and tied to this subject
   *
   * The subject will through the trigger member add this task to the task
   * queue automatically, making it possible for a SubjectTask to specify
   * when it should run but leaving control of actual launch to the subject
   * and task queue.
   */
  class SubjectTask : public Task
  {
  public:
    SubjectTask(SubjectPtr subject);
    virtual ~SubjectTask() = default;

    std::unique_ptr<Trigger> trigger;
    std::unique_ptr<Supply> supply;

  private:
    const SubjectPtr m_subject;
  };
}
}
