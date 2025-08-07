/**
 * @file      user.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief User definition.
 */

#ifndef USER_H_
#define USER_H_

#include <cstdint>

#include "server/core/types.h"

namespace server::user
{
    class User
    {
      public:
        explicit User(core::UserId id);

        ~User() = default;

        bool operator==(const User& other)
        {
            return m_id == other.m_id;
        }

        core::UserId m_id; // Id of the user
    };
} // namespace server::user

#endif // USER_H_