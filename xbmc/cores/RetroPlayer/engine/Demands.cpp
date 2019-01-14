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

#include "Demands.h"
#include "Subjects.h"

#include <utility> //! @todo Move to .cpp

using namespace KODI;
using namespace RETRO;

PredicateObjectDemand::PredicateObjectDemand(Predicate predicate, Object object) :
  m_predicate(std::move(predicate)),
  m_object(std::move(object))
{
}

bool PredicateObjectDemand::Matches(const Subject &subject) const
{
  auto it = subject.GetMetadata().find(m_predicate);
  if (it != subject.GetMetadata().end())
  {
    const Object &object = it->second;
    if (!object.empty())
    {
      if (!m_object.empty())
        return object == m_object;

      return true;
    }
  }

  return false;
}

bool None::Matches(const Subject &subject) const
{
  return !PredicateObjectDemand::Matches(subject);
}

RequiredClass::RequiredClass(Class clazz, bool allowExtended) :
  m_clazz(std::move(clazz)),
  m_allowExtended(allowExtended)
{
}

bool RequiredClass::Matches(const Subject &subject) const
{
  // Input and sought class are same
  if (m_clazz == subject.GetClass())
    return true;

  // Input class may extend towards sought class, but it has not reached
  // there yet
  else if (m_clazz.find(subject.GetClass()) != std::string::npos)
    return false;

  // Input cl;ass is extended version of sought class
  else if (subject.GetClass().find(m_clazz) != std::string::npos)
  {
    if (m_allowExtended)
      return true;
    else
      ;//return Match::NEVER;
  }

  // Input class will never reach sought class, diamond problem
  //return Match::NEVER;
  return false;
}
