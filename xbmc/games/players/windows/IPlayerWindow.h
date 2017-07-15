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

/*!
 * \brief Player configuration window
 */

namespace KODI
{
  namespace GAME
  {
    class IVirtualPortPanel
    {
    public:
      virtual ~IVirtualPortPanel() = default;

      /*!
       * \brief The button to add a port has been pressed
       */
      virtual void OnAddPort() = 0;

      /*!
       * \brief The button to remove a port has been pressed
       */
      virtual void OnRemovePort() = 0;

      /*!
       * \brief A device has been selected
       *
       * \param column The column index of the device
       * \param row    The row index of the device
       */
      virtual void OnSelect(unsigned int column, unsigned int row) = 0;

      /*!
       * \brief Check if a device exists at the specified location
       *
       * \param column The column to check
       * \param row The row to check
       *
       * \return True if the location has a device occupying it
       */
      virtual bool HasDevice(unsigned int column, unsigned int row) = 0;
    };

    /*!
     * \brief A panel for players and spectators
     */
    class IPlayerPanel
    {
    public:
      virtual ~IPlayerPanel() = default;

      /*!
       * \brief A player (or empty slot) has been selected
       *
       * \param index The index of the player or empty slot
       */
      virtual void OnSelect(unsigned int index) = 0;

      /*!
       * \brief Check the number of player slots in this panel
       *
       * This can grow or shrink depending on how many ports are available.
       *
       * \return The number of player spots, including empty spots
       */
      virtual unsigned int PlayerSlotCount() const = 0;
    };
  }
}
