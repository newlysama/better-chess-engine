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
     * @typedef RoomPlayers
     * @brief   Pair holding the 2 room players ids.
     */
    typedef std::pair<Id, Id> RoomPlayers;

    /**
     * @typedef RoomSpectators
     * @brief   Map of spectating users of a room.
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