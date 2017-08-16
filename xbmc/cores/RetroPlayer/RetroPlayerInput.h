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

#include "games/addons/GameClientCallbacks.h"
#include "games/players/IPlayerHandler.h"
#include "peripherals/PeripheralTypes.h"
#include "utils/Observer.h"

#include <map>

namespace PERIPHERALS
{
  class CPeripherals;
}

namespace KODI
{
namespace GAME
{
  class CGameClient;
}

namespace RETRO
{
  class CRetroPlayerAgent;

  class CRetroPlayerInput : public GAME::IGameInputCallback,
                            public GAME::IPlayerHandler,
                            public Observer
  {
  public:
    CRetroPlayerInput(const GAME::CGameClient &gameClient, PERIPHERALS::CPeripherals &peripheralManager);
    ~CRetroPlayerInput() override;

    void SetSpeed(double speed);

    void HardwareReset();

    // implementation of IGameInputCallback
    void PollInput() override;

    // implementation of IPlayerHandler
    void AddPlayer(unsigned int index, const std::string &controllerAddress) override;
    void UpdateAddress(unsigned int index, const std::string &controllerAddress) override;
    void RemovePlayer(unsigned int index) override;

    // implementation of Observer
    void Notify(const Observable &obs, const ObservableMessage msg) override;

  private:
    void UpdatePeripherals();

    // Construction parameters
    GAME::CGameClient &m_gameClient;
    PERIPHERALS::CPeripherals &m_peripheralManager;

    // Input variables
    PERIPHERALS::EventPollHandlePtr m_inputPollHandle;
    std::map<unsigned int, CRetroPlayerAgent> m_agents;
  };
}
}
