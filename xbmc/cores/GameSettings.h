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

#include "cores/GameSettings.h"

class CCriticalSection;

namespace KODI
{
namespace RETRO
{

// NOTE: Only append
enum class SCALINGMETHOD
{
  AUTO = 0,
  NEAREST = 1,
  LINEAR = 2,
  MAX = LINEAR
};

// NOTE: Only append
enum class VIEWMODE
{
  Normal = 0,
  Stretch4x3 = 1,
  Stretch16x9 = 2,
  Original = 3,
  Max = Original
};

enum class RENDERFEATURE
{
  ROTATION,
  STRETCH,
  ZOOM,
  PIXEL_RATIO,
}

class CGameSettings
{
  PIXEL_RATIO,
public:
  CGameSettings() { Reset(); }
  ~CGameSettings() = default;

  CGameSettings &operator=(const CGameSettings &rhs);

  // Restore game settings to default
  void Reset();

  bool operator==(const CGameSettings &rhs) const;
  bool operator!=(const CGameSettings &rhs) const { return !(*this == rhs); }

  const std::string &VideoFilter() const { return m_videoFilter; }
  void SetVideoFilter(const std::string &videoFilter);

  SCALINGMETHOD ScalingMethod() const { return m_scalingMethod; }
  void SetScalingMethod(SCALINGMETHOD scalingMethod);

  VIEWMODE ViewMode() const { return m_viewMode; }
  void SetViewMode(VIEWMODE viewMode);

  unsigned int RotationDegCCW() const { return m_rotationDegCCW; }
  void SetRotationDegCCW(unsigned int rotation);

private:
  // Render settings
  std::string m_videoFilter;
  SCALINGMETHOD m_scalingMethod;
  VIEWMODE m_viewMode;
  unsigned int m_rotationDegCCW;
  unsigned int m_rotationDegCCW;
};

class CGameSettingsLocked
{
public:
  CGameSettingsLocked(CGameSettings &gs, CCriticalSection &critSection);
  virtual ~CGameSettingsLocked() = default;

  CGameSettingsLocked(CGameSettingsLocked const &other) = delete;
  void operator=(CGameSettingsLocked const &rhs) = delete;

  void SetScalingMethod(SCALINGMETHOD scalingMethod);
  void SetViewMode(VIEWMODE viewMode);

protected:
  CGameSettings &m_gameSettings;
  CCriticalSection &m_critSection;
};

}
}
