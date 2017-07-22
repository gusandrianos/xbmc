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

#include "games/controllers/ControllerTypes.h"

#include <memory>
#include <string>
#include <vector>

struct game_input_device;
struct game_input_port;

namespace KODI
{
namespace GAME
{
  class CControllerPort;
  class CGameClientPort;

  /*!
   * \ingroup games
   * \brief Represents a device connected to a port
   */
  class CGameClientDevice
  {
  public:
    CGameClientDevice(const game_input_device &device);
    CGameClientDevice(const ControllerPtr &controller);

    const ControllerPtr &Controller() const { return m_controller; }
    const std::string &Model() const { return m_model; }
    bool Exclusive() const { return m_bExclusive; }
    const std::vector<std::unique_ptr<CGameClientPort>> &Ports() const { return m_ports; }

    bool HasController(const std::string &address) const;

  private:
    void HandlePort(const game_input_port &logicalPort, const CControllerPort &physicalPort);

    ControllerPtr m_controller;
    std::string m_model;
    bool m_bExclusive = true;
    bool m_bForce = false;
    std::vector<std::unique_ptr<CGameClientPort>> m_ports;
  };

  /*!
   * \ingroup games
   * \brief Represents a port that devices can connect to
   */
  class CGameClientPort
  {
  public:
    /*!
     * \brief Construct a hardware port
     */
    CGameClientPort(const game_input_port &port);

    /*!
     * \brief Construct a hardware port that accepts the given controllers
     */
    CGameClientPort(const ControllerVector &controllers);

    /*!
     * \brief Construct a controller port
     */
    CGameClientPort(const game_input_port &logicalPort, const CControllerPort &physicalPort);

    /*!
     * \brief Get the ID of the port
     */
    const std::string &ID() const { return m_portId; }

    /*!
     * \brief Get the list of devices accepted by this port
     */
    const std::vector<std::unique_ptr<CGameClientDevice>> &Devices() const { return m_acceptedDevices; }

    bool HasController(const std::string &address) const;

  private:
    std::string m_portId;
    std::vector<std::unique_ptr<CGameClientDevice>> m_acceptedDevices;
  };
}
}
