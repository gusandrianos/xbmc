/*
 *      Copyright (C) 2012-2015 Team XBMC
 *      http://xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "addons/include/kodi_game_types.h"

#include <string>
#include <vector>

struct game_client_properties;

namespace GAME
{

class CGameClient;

/**
 * \brief Wrapper for game client properties declared in kodi_game_types.h
 */
class CGameClientProperties
{
public:
  CGameClientProperties(const CGameClient* parent, game_client_properties*& props);
  ~CGameClientProperties(void);

  void InitializeProperties(void);

private:
  // Equal to parent's library path
  const char* GetLibraryPath(void);

  // List of proxy DLLs needed to load the game client
  const char* const* GetProxyDllPaths(void);

  // Number of proxy DLLs needed to load the game client
  unsigned int GetProxyDllCount(void) const { return m_proxyDllPaths.size(); }

  // Address of netplay server (TODO: implement auto-discovery)
  const char* GetNetplayServer(void);

  // Port of netplay server (TODO: implement auto-discovery)
  unsigned int GetNetplayServerPort(void);

  // Equal to special://profile/addon_data/<parent's id>/system
  const char* GetSystemDirectory(void);

  // Equal to parent's library path (TODO)
  const char* GetContentDirectory(void);

  // Equal to special://profile/addon_data/<parent's id>/save
  const char* GetSaveDirectory(void);

  // Helper functions
  bool AddProxyDll(const std::string& strLibPath);
  bool HasProxyDll(const char* strLibPath) const;

  const CGameClient* const  m_parent;
  game_client_properties    m_properties;

  // Buffers to hold the strings
  std::string        m_strLibraryPath;
  std::vector<char*> m_proxyDllPaths;
  std::string        m_strNetplayServer;
  std::string        m_strSystemDirectory;
  std::string        m_strContentDirectory;
  std::string        m_strSaveDirectory;
};

} // namespace GAME
