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

#include "ControllerTopology.h"
#include "ControllerDefinitions.h"
#include "utils/log.h"
#include "utils/XMLUtils.h"

#include <algorithm>

using namespace KODI;
using namespace GAME;

// --- CControllerPort ---------------------------------------------------------

void CControllerPort::Reset(void)
{
  CControllerPort defaultPort;
  *this = std::move(defaultPort);
}

bool CControllerPort::IsCompatible(const std::string &controllerId, const std::string &modelName /* = "" */) const
{
  auto IsCompatible = [&controllerId, &modelName](const ControllerID &controller)
    {
      if (controller.controllerId != controllerId)
        return false;

      if (!controller.modelName.empty() && controller.modelName != modelName)
        return false;

      return true;
    };

  return std::find_if(m_accepts.begin(), m_accepts.end(), IsCompatible) != m_accepts.end();
}

bool CControllerPort::Deserialize(const TiXmlElement* pElement)
{
  Reset();

  if (pElement == nullptr)
    return false;

  m_portId = XMLUtils::GetAttribute(pElement, LAYOUT_XML_ATTR_PORT_ID);

  for (const TiXmlElement* pChild = pElement->FirstChildElement(); pChild != nullptr; pChild = pChild->NextSiblingElement())
  {
    if (pChild->ValueStr() == LAYOUT_XML_ELM_ACCEPTS)
    {
      std::string controller = XMLUtils::GetAttribute(pChild, LAYOUT_XML_ATTR_CONTROLLER);
      std::string model = XMLUtils::GetAttribute(pChild, LAYOUT_XML_ATTR_MODEL);

      if (!controller.empty())
        m_accepts.emplace_back(ControllerID{ std::move(controller), std::move(model) });
      else
        CLog::Log(LOGWARNING, "<%s> tag is missing \"%s\" attribute", LAYOUT_XML_ELM_ACCEPTS, LAYOUT_XML_ATTR_CONTROLLER);
    }
    else
    {
      CLog::Log(LOGDEBUG, "Unknown physical topology port tag: <%s>", pChild->ValueStr().c_str());
    }
  }

  return true;
}

// --- CControllerTopology -----------------------------------------------------

void CControllerTopology::Reset()
{
  CControllerTopology defaultTopology;
  *this = std::move(defaultTopology);
}

bool CControllerTopology::Deserialize(const TiXmlElement* pElement)
{
  Reset();

  if (pElement == nullptr)
    return false;

  m_bHasPlayer = (XMLUtils::GetAttribute(pElement, LAYOUT_XML_ATTR_HAS_PLAYER) != "false");

  for (const TiXmlElement* pChild = pElement->FirstChildElement(); pChild != nullptr; pChild = pChild->NextSiblingElement())
  {
    if (pChild->ValueStr() == LAYOUT_XML_ELM_PORT)
    {
      CControllerPort port;
      if (port.Deserialize(pChild))
        m_ports.emplace_back(std::move(port));
    }
    else
    {
      CLog::Log(LOGDEBUG, "Unknown physical topology tag: <%s>", pChild->ValueStr().c_str());
    }
  }

  return true;
}
