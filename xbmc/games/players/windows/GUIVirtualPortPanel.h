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

#include "IPlayerWindow.h"
#include "games/GameTypes.h"

namespace KODI
{
namespace GAME
{
  class CGUIVirtualPortPanel : public IVirtualPortPanel
  {
  public:
    CGUIVirtualPortPanel(const GameClientTopology &controllerTopology);
    ~CGUIVirtualPortPanel() override;

    // implementation of IVirtualPortPanel
    void OnAddPort() override { }
    void OnRemovePort() override { }
    void OnSelect(unsigned int column, unsigned int row) override { }
    bool HasDevice(unsigned int column, unsigned int row) override { return false; }

  private:
    GameClientTopology m_topology;
  };
}
}
