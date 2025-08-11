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
#include "server/snapshot/snapshots.h"
#include "server/user/user.h"

namespace server::room
{
    class Room
    {
      public:
        /**
         * @brief Constructor.
         */
        explicit Room(core::Id id) noexcept;

        /**
         * @brief Default destructor.
         */
        ~Room() = default;

        /**
         * @brief Generates a random joining code.
         */
        void createJoiningCode() noexcept;

        /**
         * @brief Look for expired pointers in m_players, and clear them.
         *
         * @return True if found some, false otherwise.
         */
        bool checkPlayers() noexcept;

        /**
         * @brief Look for expired pointers in m_spectators, and clear them.
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
        bool playersContains(const std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Add a user to room's players.
         *
         * @param [in] user : the user to add
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> addPlayer(const std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Add a user to room's spectators.
         *
         * @param [in] user : the user to add
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> addSpectator(const std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Remove a user from room's players.
         *
         * @param [in] user : the user to remove
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> removePlayer(const std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Remove a user from room's spectators.
         *
         * @param [in] user : the user to remove
         *
         * @return Nothing if succesfull, error message user if not
         */
        std::expected<void, std::string> removeSpectator(const std::shared_ptr<user::User>& user) noexcept;

        /**
         * @brief Apply a received MoveSnapshot to the game state.
         *
         * @param [in] moveSnapshot : The move to apply
         *
         * @return The GameSnapshot representing the new state of the game
         */
        snapshot::GameSnapshot makeMove(const snapshot::MoveSnapshot& moveSnapshot) noexcept;

      private:
        core::Id m_id;                     // Id of the Room
        std::string_view m_joinCode;       // Code to join the room
        engine::game::Game m_game;         // Instance of the engine
        core::RoomPlayers m_players;       // Vector of the 2 players
        core::RoomSpectators m_spectators; // Map of the spectators
    };
} // namespace server::room

#endif // ROOM_H_