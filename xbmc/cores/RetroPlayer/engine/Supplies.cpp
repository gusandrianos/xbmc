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

#include "Supplies.h"
#include "Demands.h"

using namespace KODI;
using namespace RETRO;

PredicateObjectSupply::PredicateObjectSupply(Predicate predicate, Object object) :
  m_object(std::move(object))
{
  // Initialize Supply
  m_predicate = std::move(predicate);
}

bool PredicateObjectSupply::Matches(const Demand &demand) const
{
  const PredicateObjectDemand *d = dynamic_cast<const PredicateObjectDemand*>(&demand);

  if (d != nullptr && !d->GetPredicate().empty())
    return m_predicate == d->GetPredicate();

  return false;
}

bool Replace::Conflict(const Supply &supply) const
{
  if (dynamic_cast<const Emit*>(&supply) != nullptr)
    return supply.GetPredicate() == m_predicate;

  return false;
}
