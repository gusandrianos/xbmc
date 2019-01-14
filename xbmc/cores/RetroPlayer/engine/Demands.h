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

#include <memory>

namespace KODI
{
namespace RETRO
{
  /*!
   * \brief Base class for what a task may demand
   */
  class Demand
  {
  public:
    virtual ~Demand() = default;

    virtual bool Matches(const Subject &subject) const = 0;

    //! @todo
    enum class Match
    {
      NEVER,
      NO,
      YES,
    };

    virtual Match MatchesSubject(const Subject &subject) const
    {
      return Match::NEVER;
    }
  };

  /*!
   * \brief Base class for demands which matches against predicates and
   *        possibly objects
   *
   */
  class PredicateObjectDemand : public Demand
  {
  public:
    PredicateObjectDemand(Predicate predicate, Object object);

    ~PredicateObjectDemand() override = default;

    bool Matches(const Subject &subject) const override;

    const Predicate &GetPredicate() const { return m_predicate; }
    const Object &GetObj() const { return m_object; }

  private:
    Predicate m_predicate;
    Object m_object;
  };

  /*!
   * \brief Optional predicate, object keypair
   *
   * The task will be put on hold if any task may provide the pair but will
   * run if it is not provided.
   * */
  class Optional : public PredicateObjectDemand
  {
    //! @todo NOT IMPLEMENTED YET
  };

  /*!
   * \brief A hard dependency
   *
   * The task will never run if its not provided. The task will wait until
   * on any task which might provide or replace the key-value-pair.
   * */
  class Required : public PredicateObjectDemand
  {
  };

  /*!
   * \brief  A key-value-pair which may NOT exist on subject for this task to
   *         run
   *
   * The task will be scheduled after any task which might provide or replace
   * the key-value-pair and will be removed from queue if one emits the
   * key-value-pair.
   */
  class None : public PredicateObjectDemand
  {
  public:
    bool Matches(const Subject &subject) const override;
  };

  /*!
   * \brief A demand on the class of a subject
   */
  class RequiredClass : public Demand
  {
  public:
    RequiredClass(Class clazz, bool allowExtended);

    ~RequiredClass() override = default;

    bool Matches(const Subject &subject) const override;

  private:
    const Class m_clazz;
    const bool m_allowExtended;

  };
}
}
