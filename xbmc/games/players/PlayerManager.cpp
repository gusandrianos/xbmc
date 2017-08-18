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
    m_localPlayers.clear();
    return;
  }

  PeripheralVector joysticks;
  m_peripheralManager.GetPeripheralsWithFeature(joysticks, FEATURE_JOYSTICK);

  PeripheralVector keyboards;
  m_peripheralManager.GetPeripheralsWithFeature(keyboards, FEATURE_KEYBOARD);

  PeripheralVector mice;
  m_peripheralManager.GetPeripheralsWithFeature(mice, FEATURE_MOUSE);

  // Look for disconnected joysticks
  for (auto &player : m_localPlayers)
  {
    if (std::find_if(joysticks.begin(), joysticks.end(),
      [&player](const PeripheralPtr &peripheral)
      {
        return player->Peripheral() == peripheral;
      }) == joysticks.end())
    {
      OnDisconnect(*player);
      player.reset();
    }
  }

  m_localPlayers.erase(std::remove_if(m_localPlayers.begin(), m_localPlayers.end(),
    [](const PlayerPtr &port)
    {
      return !port;
    }), m_localPlayers.end());

  // Look for newly connected joysticks
  for (auto &joystick : joysticks)
  {
    if (std::find_if(m_localPlayers.begin(), m_localPlayers.end(),
      [joystick](const PlayerPtr &player)
      {
        return joystick == player->Peripheral();
      }) == m_localPlayers.end())
    {
      PlayerPtr newPlayer = std::make_shared<CPlayer>(joystick, this);
      m_localPlayers.emplace_back(newPlayer);
      m_spectators.emplace_back(std::move(newPlayer));
    }
  }
}

void OnDisconnect(const CPlayer &player)
{
  //! @todo
}
