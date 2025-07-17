/**
 * @file      utils.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Utility functions.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <algorithm>
#include <string_view>

#include "engine/core/const.h"
#include "logging/logging.h"

/**
 * @namespace utils
 */
namespace utils
{
    /**
     * @brief Retreives the rank/file string notation given a square index.
     *
     * @param [in] squareIndex : the index of the square within a Bitboard
     * @return std::string_view : rank/file string notation or "Unkown" if squareIndex is not valid
     */
    inline std::string_view squareIndexToString(const int squareIndex)
    {
        auto iter = std::find_if(std::begin(engine::core::SQUARE_INDEX), std::end(engine::core::SQUARE_INDEX),
                                 [&squareIndex](auto&& pair) { return pair.second == squareIndex; });

        if (iter == std::end(engine::core::SQUARE_INDEX)) [[unlikely]]
        {
            LOG_ERROR("[utils::squareIndexToString()] Invalid square index: {}", squareIndex);
            return "Unknown";
        }

        return iter->first;
    }
} // namespace utils

#endif // UTILS_H_