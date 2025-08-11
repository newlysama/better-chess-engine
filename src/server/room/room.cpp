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

    bool Room::checkPlayers() noexcept
    {
        // Reset a slots if a player's ptr has expired
        auto clearIfExpired = [](std::weak_ptr<User>& player) -> bool {
            if (player.expired())
            {
                player.reset();
                return true;
            }

            return false;
        };

        return clearIfExpired(m_players.first) || clearIfExpired(m_players.second);
    }

    bool Room::checkSpectators() noexcept
    {
        auto erased = std::erase_if(m_spectators, [](auto& spectator) { return spectator.second.expired(); });

        if (erased > 0)
        {
            LOG_INFO("Found {} disconnected spectator(s) in room {}, freeing", erased, m_id);
            return true;
        }

        return false;
    }

    bool Room::playersContains(const std::shared_ptr<user::User>& user) const noexcept
    {
        return m_players.first.lock() == user || m_players.second.lock() == user;
    }

    std::expected<void, std::string> Room::addPlayer(const std::shared_ptr<User>& user) noexcept
    {
        // User already present
        if (playersContains(user))
        {
            return std::unexpected(
                std::format("Cannot add user {} to room {}'s players : already present", user->m_id, m_id));
        }
        // Room is full
        else if (!m_players.first.expired() && !m_players.second.expired())
        {
            std::shared_ptr<User> player1 = m_players.first.lock();
            std::shared_ptr<User> player2 = m_players.second.lock();

            return std::unexpected(
                std::format("Cannot add user {} to room {}'s players : room is full ({} and {} already present)",
                            user->m_id, m_id, player1->m_id, player2->m_id));
        }

        // Player 1 is set
        if (!m_players.first.expired())
        {
            m_players.second = user;
        }
        else
        {
            m_players.first = user;
        }

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
        auto clearIfMatch = [&](std::weak_ptr<User>& player) -> bool {
            if (player.lock() == user)
            {
                player.reset();
                return true;
            }

            return false;
        };

        if (clearIfMatch(m_players.first) || clearIfMatch(m_players.second))
        {
            LOG_INFO("Removed user {} from room {}'s players", user->m_id, m_id);
            return std::expected<void, std::string>{};
        }

        return std::unexpected(
            std::format("Cannot remove user {} from room {}'s players : not found", user->m_id, m_id));
    }

    std::expected<void, std::string> Room::removeSpectator(const std::shared_ptr<user::User>& user) noexcept
    {
        const auto erased = std::erase_if(m_spectators, [&](auto& pair) -> int {
            auto spectator = pair.second.lock();

            return spectator && spectator == user;
        });

        if (erased > 0)
        {
            LOG_INFO("Removed user {} from room {}'s spectators", user->m_id, m_id);
            return {};
        }
        return std::unexpected(
            std::format("Cannot remove user {} from room {}'s spectators : not found", user->m_id, m_id));
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