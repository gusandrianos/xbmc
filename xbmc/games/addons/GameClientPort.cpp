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

#include "GameClientPort.h"
#include "addons/kodi-addon-dev-kit/include/kodi/kodi_game_types.h"
#include "games/controllers/Controller.h"
#include "games/controllers/ControllerLayout.h"
#include "games/controllers/ControllerTopology.h"
#include "games/GameServices.h"
#include "utils/StringUtils.h"
#include "ServiceBroker.h"

#include <algorithm>

using namespace KODI;
using namespace GAME;

#define DEFAULT_PORT_ID  "1"

// --- CGameClientDevice -------------------------------------------------------

CGameClientDevice::CGameClientDevice(const game_input_device &device) :
  m_controller(CServiceBroker::GetGameServices().GetController(device.controller_id ? device.controller_id : "")),
  m_model(device.model ? device.model : ""),
  m_bExclusive(device.exclusive),
  m_bForce(device.force)
{
  if (m_controller && device.available_ports != nullptr)
  {
    // Look for matching ports
    //! @todo Decide if we should enumerate in order of physical or logical ports
    for (const auto &physicalPort : m_controller->Layout().Topology().Ports())
    {
      for (unsigned int i = 0; i < device.port_count; i++)
      {
        const auto &logicalPort = device.available_ports[i];
        if (logicalPort.port_id != nullptr && logicalPort.port_id == physicalPort.ID())
        {
          // Handle matching ports
          HandlePort(logicalPort, physicalPort);
          break;
        }
      }
    }
  }
}

CGameClientDevice::CGameClientDevice(const ControllerPtr &controller) :
  m_controller(controller)
{
}

void CGameClientDevice::HandlePort(const game_input_port &logicalPort, const CControllerPort &physicalPort)
{
  std::unique_ptr<CGameClientPort> port(new CGameClientPort(logicalPort, physicalPort));
  m_ports.emplace_back(std::move(port));
}

bool CGameClientDevice::HasController(const std::string &address) const
{
  auto parts = StringUtils::Split(address, "/", 2);

  if (parts.size() == 2)
  {
    const std::string &portId = parts[0];

    auto it = std::find_if(m_ports.begin(), m_ports.end(),
      [&portId](const std::unique_ptr<CGameClientPort> &port)
      {
        return portId == port->ID();
      });

    if (it != m_ports.end())
    {
      const auto &port = *it;
      return port->HasController(parts[1]);
    }
  }

  return false;
}

// --- CGameClientPort ---------------------------------------------------------

CGameClientPort::CGameClientPort(const game_input_port &port) :
  m_portId(port.port_id ? port.port_id : "")
{
  if (port.accepted_devices != nullptr)
  {
    for (unsigned int i = 0; i < port.device_count; i++)
    {
      std::unique_ptr<CGameClientDevice> device(new CGameClientDevice(port.accepted_devices[i]));

      if (device->Controller() != CController::EmptyPtr)
        m_acceptedDevices.emplace_back(std::move(device));
    }
  }
}

CGameClientPort::CGameClientPort(const ControllerVector &controllers) :
  m_portId(DEFAULT_PORT_ID)
{
  for (const auto &controller : controllers)
    m_acceptedDevices.emplace_back(new CGameClientDevice(controller));
}

CGameClientPort::CGameClientPort(const game_input_port &logicalPort, const CControllerPort &physicalPort) :
  m_portId(physicalPort.ID())
{
  if (logicalPort.accepted_devices != nullptr)
  {
    for (unsigned int i = 0; i < logicalPort.device_count; i++)
    {
      // Ensure device is physically compatible
      const game_input_device &deviceStruct = logicalPort.accepted_devices[i];

      std::string controllerId = deviceStruct.controller_id ? deviceStruct.controller_id : "";
      std::string model = deviceStruct.model ? deviceStruct.model : "";

      if (physicalPort.IsCompatible(controllerId, model))
      {
        std::unique_ptr<CGameClientDevice> device(new CGameClientDevice(deviceStruct));

        if (device->Controller() != CController::EmptyPtr)
          m_acceptedDevices.emplace_back(std::move(device));
      }
    }
  }
}

bool CGameClientPort::HasController(const std::string &address) const
{
  auto parts = StringUtils::Split(address, "/", 2);

  if (!parts.empty())
  {
    const std::string &controllerId = parts[0];

    auto it = std::find_if(m_acceptedDevices.begin(), m_acceptedDevices.end(),
      [&controllerId](const std::unique_ptr<CGameClientDevice> &device)
      {
        return controllerId == device->Controller()->ID();
      });

    if (it != m_acceptedDevices.end())
    {
      // Base case: No further ports were specified
      if (parts.size() == 1)
        return true;

      // Otherwise, recurse deeper
      return (*it)->HasController(parts[1]);
    }
  }

  return false;
}
