/**
 * @file      user_service.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief User service implementation.
 */

#include "user_service.h"

#include <unordered_map>

namespace server::user
{
    using namespace server::core;

    UserService::UserService() noexcept
        : m_usersMap{}
    {
    }

    std::expected<std::shared_ptr<User>, std::string> UserService::getUser(const Id id) const noexcept
    {
        UsersMap::const_accessor acc;
        if (m_usersMap.find(acc, id))
        {
            return acc->second;
        }

        return std::unexpected(std::format("User {} not found", id));
    }

    std::expected<void, std::string> UserService::addUser(const Id id) noexcept
    {
        UsersMap::accessor acc;
        if (m_usersMap.find(acc, id))
        {
            return std::unexpected(std::format("User {} already exists", id));
        }

        m_usersMap.insert(acc, {id, std::make_shared<User>(User{id})});
    }
} // namespace server::user