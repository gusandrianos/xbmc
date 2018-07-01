/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "RetroPlayerInput.h"
#include "peripherals/EventPollHandle.h"
#include "peripherals/Peripherals.h"
#include "utils/log.h"
#include "ServiceBroker.h"

using namespace KODI;
using namespace RETRO;

CRetroPlayerInput::CRetroPlayerInput() :
  m_peripheralManager(CServiceBroker::GetPeripherals())
{
  CLog::Log(LOGDEBUG, "RetroPlayer[INPUT]: Initializing input");

}

CRetroPlayerInput::~CRetroPlayerInput()
{
  CLog::Log(LOGDEBUG, "RetroPlayer[INPUT]: Deinitializing input");
}

bool CRetroPlayerInput::OpenStream(const StreamProperties& properties)
{
  CLog::Log(LOGDEBUG, "RetroPlayer[INPUT]: Acquiring input handle");
  m_inputPollHandle = m_peripheralManager.RegisterEventPoller();
  return true;
}

void CRetroPlayerInput::AddStreamData(const StreamPacket& packet)
{
  //! @todo
}

void CRetroPlayerInput::CloseStream()
{
  CLog::Log(LOGDEBUG, "RetroPlayer[INPUT]: Releasing input handle");
  m_inputPollHandle.reset();
}

void CRetroPlayerInput::SetSpeed(double speed)
{
  if (speed != 0.0)
    m_inputPollHandle->Activate();
  else
    m_inputPollHandle->Deactivate();
}

void CRetroPlayerInput::PollInput()
{
  m_inputPollHandle->HandleEvents(true);
}
