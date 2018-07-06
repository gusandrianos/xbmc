/*
 *      Copyright (C) 2016-2017 Team Kodi
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

#include "SavestateReader.h"
#include "cores/RetroPlayer/environment/Environment.h"
#include "filesystem/File.h"
#include "utils/log.h"
#include "URL.h"

using namespace KODI;
using namespace RETRO;

std::vector<uint8_t> CSavestateReader::ReadSave(const std::string &savestatePath)
{
  std::vector<uint8_t> data;

  CLog::Log(LOGDEBUG, "Loading savestate from %s", CURL::GetRedacted(savestatePath).c_str());

  XFILE::CFile savestateFile;
  if (savestateFile.Open(savestatePath, XFILE::READ_TRUNCATED))
  {
    int64_t size = savestateFile.GetLength();
    if (size > 0)
    {
      data.resize(static_cast<size_t>(size));
      if (savestateFile.Read(data.data(), data.size()) != static_cast<ssize_t>(data.size()))
      {
        CLog::Log(LOGERROR, "Failed to read savestate %s of size %d bytes",
          CURL::GetRedacted(savestatePath).c_str(),
          size);
        data.clear();
      }
    }
    else
      CLog::Log(LOGERROR, "Failed to get savestate length: %s", CURL::GetRedacted(savestatePath).c_str());
  }
  else
    CLog::Log(LOGERROR, "Failed to open savestate file %s", CURL::GetRedacted(savestatePath).c_str());

  return data;
}

std::string CSavestateReader::LoadGameClient(const std::string &savestatePath)
{
  std::vector<uint8_t> data = ReadSave(savestatePath);

  if (!data.empty())
    return CEnvironment::GetGameClient(data);

  return "";
}
