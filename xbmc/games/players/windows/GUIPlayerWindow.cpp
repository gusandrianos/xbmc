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

#include "GUIPlayerWindow.h"
#include "GUIPlayerPanel.h"
#include "GUIPlayerWindowDefines.h"
#include "GUIVirtualPortPanel.h"
#include "IPlayerWindow.h"
#include "addons/AddonManager.h"
#include "games/addons/GameClient.h"
#include "games/controllers/Controller.h"
#include "games/controllers/ControllerFeature.h"
#include "games/tags/GameInfoTag.h"
#include "games/GameServices.h"
#include "guilib/GUIMessage.h"
#include "guilib/LocalizeStrings.h"
#include "guilib/WindowIDs.h"
#include "settings/GameSettings.h"
#include "settings/MediaSettings.h"
#include "settings/Settings.h"
#include "utils/Variant.h"
#include "Application.h"
#include "FileItem.h"
#include "ServiceBroker.h"

using namespace KODI;
using namespace GAME;

CGUIPlayerWindow::CGUIPlayerWindow() :
  CGUIDialog(WINDOW_DIALOG_GAME_PLAYERS, "DialogGamePlayers.xml"),
  m_hotkeys(new CFileItemList)
{
  // initialize CGUIWindow
  m_loadType = KEEP_IN_MEMORY;
}

CGUIPlayerWindow::~CGUIPlayerWindow() = default;

void CGUIPlayerWindow::DoProcess(unsigned int currentTime, CDirtyRegionList &dirtyregions)
{
  //! @todo Fade player focus texture when player is being moved

  CGUIDialog::DoProcess(currentTime, dirtyregions);

  //! @todo Unfade player texture and remove focus
}

bool CGUIPlayerWindow::OnMessage(CGUIMessage& message)
{
  // Set to true to block the call to the super class
  bool bHandled = false;

  switch (message.GetMessage())
  {
    case GUI_MSG_CLICKED:
    {
      int controlId = message.GetSenderId();

      if (controlId == CONTROL_ACTION_ADD_PORT)
      {
        OnAddPort();
        bHandled = true;
      }
      else if (controlId == CONTROL_ACTION_REMOVE_PORT)
      {
        OnRemovePort();
        bHandled = true;
      }
      else if (controlId == CONTROL_ACTION_EXCLUSIVE_PORTS)
      {
        OnExclusivePortsSelected();
        bHandled = true;
      }
      break;
    }
    case GUI_MSG_FOCUSED:
    {
      int controlId = message.GetControlId();

      /*
      if (CONTROL_CONTROLLER_BUTTONS_START <= controlId && controlId < CONTROL_CONTROLLER_BUTTONS_END)
      {
        OnControllerFocused(controlId - CONTROL_CONTROLLER_BUTTONS_START);
      }
      else if (CONTROL_FEATURE_BUTTONS_START <= controlId && controlId < CONTROL_FEATURE_BUTTONS_END)
      {
        OnFeatureFocused(controlId - CONTROL_FEATURE_BUTTONS_START);
      }
      */

      break;
    }
    case GUI_MSG_SETFOCUS:
    {
      int controlId = message.GetControlId();

      /*
      if (CONTROL_CONTROLLER_BUTTONS_START <= controlId && controlId < CONTROL_CONTROLLER_BUTTONS_END)
      {
        OnControllerFocused(controlId - CONTROL_CONTROLLER_BUTTONS_START);
      }
      else if (CONTROL_FEATURE_BUTTONS_START <= controlId && controlId < CONTROL_FEATURE_BUTTONS_END)
      {
        OnFeatureFocused(controlId - CONTROL_FEATURE_BUTTONS_START);
      }
      */

      break;
    }
    default:
      break;
  }

  if (!bHandled)
    bHandled = CGUIDialog::OnMessage(message);

  return bHandled;
}

void CGUIPlayerWindow::OnInitWindow()
{
  // Expose game client name to skin
  m_gameClient = GetGameClient();

  if (m_gameClient)
    SetProperty(WINDOW_PROPERTY_ADDON_NAME, m_gameClient->Name());
  else
  {
    // Fallback error message
    SetProperty(WINDOW_PROPERTY_ADDON_NAME, g_localizeStrings.Get(35202)); // "Error: Not playing a game"
  }

  CGUIDialog::OnInitWindow();

  //! @todo Create GUI elements
  m_virtualPorts.reset(new CGUIVirtualPortPanel(m_gameClient->ControllerTopology()));

  // Populate list of hotkeys
  InitHotkeys();

  CGUIPlayerPanel *playerPanel = dynamic_cast<CGUIPlayerPanel*>(GetControl(CONTROL_PLAYER_PANEL));
  if (playerPanel != nullptr)
  {
    //! @todo
  }

  UpdateButtons();
}

