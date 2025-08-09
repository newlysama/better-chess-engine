/**
 * @file      user.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief User implementation.
 */

#include "user.h"

namespace server::user
{
    using namespace server::core;

    User::User(UserId id)
        : m_id(id)
    {
    }
} // namespace server::user