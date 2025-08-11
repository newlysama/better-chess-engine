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
        , m_game{}
        , m_players{}
        , m_spectators{}
    {
        this->createJoiningCode();
        m_players.reserve(2);
    }

    void Room::createJoiningCode() noexcept
    {
        std::string out(JOINING_CODE_SIZE, '?');
        std::random_device rd;
        std::uniform_int_distribution<int> dist(0, JOINING_CODE_ALPHABET.size() - 1);

        for (int i = 0; i < JOINING_CODE_SIZE; i++)
        {
            out[i] = JOINING_CODE_ALPHABET[dist(rd)];
        }

        m_joinCode = std::string_view{std::move(out.data())};
    }

    bool Room::checkPlayers() noexcept
    {
        bool deleted = false;
        for (int i = 0; i < 2; i++)
        {
            if (m_players[i].lock() == nullptr)
            {
                // Free the slot
                m_players[i].reset();
                deleted = true;

                LOG_INFO("Found disconnected user in room {}'s players, freeing the slot", m_id);
            }
        }

        return deleted;
    }

    bool Room::checkSpectators() noexcept
    {
        bool deleted = false;
        for (auto it = m_spectators.begin(); it != m_spectators.end(); it++)
        {
            if (it->second.lock() == nullptr)
            {
                // Free the slot
                m_spectators.erase(it);
                deleted = true;

                LOG_INFO("Found disconnected user in room {}'s spectators, freeing the slot", m_id);
            }
        }

        return deleted;
    }

    bool Room::playersContains(const std::shared_ptr<user::User>& user) noexcept
    {
        for (int i = 0; i < 2; i++)
        {
            std::shared_ptr<User> player = m_players[i].lock();

            if (player == user)
            {
                return true;
            }
        }

        return false;
    }

    std::expected<void, std::string> Room::addPlayer(const std::shared_ptr<User>& user) noexcept
    {
        // Room is full
        if (m_players.size() == 2)
        {
            std::shared_ptr<User> player1 = m_players[0].lock();
            std::shared_ptr<User> player2 = m_players[1].lock();

            return std::unexpected(
                std::format("Cannot add user {} to room {}'s players : room is full ({} and {} already present)",
                            user->m_id, m_id, player1->m_id, player2->m_id));
        }
        // User already present
        else if (playersContains(user))
        {
            return std::unexpected(
                std::format("Cannot add user {} to room {}'s players : already present", user->m_id, m_id));
        }

        // Use emplace since user is shared_ptr, not already weak_ptr
        m_players.emplace_back(user);
        LOG_INFO("Added user {} to room {}'s players", user->m_id, m_id);

        return std::expected<void, std::string>{};
    }

    std::expected<void, std::string> Room::addSpectator(const std::shared_ptr<user::User>& user) noexcept
    {
        // User already present
        if (m_spectators.contains(user->m_id))
        {
            return std::unexpected(
                std::format("Cannot add user {} to room {}'s spectators : already present", user->m_id, m_id));
        }

        // Use emplace since user is shared_ptr, not already weak_ptr
        m_spectators.emplace(user->m_id, user);
        LOG_INFO("Added user {} to room {}'s spectators", user->m_id, m_id);

        return std::expected<void, std::string>{};
    }

    std::expected<void, std::string> Room::removePlayer(const std::shared_ptr<user::User>& user) noexcept
    {
        for (int i = 0; i < 2; i++)
        {
            if (m_players[i].lock() == user)
            {
                m_players[i].reset();
                LOG_INFO("Removed user {} from room {}'s players", user->m_id, m_id);

                return std::expected<void, std::string>{};
            }
        }

        return std::unexpected(std::format("Cannot remove user {} to room {}'s players : not found", user->m_id, m_id));
    }

    std::expected<void, std::string> Room::removeSpectator(const std::shared_ptr<user::User>& user) noexcept
    {
        for (auto it = m_spectators.begin(); it != m_spectators.end(); it++)
        {
            if (it->second.lock() == user)
            {
                m_spectators.erase(it);
                LOG_INFO("Removed user {} from room {}'s players", user->m_id, m_id);

                return std::expected<void, std::string>{};
            }
        }

        return std::unexpected(std::format("Cannot remove user {} to room {}'s players : not found", user->m_id, m_id));
    }

    GameSnapshot Room::makeMove(const MoveSnapshot& moveSnapshot) noexcept
    {
        const Move& move = m_game.m_moveList.find(moveSnapshot.fromSquare, moveSnapshot.toSquare);

        m_game.makeMove<false>(move);

        GameSnapshot snapshot = gameToGameSnapshot(m_game);
        snapshot.roomId = m_id;
        snapshot.lastMove = moveSnapshot;

        return snapshot;
    }
} // namespace server::room