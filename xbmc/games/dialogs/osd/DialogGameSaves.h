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

#include "DialogGameVideoSelect.h"
#include "FileItem.h"

#include <memory>

namespace KODI
{
namespace RETRO
{
  class CGUIGameSaveHandle;
}

namespace GAME
{
  class CDialogGameSaves : public CDialogGameVideoSelect
  {
  public:
    CDialogGameSaves(void);
    virtual ~CDialogGameSaves(void) = default;

    // implementation of CGUIControl via CDialogGameVideoSelect
    virtual bool OnMessage(CGUIMessage& message) override;

  protected:
    // implementation of CDialogGameVideoSelect
    std::string GetHeading() override;
    void PreInit() override;
    void GetItems(CFileItemList &items) override;
    void OnItemFocus(unsigned int index) override;
    unsigned int GetFocusedItem() const override;
    void PostExit() override;

  private:
    void InitSaves();

    void RegisterDialog();
    void UnregisterDialog();

    std::shared_ptr<RETRO::CGUIGameSaveHandle> m_gameSaveHandle;

    CFileItemList m_saves;
  };
}
}
