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

#include "PlayerManager.h"
#include "peripherals/Peripherals.h"

using namespace KODI;
using namespace GAME;

CPlayerManager::CPlayerManager(PERIPHERALS::CPeripherals &peripheralManager) :
  m_peripheralManager(peripheralManager)
{
  m_peripheralManager.RegisterObserver(this);
}

CPlayerManager::~CPlayerManager()
{
  m_peripheralManager.UnregisterObserver(this);
}

void CPlayerManager::RegisterPlayerHandler(IPlayerHandler *handler, GameClientTopology topology)
{
  m_topologies.insert(std::make_pair(handler, std::move(topology)));

  ProcessPeripherals();
}

void CPlayerManager::UnregisterPlayerHandler(IPlayerHandler *handler)
{
  m_topologies.erase(handler);

  ProcessPeripherals();
}

void CPlayerManager::Notify(const Observable &obs, const ObservableMessage msg)
{
  switch (msg)
  {
    case ObservableMessagePeripheralsChanged:
      ProcessPeripherals();
      break;
    default:
      break;
  }
}

void CPlayerManager::ProcessPeripherals()
{
  using namespace PERIPHERALS;

  PeripheralVector joysticks;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_JOYSTICK);

  PeripheralVector keyboard;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_KEYBOARD);

  PeripheralVector mouse;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_MOUSE);

  std::vector<CPeripheral*> disconnectedJoysticks;
  std::vector<CPeripheral*> m_joysticks;
  for (auto joystick : m_joysticks)
  {
    if (std::find_if(joysticks.begin(), joysticks.end(),
      [joystick](const PeripheralPtr &peripheral)
      {
        return joystick == peripheral.get();
      }) == joysticks.end())
    {
      disconnectedJoysticks.push_back(joystick);
    }
  }

  for (auto& joystick : joysticks)
  {

    auto itConnectedPort = newPortMap.find(joystick.get());
    auto itDisconnectedPort = m_portMap.find(joystick.get());

    IInputHandler* newHandler = itConnectedPort != newPortMap.end() ? itConnectedPort->second : nullptr;
    IInputHandler* oldHandler = itDisconnectedPort != m_portMap.end() ? itDisconnectedPort->second->InputHandler() : nullptr;

    if (oldHandler != newHandler)
    {
      // Unregister old handler
      if (oldHandler != nullptr)
      {
        PortPtr& oldPort = itDisconnectedPort->second;

        oldPort->UnregisterInput(joystick.get());

        m_portMap.erase(itDisconnectedPort);
      }

      // Register new handler
      if (newHandler != nullptr)
      {
        CGameClient *gameClient = m_portManager.GameClient(newHandler);
        if (gameClient)
        {
          PortPtr newPort(new CPort(newHandler, *gameClient));

          newPort->RegisterInput(joystick.get());

          m_portMap.insert(std::make_pair(joystick.get(), std::move(newPort)));
        }
      }
    }
  }
}
