/*
 *      Copyright (C) 2018 Team Kodi
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

#include "DialogGameRewind.h"
//#include "cores/RetroPlayer/guibridge/GUIGamePlaybackHandle.h" //! @todo
#include "cores/RetroPlayer/guibridge/GUIGameRenderManager.h"
#include "guilib/GUIMessage.h"
#include "guilib/WindowIDs.h"
#include "ServiceBroker.h"

using namespace KODI;
using namespace GAME;

CDialogGameRewind::CDialogGameRewind() :
  CGUIDialog(WINDOW_DIALOG_GAME_REWIND, "DialogSeekBar.xml")
{
  // Initialize CGUIWindow
  m_loadType = KEEP_IN_MEMORY;
}

bool CDialogGameRewind::OnMessage(CGUIMessage &message)
{
  switch (message.GetMessage())
  {
    case GUI_MSG_WINDOW_INIT:
    {
      // Don't init this dialog if we aren't playing a game
      if (!RegisterDialog())
        return false;

      break;
    }
    case GUI_MSG_WINDOW_DEINIT:
    {
      UnregisterDialog();

      break;
    }
    default:
      break;
  }

  return CGUIDialog::OnMessage(message);
}

bool CDialogGameRewind::RegisterDialog()
{
  //m_gamePlaybackHandle = CServiceBroker::GetGameRenderManager().RegisterPlaybackDialog();

  //return m_gamePlaybackHandle.get() != nullptr;

  return true;
}

void CDialogGameRewind::UnregisterDialog()
{
  m_gamePlaybackHandle.reset();
}
