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

    std::expected<GameSnapshot, std::string> RoomService::makeMove(const Id roomId, const MoveSnapshot& snap) noexcept
    {
        RoomsMap::accessor acc;
        auto checkRoom = this->getRoom(roomId);

        if (checkRoom.has_value())
        {
            auto checkGameSnapshot = checkRoom.value()->makeMove(snap);

            if (checkGameSnapshot.has_value())
            {
                return checkGameSnapshot.value();
            }

            return std::unexpected(checkGameSnapshot.error());
        }

        return std::unexpected(checkRoom.error());
    }
} // namespace server::room