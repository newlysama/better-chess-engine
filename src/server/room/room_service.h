/**
 * @file      room_service.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Room Service definition -> handles the rooms.
 */

#ifndef ROOM_SERVICE_H_
#define ROOM_SERVICE_H_

#include <expected>

#include "server/core/id_pool.h"
#include "server/core/types.h"
#include "server/room/room.h"
#include "server/snapshot/snapshots.h"

namespace server::room
{
    class RoomService
    {
      public:
        /**
         * @brief Constructor.
         */
        RoomService() noexcept;

        /**
         * @brief Default destructor.
         */
        ~RoomService() = default;

        /**
         * @brief Create a room and add it to the m_roomsMap.
         */
        void createRoom() noexcept;

        /**
         * @brief Close a given room.
         *
         * @param [in] roomId : id of the room to be closed
         */
        void closeRoom(const core::Id roomId) noexcept;

        /**
         * @brief Get a room if already presents in the map.
         *
         * @param [in] roomId : requested room's id
         *
         * @return The requested room if present, an error message if not
         */
        std::expected<std::shared_ptr<Room>, std::string> getRoom(const core::Id roomId) const noexcept;

        /**
         * @brief Add a user to a given room's players.
         *
         * @param [in] roomId : the given room
         * @param [in] userId : the given user
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> addPlayer(const core::Id roomId, const core::Id userId) noexcept;

        /**
         * @brief Add a user to a given room's spectators.
         *
         * @param [in] roomId : the given room
         * @param [in] userId : the given user
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> addSpectator(const core::Id roomId, const core::Id userId) noexcept;

        /**
         * @brief Remove a user from a given room's players.
         *
         * @param [in] roomId : the given room
         * @param [in] userId : the given user
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> removePlayer(const core::Id roomId, const core::Id userId) noexcept;

        /**
         * @brief Remove a user from a given room's spectators.
         *
         * @param [in] roomId : the given room
         * @param [in] userId : the given user
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> removeSpectator(const core::Id roomId, const core::Id userId) noexcept;

        /**
         * @brief Sends a MoveSnapshot to apply to a specific room.
         *
         * @param [in] roomId   : the room's id to send the snapshot to
         * @param [in] snapshot : the MoveSnapshot to send
         *
         * @return The new game state's snapshot if room exists, error message if not
         */
        std::expected<snapshot::GameSnapshot, std::string> makeMove(const core::Id roomId,
                                                                    const snapshot::MoveSnapshot& snapshot) noexcept;

      private:
        core::RoomsMap m_roomsMap; // Maps each room with its id
        core::IdPool m_idPool;     // Id's management pool
    };
} // namespace server::room

#endif // ROOM_SERVICE_H_