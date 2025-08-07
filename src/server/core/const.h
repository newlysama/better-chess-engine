/**
 * @file      const.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Provides various constants.
 */

#ifndef SERVER_CONST_H_
#define SERVER_CONST_H_

#include <string_view>

namespace server::core
{
    // Available characters for the joining code
    inline constexpr std::string_view JOINING_CODE_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    // Joining code size
    inline constexpr int JOINING_CODE_SIZE = 6;

} // namespace server::core

#endif // SERVER_CONST_H_