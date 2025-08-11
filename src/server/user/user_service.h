/**
 * @file      user_service.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief User service definition : handles the users.
 */

#ifndef USER_SERVICE_H_
#define USER_SERVICE_H_

#include <expected>

#include "server/core/types.h"
#include "server/user/user.h"

namespace server::user
{
    class UserService
    {
      public:
        /**
         * @brief Constructor.
         */
        explicit UserService() noexcept;

        /**
         * @brief Default destructor.
         */
        ~UserService() = default;

        /**
         * @brief Get a user if already presents in the map.
         *
         * @param [in] id : requested user's id
         *
         * @return The requested user if present, an error message if not
         */
        std::expected<std::shared_ptr<User>, std::string> getUser(const core::UserId id) const noexcept;

        /**
         * @brief Add a user the m_usersMap.
         *
         * @param [in] id : new user id
         *
         * @return Noting if insertion was successful, an error message if not
         */
        std::expected<void, std::string> addUser(const core::UserId id) noexcept;

      private:
        core::UsersMap m_usersMap; // Map each user with its id
    };
} // namespace server::user

#endif // USER_SERVICE_H_