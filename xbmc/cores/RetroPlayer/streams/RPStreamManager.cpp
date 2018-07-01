/*
 *  Copyright (C) 2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "RPStreamManager.h"
#include "IRetroPlayerStream.h"
#include "RetroPlayerAudio.h"
//#include "RetroPlayerHardwareBuffer.h" //! @todo
#include "RetroPlayerInput.h"
#include "RetroPlayerMemory.h"
//#include "RetroPlayerSoftwareBuffer.h" //! @todo
#include "RetroPlayerVideo.h"

using namespace KODI;
using namespace RETRO;

CRPStreamManager::CRPStreamManager(CRPRenderManager& renderManager, CRPProcessInfo& processInfo) :
  m_renderManager(renderManager),
  m_processInfo(processInfo)
{
}

CRPStreamManager::~CRPStreamManager()
{
  for (const StreamPtr &stream : m_streams)
    stream->CloseStream();
}

void CRPStreamManager::SetSpeed(double speed)
{
  for (const StreamPtr &stream : m_streams)
    stream->SetSpeed(speed);
}

StreamPtr CRPStreamManager::CreateStream(StreamType streamType)
{
  StreamPtr stream;

  switch (streamType)
  {
  case StreamType::AUDIO:
  {
    stream.reset(new CRetroPlayerAudio(m_processInfo));
    break;
  }
  case StreamType::VIDEO:
  case StreamType::SW_BUFFER:
  {
    stream.reset(new CRetroPlayerVideo(m_renderManager, m_processInfo));
    break;
  }
  case StreamType::HW_BUFFER:
  {
    //stream.reset(new CRetroPlayerHardware(m_renderManager, m_processInfo)); //! @todo
    break;
  }
  case StreamType::MEMORY:
  {
    stream.reset(new CRetroPlayerMemory);
    break;
  }
  case StreamType::INPUT:
  {
    stream.reset(new CRetroPlayerInput);
    break;
  }

  default:
    break;
  }

  if (stream)
    m_streams.push_back(stream);

  return stream;
}

void CRPStreamManager::CloseStream(StreamPtr stream)
{
  if (stream)
  {
    auto it = std::find(m_streams.begin(), m_streams.end(), stream);
    if (it != m_streams.end())
      m_streams.erase(it);

    stream->CloseStream();
  }
}
