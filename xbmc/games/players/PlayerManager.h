/*
 *      Copyright (C) 2017 Team Kodi
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

#include "games/GameTypes.h"
#include "utils/Observer.h"

#include <map>

class CSetting;

namespace PERIPHERALS
{
  class CPeripherals;
}

namespace KODI
{
namespace GAME
{
  class IPlayerFrontend;
  class IPlayerHandler;

  class CPlayerManager : public Observer
  {
  public:
    CPlayerManager(PERIPHERALS::CPeripherals &peripheralManager);
    ~CPlayerManager();

    void RegisterPlayerHandler(IPlayerHandler *handler, GameClientTopology topology);
    void UnregisterPlayerHandler(IPlayerHandler *handler);

    void RegisterFrontend(IPlayerFrontend *frontend);
    void UnregisterFrontend(IPlayerFrontend *frontend);

    // implementation of Observer
    void Notify(const Observable& obs, const ObservableMessage msg) override;

  private:
    void ProcessPeripherals();

    // Construction parameters
    PERIPHERALS::CPeripherals &m_peripheralManager;

    std::map<IPlayerHandler*, GameClientTopology> m_topologies;
  };
} // namespace GAME
}
