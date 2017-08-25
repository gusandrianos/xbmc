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

#include "DialogGameSaves.h"
#include "guilib/LocalizeStrings.h"
#include "guilib/GUIMessage.h"
#include "guilib/WindowIDs.h"

using namespace KODI;
using namespace GAME;

CDialogGameSaves::CDialogGameSaves(void) :
  CDialogGameVideoSelect(WINDOW_DIALOG_GAME_SAVES)
{
}

bool CDialogGameSaves::OnMessage(CGUIMessage &message)
{
  switch (message.GetMessage())
  {
    case GUI_MSG_WINDOW_INIT:
    {
      RegisterDialog();

      //if (!m_gameSaveHandle)
      //  return false;

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

std::string CDialogGameSaves::GetHeading()
{
  return g_localizeStrings.Get(35249); // "Saved games"
}

void CDialogGameSaves::PreInit()
{
  m_saves.Clear();

  InitSaves();

  if (m_saves.Size() == 0)
  {
    CFileItemPtr item = std::make_shared<CFileItem>(g_localizeStrings.Get(231)); // "None"
    m_saves.Add(std::move(item));
  }

  //m_bHasDescription = false;
}

void CDialogGameSaves::InitSaves()
{
  if (m_gameSaveHandle)
  {
    //! @todo
    /*
    auto saves = m_gameSaveHandle->GetSaves(m_saves);
    for (const auto &save : saves)
    {
      CFileItemPtr item = std::make_shared<CFileItem>(g_localizeStrings.Get(scalingMethodProps.nameIndex));
      item->SetLabel2(g_localizeStrings.Get(scalingMethodProps.categoryIndex));
      item->SetProperty("game.videofilter", CVariant{ videoSettings.GetVideoFilter() });
      item->SetProperty("game.videofilterdescription", CVariant{ g_localizeStrings.Get(scalingMethodProps.descriptionIndex) });
      m_saves.Add(std::move(item));
      }
    }
    */
  }
}

void CDialogGameSaves::GetItems(CFileItemList &items)
{
  for (const auto &item : m_saves)
    items.Add(item);
}

void CDialogGameSaves::OnItemFocus(unsigned int index)
{
  if (static_cast<int>(index) < m_saves.Size())
  {
    /*! @todo
    CFileItemPtr item = m_saves[index];

    std::string videoFilter;
    std::string description;
    GetProperties(*item, videoFilter, description);

    CGameSettings &gameSettings = CMediaSettings::GetInstance().GetCurrentGameSettings();

    if (gameSettings.VideoFilter() != videoFilter)
    {
      gameSettings.SetVideoFilter(videoFilter);
      gameSettings.NotifyObservers(ObservableMessageSettingsChanged);

      OnDescriptionChange(description);
      m_bHasDescription = true;
    }
    else if (!m_bHasDescription)
    {
      OnDescriptionChange(description);
      m_bHasDescription = true;
    }
    */
  }
}

unsigned int CDialogGameSaves::GetFocusedItem() const
{
  /*! @todo
  CGameSettings &gameSettings = CMediaSettings::GetInstance().GetCurrentGameSettings();

  for (int i = 0; i < m_saves.Size(); i++)
  {
    std::string videoFilter;
    std::string description;
    GetProperties(*m_saves[i], videoFilter, description);

    if (videoFilter == gameSettings.VideoFilter())
    {
      return i;
    }
  }
  */

  return 0;
}

void CDialogGameSaves::PostExit()
{
  m_saves.Clear();
}

void CDialogGameSaves::RegisterDialog()
{
  //m_gameSaveHandle = CServiceBroker::GetGameRenderManager().RegisterSaveDialog(*this);
}

void CDialogGameSaves::UnregisterDialog()
{
  m_gameSaveHandle.reset();
}
