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

#include "server/core/id_pool.h"
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
         * @brief Create a user and add it to the m_usersMap.
         */
        void createUser() noexcept;

        /**
         * @brief Delete a user.
         *
         * @param [in] id : id of the user to be deleted
         */
        void deleteUser(core::Id id) noexcept;

      private:
        core::UsersMap m_usersMap; // Map each user with its id
        core::IdPool m_idPool;     // Ids management pool

        /**
         * @brief Get a user if already presents in the map.
         *
         * @param [in] id : requested user's id
         *
         * @return The requested user if present, an error message if not
         */
        std::expected<std::shared_ptr<User>, std::string> getUser(const core::Id id) const noexcept;
    };
} // namespace server::user

#endif // USER_SERVICE_H_