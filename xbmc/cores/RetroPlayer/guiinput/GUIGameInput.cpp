/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GUIGameInput.h"
#include "input/Action.h"
#include "input/ActionIDs.h"

using namespace KODI;
using namespace RETRO;

CGUIGameInput::CGUIGameInput()
{
  //! @todo Register input
}

CGUIGameInput::~CGUIGameInput()
{
  //! @todo Unregister input
}

bool CGUIGameInput::OnAction(const CAction &action)
{
  switch (action.GetID())
  {
    case ACTION_MOVE_LEFT:
    {
      // Left
      break;
    }
    case ACTION_MOVE_RIGHT:
    {
      // Right
      break;
    }
    case ACTION_MOVE_UP:
    {
      // Up
      break;
    }
    case ACTION_MOVE_DOWN:
    {
      // Down
      break;
    }
    case ACTION_SELECT_ITEM:
    {
      // Start
      break;
    }
    default:
      break;
  }

  return false;
}
