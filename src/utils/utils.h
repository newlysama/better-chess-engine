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

    /**
     * @brief Given a char used in FEN notation to represent a piece, return a pair Color - Piece.
     *
     * @param [in] c : char to check
     * @return std::pair<Colors, Pieces> : the according pair
     */
    inline std::pair<engine::core::Colors, engine::core::Pieces> fenCharToPiece(const char c) noexcept
    {
        using namespace engine::core;

        switch (c)
        {
        case 'p':
            return {Colors::BLACK, Pieces::PAWN};
        case 'n':
            return {Colors::BLACK, Pieces::KNIGHT};
        case 'r':
            return {Colors::BLACK, Pieces::ROOK};
        case 'b':
            return {Colors::BLACK, Pieces::BISHOP};
        case 'q':
            return {Colors::BLACK, Pieces::QUEEN};
        case 'k':
            return {Colors::BLACK, Pieces::KING};
        case 'P':
            return {Colors::WHITE, Pieces::PAWN};
        case 'N':
            return {Colors::WHITE, Pieces::KNIGHT};
        case 'R':
            return {Colors::WHITE, Pieces::ROOK};
        case 'B':
            return {Colors::WHITE, Pieces::BISHOP};
        case 'Q':
            return {Colors::WHITE, Pieces::QUEEN};
        case 'K':
            return {Colors::WHITE, Pieces::KING};
        default:
            return {Colors::UNKNOWN_COLOR, Pieces::UNKNOWN_PIECE};
        }
    }
} // namespace utils

#endif // UTILS_H_