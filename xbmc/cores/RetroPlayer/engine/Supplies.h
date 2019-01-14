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
   * \brief Base class for what a task may supply
   *
   */
  class Supply
  {
  public:
    virtual ~Supply() = default;

    const Predicate &GetPredicate() const { return m_predicate; }

  protected:
    Predicate m_predicate;
  };

  /*!
   * \brief Base class for supplies which matches against predicates and
   *        possibly objects
   */
  class PredicateObjectSupply : public Supply
  {
  public:
    PredicateObjectSupply(Predicate predicate, Object object);
    ~PredicateObjectSupply() override = default;

    virtual bool Matches(const Demand &demand) const;

    virtual bool Conflict(const Supply &supply) const
    {
      return false;
    }

  private:
    Object m_object;
  };

  /*!
   * \brief The task might supply emit predicate(s) (and object) when run
   */
  class Emit : public PredicateObjectSupply
  {
  public:
    ~Emit() override = default;
  };

  /*!
   * \brief The task might when run replace the predicate (and object(s))
   */
  class Replace : public PredicateObjectSupply
  {
  public:
    ~Replace() override = default;

    bool Conflict(const Supply &supply) const override;
  };

  /*!
   * \brief Will upgrade the predicate object
   *
   * NOT IMPLEMENTED YET.
   */
  class Upgrade : public Replace
  {
  };

  /*!
   * \brief Will upgrade the class of a subject
   *
   * NOT IMPLEMENTED YET.
   */
  class UpgradeClass : public Supply
  {
  };
}
}
