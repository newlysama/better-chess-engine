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

namespace utils
{
    /**
     * @brief Retreives the rank/file string notation given a square index.
     *
     * @param [in] square : the index of the square within a Bitboard
     * @return std::string_view : rank/file string notation or "Unkown" if square is not valid
     */
    inline std::string_view squareIndexToString(const int square)
    {
        auto iter = std::find_if(std::begin(engine::core::SQUARE_INDEX), std::end(engine::core::SQUARE_INDEX),
                                 [&square](auto&& pair) { return pair.second == square; });

        if (iter == std::end(engine::core::SQUARE_INDEX)) [[unlikely]]
        {
            return "None";
        }

        return iter->first;
    }

    /**
     * @brief Given a char used in FEN notation to represent a piece, return a pair Color - Piece.
     *
     * @param [in] c : char to check
     * @return std::pair<Color, Piece> : the according pair
     */
    inline std::pair<engine::core::Color, engine::core::Piece> fenCharToPiece(const char c) noexcept
    {
        using namespace engine::core;

        switch (c)
        {
        case 'p':
            return {Color::BLACK, Piece::PAWN};
        case 'n':
            return {Color::BLACK, Piece::KNIGHT};
        case 'r':
            return {Color::BLACK, Piece::ROOK};
        case 'b':
            return {Color::BLACK, Piece::BISHOP};
        case 'q':
            return {Color::BLACK, Piece::QUEEN};
        case 'k':
            return {Color::BLACK, Piece::KING};
        case 'P':
            return {Color::WHITE, Piece::PAWN};
        case 'N':
            return {Color::WHITE, Piece::KNIGHT};
        case 'R':
            return {Color::WHITE, Piece::ROOK};
        case 'B':
            return {Color::WHITE, Piece::BISHOP};
        case 'Q':
            return {Color::WHITE, Piece::QUEEN};
        case 'K':
            return {Color::WHITE, Piece::KING};
        default:
            return {Color::UNKNOWN_COLOR, Piece::UNKNOWN_PIECE};
        }
    }
} // namespace utils

#endif // UTILS_H_