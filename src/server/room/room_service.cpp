/**
 * @file      romm_service.h
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

    std::expected<std::shared_ptr<Room>, std::string> RoomService::getRoom(const RoomId id) const noexcept
    {
        RoomsMap::const_accessor acc;
        if (m_roomsMap.find(acc, id))
        {
            return acc->second;
        }

        return std::unexpected(std::format("Room {} not found", id));
    }

    std::expected<void, std::string> RoomService::addRoom(const RoomId id) noexcept
    {
        RoomsMap::accessor acc;
        if (m_roomsMap.find(acc, id))
        {
            return std::unexpected(std::format("Room {} already exists", id));
        }

        m_roomsMap.insert(acc, {id, std::make_shared<Room>(Room{id})});
        return std::expected<void, std::string>{};
    }

    std::expected<GameSnapshot, std::string> RoomService::makeMove(const RoomId id, const MoveSnapshot& snap) noexcept
    {
        RoomsMap::accessor acc;
        std::expected<std::shared_ptr<Room>, std::string> check = this->getRoom(id);

        if (check.has_value())
        {
            return check.value()->makeMove(snap);
        }

        return std::unexpected(check.error());
    }
} // namespace server::room