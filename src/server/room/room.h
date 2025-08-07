/**
 * @file      room.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Room definition.
 */

#ifndef ROOM_H_
#define ROOM_H_

#include <expected>
#include <memory>

#include "engine/game/game.h"
#include "server/core/types.h"
#include "server/user/user.h"

namespace server::room
{
    class Room
    {
      public:
        /**
         * @brief Constructor.
         */
        explicit Room(core::RoomId id) noexcept;

        /**
         * @brief Default destructor.
         */
        ~Room() = default;

        /**
         * @brief Generates a random joining code.
         */
        void createJoiningCode() noexcept;

        /**
         * @brief Look for expired pointers in m_players.
         *
         * @return True if found some, false otherwise.
         */
        bool checkPlayers() noexcept;

        /**
         * @brief Look for expired pointers in m_spectators.
         *
         * @return True if found some, false otherwise.
         */
        bool checkSpectators() noexcept;

        /**
         * @brief Check if a user is present in m_players.
         *
         * @param [in] user : user to look for
         *
         * @return true if present, false otherwise
         */
        bool playersContains(std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Add a user to room's players.
         *
         * @param [in] user : the user to add
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> addPlayer(std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Add a yser to room's spectators.
         *
         * @param [in] user : the user to add
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> addSpectator(std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Remove a user from room's players.
         *
         * @param [in] user : the user to remove
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> removePlayer(std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Remove a user from room's spectators.
         *
         * @param [in] user : the user to remove
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> removeSpectator(std::shared_ptr<user::User>& user) noexcept;

      private:
        core::RoomId m_id;                 // Id of the Room
        std::string_view m_joinCode;       // Code to join the room
        engine::game::Game& m_game;        // Instance of the engine
        core::RoomPlayers m_players;       // Vector of the 2 players
        core::RoomSpectators m_spectators; // Map of the spectators
    }
} // namespace server::room

#endif // ROOM_H_