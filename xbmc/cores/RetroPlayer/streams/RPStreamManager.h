/*
 *  Copyright (C) 2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IStreamManager.h"
#include "RetroPlayerStreamTypes.h"

#include <vector>

namespace KODI
{
namespace RETRO
  {
  class CRPProcessInfo;
  class CRPRenderManager;
  class IRetroPlayerStream;

  class CRPStreamManager : public IStreamManager
  {
  public:
    CRPStreamManager(CRPRenderManager& renderManager, CRPProcessInfo& processInfo);
    ~CRPStreamManager() override;

    // Functions called from the player
    void EnableAudio(bool bEnable);
    void SetSpeed(double speed);

    // Implementation of IStreamManager
    StreamPtr CreateStream(StreamType streamType) override;
    void CloseStream(StreamPtr stream) override;

  private:
    // Construction parameters
    CRPRenderManager& m_renderManager;
    CRPProcessInfo& m_processInfo;

    // Stream parameters
    std::vector<StreamPtr> m_streams;
  };
}
}
