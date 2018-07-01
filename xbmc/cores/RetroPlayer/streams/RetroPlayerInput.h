/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "IRetroPlayerStream.h"
#include "games/addons/GameClientCallbacks.h"
#include "peripherals/PeripheralTypes.h"

namespace PERIPHERALS
{
  class CPeripherals;
}

namespace KODI
{
namespace RETRO
{
  class CRetroPlayerInput : public IRetroPlayerStream,
                            public GAME::IGameInputCallback
  {
  public:
    CRetroPlayerInput();
    ~CRetroPlayerInput() override;

    // Implementation of IRetroPlayerStream
    bool OpenStream(const StreamProperties& properties) override;
    bool GetStreamBuffer(unsigned int width, unsigned int height, StreamBuffer& buffer) override { return false; }
    void AddStreamData(const StreamPacket& packet) override;
    void CloseStream() override;
    void SetSpeed(double speed) override;

    // implementation of IGameInputCallback
    void PollInput() override;

  private:
    PERIPHERALS::CPeripherals &m_peripheralManager;

    // Input variables
    PERIPHERALS::EventPollHandlePtr m_inputPollHandle;
  };
}
}
