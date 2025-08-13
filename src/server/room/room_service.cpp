/**
 * @file      room_service.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Room Service implementation.
 */

#include "room_service.h"

namespace server::room
{
    using namespace server::core;
    using namespace server::snapshot;

    RoomService::RoomService() noexcept
        : m_roomsMap{}
    {
    }

    void RoomService::createRoom() noexcept
    {
        while (true)
        {
            Id roomId = m_idPool.acquire();

            RoomsMap::accessor acc;
            if (m_roomsMap.insert(acc, {roomId, std::make_shared<Room>(Room{roomId})}))
            {
                // Sucess
                return;
            }

            // Rare case of collision
            // Retry until sucess
            m_idPool.release(roomId);
        }
    }

    void RoomService::closeRoom(const Id roomId) noexcept
    {
        RoomsMap::accessor acc;
        if (m_roomsMap.find(acc, roomId))
        {
            m_roomsMap.erase(acc);
            m_idPool.release(roomId);
        }
    }

    std::expected<std::shared_ptr<Room>, std::string> RoomService::getRoom(const Id roomId) const noexcept
    {
        RoomsMap::const_accessor acc;
        if (m_roomsMap.find(acc, roomId))
        {
            return acc->second;
        }

        return std::unexpected(std::format("Room {} not found", roomId));
    }

    std::expected<void, std::string> RoomService::addPlayer(const core::Id roomId, const core::Id userId) noexcept
    {
        auto checkRoom = this->getRoom(roomId);

        return checkRoom.has_value() ? checkRoom.value()->addPlayer(userId) : std::unexpected(checkRoom.error());
    }

    std::expected<void, std::string> RoomService::addSpectator(const core::Id roomId, const core::Id userId) noexcept
    {
        auto checkRoom = this->getRoom(roomId);

        return checkRoom.has_value() ? checkRoom.value()->addSpectator(userId) : std::unexpected(checkRoom.error());
    }

    std::expected<void, std::string> RoomService::removePlayer(const core::Id roomId, const core::Id userId) noexcept
    {
        auto checkRoom = this->getRoom(roomId);

        return checkRoom.has_value() ? checkRoom.value()->removePlayer(userId) : std::unexpected(checkRoom.error());
    }

    std::expected<void, std::string> RoomService::removeSpectator(const core::Id roomId, const core::Id userId) noexcept
    {
        auto checkRoom = this->getRoom(roomId);

        return checkRoom.has_value() ? checkRoom.value()->removeSpectator(userId) : std::unexpected(checkRoom.error());
    }

    std::expected<GameSnapshot, std::string> RoomService::makeMove(const Id roomId, const MoveSnapshot& snap) noexcept
    {
        auto checkRoom = this->getRoom(roomId);

        return checkRoom.has_value() ? checkRoom.value()->makeMove(snap) : std::unexpected(checkRoom.error());
    }
} // namespace server::room