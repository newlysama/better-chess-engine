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
#include <unordered_map>
#include <vector>

namespace server::user
{
    class User
};

namespace server::core
{

    /**
     * @typedef UserId, RoomId
     * @brief   Wrapper around uint16_t for user and room ids.
     */
    typedef uint16_t UserId, RoomId;

    /**
     * @typedef UsersMap
     * @brief   Maps users through their id's.
     */
    typedef tbb::concurrent_hash_map<UserId, std::shared_ptr<server::user::User>> UsersMap;

    /**
     * @typedef RoomPlayers
     * @brief   1x2 vector holding the 2 room players.
     */
    typedef std::vector<std::weak_ptr<user::User>> RoomPlayers;

    /**
     * @typedef RoomSpectators
     * @brief   Map of spectating users of a room.
     */
    typedef std::unordered_map<UserId, std::weak_ptr<user::User>> RoomSpectators;
} // namespace server::core

#endif // SERVER_TYPES_H_