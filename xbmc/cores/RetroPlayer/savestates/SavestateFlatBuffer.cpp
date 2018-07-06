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

#include "SavestateFlatBuffer.h"

#include "savestate_generated.h"

using namespace KODI;
using namespace RETRO;

namespace
{
  /*!
   * \brief The initial size of the FlatBuffer's memory buffer
   *
   * 1024 is the default size in the FlatBuffers header. We might as well use
   * this until our size requirements are more known.
   */
  const size_t INITIAL_FLATBUFFER_SIZE = 1024;

  /*!
   * \brief Translate the save type (RetroPlayer to FlatBuffers)
   */
  SaveType TranslateType(SAVE_TYPE type)
  {
    switch (type)
    {
    case SAVE_TYPE::AUTO:
      return SaveType_Auto;
    case SAVE_TYPE::MANUAL:
      return SaveType_Manual;
    default:
      break;
    }

    return SaveType_Unknown;
  }

  /*!
   * \brief Translate the save type (FlatBuffers to RetroPlayer)
   */
  SAVE_TYPE TranslateType(SaveType type)
  {
    switch (type)
    {
    case SaveType_Auto:
      return SAVE_TYPE::AUTO;
    case SaveType_Manual:
      return SAVE_TYPE::MANUAL;
    default:
      break;
    }

    return SAVE_TYPE::UNKNOWN;
  }
}

CSavestateFlatBuffer::CSavestateFlatBuffer()
{
  Reset();
}

CSavestateFlatBuffer::~CSavestateFlatBuffer() = default;

void CSavestateFlatBuffer::Reset()
{
  m_builder.reset(new flatbuffers::FlatBufferBuilder(INITIAL_FLATBUFFER_SIZE));
  m_savestateBuilder.reset(new SavestateBuilder(*m_builder));
  m_data.clear();
  m_savestate = nullptr;
}

SAVE_TYPE CSavestateFlatBuffer::Type() const
{
  if (m_savestate)
    return TranslateType(m_savestate->type());

  return SAVE_TYPE::UNKNOWN;
}

void CSavestateFlatBuffer::SetType(SAVE_TYPE type)
{
  m_savestateBuilder->add_type(TranslateType(type));
}

uint8_t CSavestateFlatBuffer::Slot() const
{
  if (m_savestate)
    return m_savestate->slot();

  return 0;
}

void CSavestateFlatBuffer::SetSlot(uint8_t slot)
{
  m_savestateBuilder->add_slot(slot);
}

std::string CSavestateFlatBuffer::Label() const
{
  std::string label;

  if (m_savestate && m_savestate->label())
    label = m_savestate->label()->c_str();

  return label;
}

void CSavestateFlatBuffer::SetLabel(const std::string &label)
{
  auto labelOffset = m_builder->CreateString(label);
  m_savestateBuilder->add_label(labelOffset);
}

CDateTime CSavestateFlatBuffer::Created() const
{
  CDateTime created;

  if (m_savestate && m_savestate->created())
    created.SetFromRFC1123DateTime(m_savestate->created()->c_str());

  return created;
}

void CSavestateFlatBuffer::SetCreated(const CDateTime &created)
{
  auto createdOffset = m_builder->CreateString(created.GetAsRFC1123DateTime());
  m_savestateBuilder->add_created(createdOffset);
}

std::string CSavestateFlatBuffer::GameFileName() const
{
  std::string gameFileName;

  if (m_savestate && m_savestate->game_file_name())
    gameFileName = m_savestate->game_file_name()->c_str();

  return gameFileName;
}

void CSavestateFlatBuffer::SetGameFileName(const std::string &gameFileName)
{
  auto gameFileNameOffset = m_builder->CreateString(gameFileName);
  m_savestateBuilder->add_game_file_name(gameFileNameOffset);
}

uint64_t CSavestateFlatBuffer::TimestampFrames() const
{
  return m_savestate->timestamp_frames();
}

void CSavestateFlatBuffer::SetTimestampFrames(uint64_t timestampFrames)
{
  m_savestateBuilder->add_timestamp_frames(timestampFrames);
}

double CSavestateFlatBuffer::TimestampWallClock() const
{
  if (m_savestate)
    return static_cast<double>(m_savestate->timestamp_wall_clock_ns()) / 1000.0 / 1000.0 / 1000.0;

  return 0.0;
}

void CSavestateFlatBuffer::SetTimestampWallClock(double timestampWallClock)
{
  const uint64_t wallClockNs = static_cast<uint64_t>(timestampWallClock * 1000.0 * 1000.0 * 1000.0);
  m_savestateBuilder->add_timestamp_wall_clock_ns(wallClockNs);
}

std::string CSavestateFlatBuffer::GameClientID() const
{
  std::string gameClientId;

  if (m_savestate && m_savestate->emulator_addon_id())
    gameClientId = m_savestate->emulator_addon_id()->c_str();

  return gameClientId;
}

void CSavestateFlatBuffer::SetGameClientID(const std::string &gameClient)
{
  auto emulatorAddonIdOffset = m_builder->CreateString(gameClient);
  m_savestateBuilder->add_emulator_addon_id(emulatorAddonIdOffset);
}

std::string CSavestateFlatBuffer::GameClientVersion() const
{
  std::string gameClientVersion;

  if (m_savestate && m_savestate->emulator_version())
    gameClientVersion = m_savestate->emulator_version()->c_str();

  return gameClientVersion;
}

void CSavestateFlatBuffer::SetGameClientVersion(const std::string &gameClientVersion)
{
  auto emulatorVersionOffset = m_builder->CreateString(gameClientVersion);
  m_savestateBuilder->add_emulator_version(emulatorVersionOffset);
}

const uint8_t *CSavestateFlatBuffer::GetMemoryData() const
{
  if (m_savestate && m_savestate->memory_data())
    return m_savestate->memory_data()->data();

  return nullptr;
}

size_t CSavestateFlatBuffer::GetMemorySize() const
{
  if (m_savestate && m_savestate->memory_data())
    return m_savestate->memory_data()->size();

  return 0;
}

uint8_t *CSavestateFlatBuffer::GetMemoryBuffer(size_t size)
{
  uint8_t *memoryBuffer = nullptr;

  auto memoryDataOffset = m_builder->CreateUninitializedVector(size, &memoryBuffer);
  m_savestateBuilder->add_memory_data(memoryDataOffset);

  return memoryBuffer;
}

void CSavestateFlatBuffer::Finalize()
{
  auto savestate = m_savestateBuilder->Finish();
  FinishSavestateBuffer(*m_builder, savestate);

  m_savestate = GetSavestate(m_builder->GetBufferPointer());
}

bool CSavestateFlatBuffer::GetData(uint8_t *&data, size_t &size)
{
  // Check if savestate was deserialized from vector or built with FlatBuffers
  if (!m_data.empty())
  {
    data = m_data.data();
    size = m_data.size();
  }
  else
  {
    data = m_builder->GetBufferPointer();
    size = m_builder->GetSize();
  }

  return true;
}

bool CSavestateFlatBuffer::Deserialize(std::vector<uint8_t> data)
{
  flatbuffers::Verifier verifier(data.data(), data.size());
  if (VerifySavestateBuffer(verifier))
  {
    m_data = std::move(data);
    m_savestate = GetSavestate(m_data.data());
    return true;
  }

  return false;
}
