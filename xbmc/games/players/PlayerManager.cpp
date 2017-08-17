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
  m_playerHandlers.insert(std::make_pair(handler, std::move(topology)));

  ProcessPeripherals();
}

void CPlayerManager::UnregisterPlayerHandler(IPlayerHandler *handler)
{
  m_playerHandlers.erase(handler);

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

  if (m_playerHandlers.empty())
  {
    // Nothing to do
    m_ports.clear();
    return;
  }

  PeripheralVector joysticks;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_JOYSTICK);

  PeripheralVector keyboard;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_KEYBOARD);

  PeripheralVector mouse;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_MOUSE);


  //! @todo
  m_joysticks = std::move(joysticks);
  m_keyboard = std::move(keyboard);
  m_mouse = std::move(mouse);


  // Look for disconnected joysticks
  for (auto &port : m_ports)
  {
    if (std::find_if(joysticks.begin(), joysticks.end(),
      [&port](const PeripheralPtr &peripheral)
      {
        return port->Peripheral() == peripheral;
      }) == joysticks.end())
    {
      OnDisconnect(*port);
      port.reset();
    }
  }

  m_ports.erase(std::remove_if(m_ports.begin(), m_ports.end(),
    [](const PortPtr &port)
    {
      return !port;
    }), m_ports.end());

  // Look for newly connected joysticks
  for (auto &joystick : joysticks)
  {
    if (std::find_if(m_ports.begin(), m_ports.end(),
      [joystick](const PortPtr &port)
      {
        return joystick == port->Peripheral();
      }) == m_ports.end())
    {
      PortPtr newPort = std::make_shared<CPort>(joystick->Location(), this);

      newPort->RegisterInput(joystick.get());

      m_portMap.insert(std::make_pair(joystick.get(), std::move(newPort)));
      m_ports.emplace_back(std::make_shared<CPort>(joystick.get()));
    }
  }


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

void OnDisconnect(const CPort &port)
{
  //! @todo
}