void CGUIPlayerWindow::OnDeinitWindow(int nextWindowID)
{
  m_hotkeys->Clear();

  //! @todo Destroy GUI elements

  CGUIDialog::OnDeinitWindow(nextWindowID);

  SaveSettings();
}

void CGUIPlayerWindow::OnAddPort()
{
  //! @todo
  UpdateButtons();
}

void CGUIPlayerWindow::OnRemovePort()
{
  //! @todo
  UpdateButtons();
}

void CGUIPlayerWindow::OnExclusivePortsSelected()
{
  CGameSettings &currentSettings = CMediaSettings::GetInstance().GetCurrentGameSettings();

  currentSettings.SetExclusivePorts(!currentSettings.ExclusivePorts());

  UpdateButtons();
}

void CGUIPlayerWindow::InitHotkeys()
{
  m_hotkeys->Clear();

  ControllerPtr controller = CServiceBroker::GetGameServices().GetDefaultController();
  if (!controller)
    return;

  {
    CFileItem item(g_localizeStrings.Get(35235)); // "Add port"
    item.SetLabel2(GetHotkey(controller, "up"));
    m_hotkeys->Add(std::move(item));
  }

  {
    CFileItem item(g_localizeStrings.Get(35236)); // "Remove port"
    item.SetLabel2(GetHotkey(controller, "down"));
    m_hotkeys->Add(std::move(item));
  }

  {
    CFileItem item(g_localizeStrings.Get(35232)); // "Move left"
    item.SetLabel2(GetHotkey(controller, "left"));
    m_hotkeys->Add(std::move(item));
  }

  {
    CFileItem item(g_localizeStrings.Get(35233)); // "Move right"
    item.SetLabel2(GetHotkey(controller, "right"));
    m_hotkeys->Add(std::move(item));
  }

  {
    CFileItem item(g_localizeStrings.Get(35234)); // "Player menu"
    item.SetLabel2(GetHotkey(controller, "a"));
    m_hotkeys->Add(std::move(item));
  }

  CGUIMessage msg(GUI_MSG_LABEL_BIND, GetID(), CONTROL_HOTKEY_LIST, 0, 0, m_hotkeys.get());
  OnMessage(msg);
}

void CGUIPlayerWindow::UpdateButtons()
{
  //! @todo Enable/disable Add/Remove Port buttons

  CGameSettings &currentSettings = CMediaSettings::GetInstance().GetCurrentGameSettings();

  if (currentSettings.ExclusivePorts())
    SetProperty(WINDOW_PROPERTY_EXCLUSIVE_MODE, "true");
  else
    SetProperty(WINDOW_PROPERTY_EXCLUSIVE_MODE, "");
}

void CGUIPlayerWindow::SaveSettings()
{
  CGameSettings &defaultSettings = CMediaSettings::GetInstance().GetDefaultGameSettings();
  CGameSettings &currentSettings = CMediaSettings::GetInstance().GetCurrentGameSettings();

  if (defaultSettings != currentSettings)
  {
    defaultSettings = currentSettings;
    CServiceBroker::GetSettings().Save();
  }
}

GameClientPtr CGUIPlayerWindow::GetGameClient() const
{
  using namespace ADDON;

  std::string gameClientId;

  if (g_application.CurrentFileItem().HasGameInfoTag())
    gameClientId = g_application.CurrentFileItem().GetGameInfoTag()->GetGameClient();

  AddonPtr addon;
  if (!gameClientId.empty())
    CAddonMgr::GetInstance().GetAddon(gameClientId, addon, ADDON::ADDON_GAMEDLL);

  return std::static_pointer_cast<CGameClient>(addon);
}

std::string CGUIPlayerWindow::GetHotkey(const ControllerPtr &controller, const std::string &featureName) const
{
  //! @todo Make hotkey configurable
  return "Select + " + controller->GetFeature(featureName).Label();
}
