/**
 * @file      types.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Provides various types.
 */

#ifndef SERVER_TYPES_H_
#define SERVER_TYPES_H_

#include <memory>
#include <tbb/concurrent_hash_map.h>
#include <utility>
#include <vector>

#include "engine/core/enums.h"

namespace server::user
{
    class User;
}

namespace server::room
{
    class Room;
}

namespace server::core
{

    /**
     * @typedef Id
     * @brief   Wrapper around uint16_t for ids.
     */
    typedef uint16_t Id;

    /**
     * @typedef RoomPlayer
     * @brief   Struct representing a room player, holding its team and user Id.
     */
    typedef struct RoomPlayer
    {
        Id userId;
        engine::core::Color color;

        RoomPlayer(const Id id, const engine::core::Color color) noexcept
            : userId(id)
            , color(color)
        {
        }

        ~RoomPlayer() = default;
    } RoomPlayer;

    /**
     * @typedef RoomPlayers
     * @brief   Pair holding the 2 room players.
     */
    typedef std::pair<RoomPlayer, RoomPlayer> RoomPlayers;

    /**
     * @typedef RoomSpectators
     * @brief   Vector of spectating users of a room.
     */
    typedef std::vector<Id> RoomSpectators;

    /**
     * @typedef UsersMap
     * @brief   Maps users through their id's.
     */
    typedef tbb::concurrent_hash_map<Id, std::shared_ptr<server::user::User>> UsersMap;

    /**
     * @typedef RoomsMap
     * @brief   Maps rooms through their id's.
     */
    typedef tbb::concurrent_hash_map<Id, std::shared_ptr<server::room::Room>> RoomsMap;
} // namespace server::core

#endif // SERVER_TYPES_H_