/*
 *  Copyright (C) 2017-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "input/ActionIDs.h"

class CAction;

namespace KODI
{
namespace RETRO
{
  class CGUIGameInput
  {
  public:
    CGUIGameInput();
    ~CGUIGameInput();

    bool OnAction(const CAction &action);
  };
}
}
