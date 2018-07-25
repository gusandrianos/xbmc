/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "games/controllers/types/ControllerTree.h"
#include "games/GameTypes.h"

#include <memory>

struct game_input_topology;
struct game_input_port;
struct game_input_device;

namespace KODI
{
namespace GAME
{
  class CGameClientTopology
  {
  public:
    CGameClientTopology() = default;
    CGameClientTopology(GameClientPortVec ports, int playerLimit);

    void Clear();

    int PlayerLimit() const { return m_playerLimit; }

    const CControllerTree &ControllerTree() const { return m_controllers; }
    CControllerTree &ControllerTree() { return m_controllers; }

    game_input_topology *GetTopology() const;
    void FreeTopology(game_input_topology *inputTopology) const;

  private:
    static CControllerTree GetControllerTree(const GameClientPortVec &ports);
    static CControllerPortNode GetPortNode(const GameClientPortPtr &port, const std::string &address);
    static CControllerNode GetControllerNode(const GameClientDevicePtr &device, const std::string &portAddress);

    static game_input_topology *GetTopology(const ControllerPortVec &ports, int playerLimit);
    static void GetPort(const CControllerPortNode &portNode, game_input_port &gamePort);
    static void FreePort(game_input_port &gamePort);
    static void GetDevice(const CControllerNode &controllerNode, game_input_device &gameDevice);
    static void FreeDevice(game_input_device &gameDevice);

    // Utility function
    static std::string MakeAddress(const std::string &baseAddress, const std::string &nodeId);

    // Game API parameters
    GameClientPortVec m_ports;
    int m_playerLimit = -1;

    // Controller parameters
    CControllerTree m_controllers;
  };
}
}
