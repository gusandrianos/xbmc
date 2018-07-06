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

#include "SavestateWriter.h"
#include "SavestateUtils.h"
#include "filesystem/File.h"
#include "utils/log.h"
#include "URL.h"

using namespace KODI;
using namespace RETRO;

CSavestateWriter::~CSavestateWriter() = default;

bool CSavestateWriter::Initialize(const std::string &gamePath)
{
  bool bSuccess = false;

  if (!gamePath.empty())
  {
    m_savestatePath = CSavestateUtils::MakePath(gamePath);
    bSuccess = true;
  }

  return bSuccess;
}

bool CSavestateWriter::WriteSave(const uint8_t *data, size_t size)
{
  if (data == nullptr || size == 0)
    return false;

  CLog::Log(LOGDEBUG, "Saving savestate to %s", CURL::GetRedacted(m_savestatePath).c_str());

  bool bSuccess = false;

  XFILE::CFile file;
  if (file.OpenForWrite(m_savestatePath))
  {
    ssize_t written = file.Write(data, size);
    bSuccess = (written == static_cast<ssize_t>(size));
  }

  if (!bSuccess)
    CLog::Log(LOGERROR, "Failed to write savestate to %s", CURL::GetRedacted(m_savestatePath).c_str());

  return bSuccess;
}

void CSavestateWriter::WriteThumb()
{
  //! @todo
}
