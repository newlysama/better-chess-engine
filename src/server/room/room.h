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
#include <tbb/task_arena.h>

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
         * @brief Add a user to room's players.
         *
         * @param [in] userId : the user to add
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> addPlayer(const core::Id userId) noexcept;

        /**
         * @brief Add a user to room's spectators.
         *
         * @param [in] userId : the user to add
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> addSpectator(const core::Id userId) noexcept;

        /**
         * @brief Remove a user from room's players.
         *
         * @param [in] userId : the user to remove
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> removePlayer(const core::Id userId) noexcept;

        /**
         * @brief Remove a user from room's spectators.
         *
         * @param [in] userId : the user to remove
         *
         * @return Nothing if succesfull, error message if not
         */
        std::expected<void, std::string> removeSpectator(const core::Id userId) noexcept;

        /**
         * @brief Apply a received MoveSnapshot to the game state.
         *
         * @param [in] moveSnapshot : The move to apply
         *
         * @return The GameSnapshot representing the new state of the game
         */
        std::expected<snapshot::GameSnapshot, std::string> makeMove(
            const snapshot::MoveSnapshot& moveSnapshot) noexcept;

      private:
        /**
         * @brief Executes a function within m_taskArena.
         *
         * @param [in] func : the function to execute
         */
        template <class F>
        decltype(auto) serial(F&& func)
        {
            return m_taskArena.execute([&] { return std::forward<F>(func)(); });
        }

        /**
         * @brief Generates a random joining code.
         */
        void createJoiningCode() noexcept;

        /**
         * @brief Check if a user is present in m_players.
         *
         * @param [in] userId : user to look for
         *
         * @return true if present, false otherwise
         */
        bool playersContains(const core::Id userId) const noexcept;

        /**
         * @brief Check if a user is present in m_spectators.
         *
         * @param [in] userId : user to look for
         *
         * @return true if present, false otherwise
         */
        bool spectatorContains(const core::Id userId) const noexcept;

        core::Id m_id;          // Id of the Room
        std::string m_joinCode; // Code to join the room

      private:
        tbb::task_arena m_taskArena;       // Execution arena (no dedicated thread)
        engine::game::Game m_game;         // Instance of the engine
        core::RoomPlayers m_players;       // Vector of the 2 players
        core::RoomSpectators m_spectators; // Map of the spectators
    };
} // namespace server::room

#endif // ROOM_H_