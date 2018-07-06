 /*
 *      Copyright (C) 2012-2017 Team Kodi
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

#include "SavestateDatabase.h"
#include "ISavestate.h"
#include "SavestateUtils.h"

using namespace KODI;
using namespace RETRO;

CSavestateDatabase::CSavestateDatabase() = default;

bool CSavestateDatabase::AddSavestate(const std::string &gamePath, const ISavestate& save)
{
  //return save.Serialize(CSavestateUtils::MakePath(gamePath));
  return false;
}

bool CSavestateDatabase::GetSavestate(const std::string& gamePath, ISavestate& save)
{
  //return save.Deserialize(CSavestateUtils::MakePath(gamePath));
  return false;
}

bool CSavestateDatabase::GetSavestatesNav(CFileItemList& items, const std::string& gamePath, const std::string& gameClient /* = "" */)
{
  //! @todo
  return false;
}

bool CSavestateDatabase::RenameSavestate(const std::string& path, const std::string& label)
{
  //! @todo
  return false;
}

bool CSavestateDatabase::DeleteSavestate(const std::string& path)
{
  //! @todo
  return false;
}

bool CSavestateDatabase::ClearSavestatesOfGame(const std::string& gamePath, const std::string& gameClient /* = "" */)
{
  //! @todo
  return false;
}
