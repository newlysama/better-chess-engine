/**
 * @file      room.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Room implementation.
 */

#include "room.h"

#include <random>

#include "logging/logging.h"
#include "server/core/const.h"
#include "server/snapshot/snapshot_handler.h"

namespace server::room
{
    using namespace engine::game;
    using namespace engine::board;
    using namespace engine::core;
    using namespace server::core;
    using namespace server::user;
    using namespace server::snapshot;

    Room::Room(Id id) noexcept
        : m_id(id)
        , m_taskArena(tbb::task_arena::constraints(1)) // Max concurrency : 1
        , m_game{}
        , m_players{RoomPlayer{NULL_ID, Color::WHITE}, RoomPlayer{NULL_ID, Color::WHITE}}
        , m_spectators{}
    {
        this->createJoiningCode();
        m_spectators.reserve(10);
    }

    void Room::createJoiningCode() noexcept
    {
        static thread_local std::mt19937 gen{std::random_device{}()};
        std::uniform_int_distribution<int> dist(0, static_cast<int>(JOINING_CODE_ALPHABET.size()) - 1);

        std::string out(JOINING_CODE_SIZE, '?');

        for (int i = 0; i < JOINING_CODE_SIZE; ++i)
            out[i] = JOINING_CODE_ALPHABET[dist(gen)];

        m_joinCode = std::move(out);
    }

    bool Room::playersContains(const Id userId) const noexcept
    {
        return m_players.first.userId == userId || m_players.second.userId == userId;
    }

    bool Room::spectatorContains(const Id userId) const noexcept
    {
        auto it = std::find(m_spectators.begin(), m_spectators.end(), userId);

        if (it != m_spectators.end())
            return true;

        return false;
    }

    std::expected<void, std::string> Room::addPlayer(const Id userId) noexcept
    {
        return serial([&] -> std::expected<void, std::string> {
            // User already present
            if (this->playersContains(userId))
            {
                return std::unexpected(
                    std::format("Cannot add user {} to room {}'s players : already present", userId, m_id));
            }
            // Room is full
            else if (m_players.first.userId != NULL_ID && m_players.second.userId != NULL_ID)
            {
                return std::unexpected(
                    std::format("Cannot add user {} to room {}'s players : room is full ({} and {} already present)",
                                userId, m_id, m_players.first.userId, m_players.second.userId));
            }

            // Player 1 is set
            if (m_players.first.userId != NULL_ID)
            {
                m_players.second.userId = userId;
            }
            else
            {
                m_players.first.userId = userId;
            }

            LOG_INFO("Added user {} to room {}'s players", userId, m_id);

            return std::expected<void, std::string>{};
        });
    }

    std::expected<void, std::string> Room::addSpectator(const Id userId) noexcept
    {
        return serial([&] -> std::expected<void, std::string> {
            // User already present
            if (this->spectatorContains(userId))
            {
                return std::unexpected(
                    std::format("Cannot add user {} to room {}'s spectators : already present", userId, m_id));
            }

            m_spectators.push_back(userId);
            LOG_INFO("Added user {} to room {}'s spectators", userId, m_id);

            return std::expected<void, std::string>{};
        });
    }

    std::expected<void, std::string> Room::removePlayer(const Id userId) noexcept
    {
        return serial([&] -> std::expected<void, std::string> {
            if (m_players.first.userId == userId)
            {
                LOG_INFO("Removed user {} from room {}'s players", userId, m_id);
                return std::expected<void, std::string>{};
            }
            else if (m_players.second.userId == userId)
            {
                LOG_INFO("Removed user {} from room {}'s players", userId, m_id);
                return std::expected<void, std::string>{};
            }
            else
            {
                return std::unexpected(
                    std::format("Cannot remove user {} from room {}'s players : not found", userId, m_id));
            }
        });
    }

    std::expected<void, std::string> Room::removeSpectator(const Id userId) noexcept
    {
        return serial([&] -> std::expected<void, std::string> {
            auto it = std::find(m_spectators.begin(), m_spectators.end(), userId);

            if (it != m_spectators.end())
            {
                *it = m_spectators.back();
                m_spectators.pop_back();

                LOG_INFO("Removed user {} from room {}'s spectators", userId, m_id);
                return std::expected<void, std::string>{};
            }
            else
            {
                return std::unexpected(
                    std::format("Cannot remove user {} from room {}'s spectators : not found", userId, m_id));
            }
        });
    }

    std::expected<GameSnapshot, std::string> Room::makeMove(const MoveSnapshot& moveSnapshot) noexcept
    {
        return serial([&] -> std::expected<GameSnapshot, std::string> {
            if (!this->playersContains(moveSnapshot.userId))
            {
                return std::unexpected(
                    std::format("Received a move request from a user than is not a player : {} (players : {} - {})",
                                moveSnapshot.userId, m_players.first.userId, m_players.second.userId));
            }

            RoomPlayer player = m_players.first.id == moveSnapshot.userId ? m_players.first : m_playres.second;

            if (moveSnapshot.color != m_game.m_state.m_sideToMove)
            {
                return std::unexpected(std::format(
                    "Received a moveSnapshot from the wrong team. User {} with color {} - Side to move is {}",
                    player.userId, player.color, utils::toString(moveSnapshot.color)));
            }

            const Move& move = m_game.m_moveList.find(moveSnapshot.fromSquare, moveSnapshot.toSquare);

            m_game.makeMove<false>(move);
            m_game.m_moveList.generateAllMoves(m_game.m_state);

            GameSnapshot snapshot = gameToGameSnapshot(m_game);
            snapshot.roomId = m_id;
            snapshot.lastMove = moveSnapshot;

            return snapshot;
        });
    }
} // namespace server::room