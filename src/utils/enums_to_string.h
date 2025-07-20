/**
 * @file      enums_to_string.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Allows to convert enums to std::string_view.
 */

#ifndef ENUMS_TO_STRING_
#define ENUMS_TO_STRING_

#include <string_view>

#include "engine/core/enums.h"

namespace utils
{
    inline constexpr std::string_view toString(engine::core::Pieces v) noexcept
    {
        switch (v)
        {
        case engine::core::Pieces::PAWN:
            return "PAWN";
        case engine::core::Pieces::KNIGHT:
            return "KNIGHT";
        case engine::core::Pieces::BISHOP:
            return "BISHOP";
        case engine::core::Pieces::ROOK:
            return "ROOK";
        case engine::core::Pieces::QUEEN:
            return "QUEEN";
        case engine::core::Pieces::KING:
            return "KING";
        case engine::core::Pieces::PIECES:
            return "6";
        case engine::core::Pieces::UNKNOWN_PIECE:
            return "UNKNOWN_PIECE";
        }
        return "UNKNOWN_PIECE";
    }

    inline constexpr engine::core::Pieces fromString(std::string_view s) noexcept
    {
        using namespace engine::core;
        return (s == "pawn")   ? Pieces::PAWN
             : (s == "knight") ? Pieces::KNIGHT
             : (s == "bishop") ? Pieces::BISHOP
             : (s == "rook")   ? Pieces::ROOK
             : (s == "queen")  ? Pieces::QUEEN
             : (s == "king")   ? Pieces::KING
             : (s == "pieces") ? Pieces::PIECES
                               : Pieces::UNKNOWN_PIECE;
    }

    inline constexpr std::string_view toString(engine::core::Colors v) noexcept
    {
        switch (v)
        {
        case engine::core::Colors::WHITE:
            return "WHITE";
        case engine::core::Colors::BLACK:
            return "BLACK";
        case engine::core::Colors::COLORS:
            return "2";
        case engine::core::Colors::UNKNOWN_COLOR:
            return "UNKNOWN_COLOR";
        }
        return "UNKNOWN_COLOR";
    }

    inline constexpr std::string_view toString(engine::core::Ranks v) noexcept
    {
        switch (v)
        {
        case engine::core::Ranks::RANK_1:
            return "RANK_1";
        case engine::core::Ranks::RANK_2:
            return "RANK_2";
        case engine::core::Ranks::RANK_3:
            return "RANK_3";
        case engine::core::Ranks::RANK_4:
            return "RANK_4";
        case engine::core::Ranks::RANK_5:
            return "RANK_5";
        case engine::core::Ranks::RANK_6:
            return "RANK_6";
        case engine::core::Ranks::RANK_7:
            return "RANK_7";
        case engine::core::Ranks::RANK_8:
            return "RANK_8";
        case engine::core::Ranks::RANKS:
            return "8";
        case engine::core::Ranks::UNKNOWN_RANK:
            return "UNKNOWN_RANK";
        }
        return "UNKNOWN_RANK";
    }

    inline constexpr std::string_view toString(engine::core::Files v) noexcept
    {
        switch (v)
        {
        case engine::core::Files::FILE_A:
            return "FILE_A";
        case engine::core::Files::FILE_B:
            return "FILE_B";
        case engine::core::Files::FILE_C:
            return "FILE_C";
        case engine::core::Files::FILE_D:
            return "FILE_D";
        case engine::core::Files::FILE_E:
            return "FILE_E";
        case engine::core::Files::FILE_F:
            return "FILE_F";
        case engine::core::Files::FILE_G:
            return "FILE_G";
        case engine::core::Files::FILE_H:
            return "FILE_H";
        case engine::core::Files::FILES:
            return "8";
        case engine::core::Files::UNKNOWN_FILE:
            return "UNKNOWN_FILE";
        }
        return "UNKNOWN_FILE";
    }

    inline constexpr std::string_view toString(engine::core::Directions v) noexcept
    {
        switch (v)
        {
        case engine::core::Directions::NORTH:
            return "NORTH";
        case engine::core::Directions::SOUTH:
            return "SOUTH";
        case engine::core::Directions::EAST:
            return "EAST";
        case engine::core::Directions::WEST:
            return "WEST";
        case engine::core::Directions::NORTH_EAST:
            return "NORTH_EAST";
        case engine::core::Directions::NORTH_WEST:
            return "NORTH_WEST";
        case engine::core::Directions::SOUTH_EAST:
            return "SOUTH_EAST";
        case engine::core::Directions::SOUTH_WEST:
            return "SOUTH_WEST";
        case engine::core::Directions::NORTH_NORTH:
            return "NORTH_NORTH";
        case engine::core::Directions::SOUTH_SOUTH:
            return "SOUTH_SOUTH";
        case engine::core::Directions::NNE:
            return "NNE";
        case engine::core::Directions::ENE:
            return "ENE";
        case engine::core::Directions::ESE:
            return "ESE";
        case engine::core::Directions::SSE:
            return "SSE";
        case engine::core::Directions::SSW:
            return "SSW";
        case engine::core::Directions::WSW:
            return "WSW";
        case engine::core::Directions::WNW:
            return "WNW";
        case engine::core::Directions::NNW:
            return "NNW";
        case engine::core::Directions::DIRECTIONS:
            return "18";
        case engine::core::Directions::UNKNOWN_DIRECTION:
            return "UNKNOWN_DIRECTION";
        }
        return "UNKNOWN_DIRECTION";
    }

    inline constexpr std::string_view toString(engine::core::MoveTypes v) noexcept
    {
        switch (v)
        {
        case engine::core::MoveTypes::QUIET:
            return "QUIET";
        case engine::core::MoveTypes::CAPTURE:
            return "CAPTURE";
        case engine::core::MoveTypes::PROMOTION:
            return "PROMOTION";
        case engine::core::MoveTypes::CASTLE:
            return "CASTLE";
        case engine::core::MoveTypes::DOUBLE_PUSH:
            return "DOUBLE_PUSH";
        case engine::core::MoveTypes::EN_PASSANT:
            return "EN_PASSANT";
        case engine::core::MoveTypes::MOVE_TYPES:
            return "5";
        case engine::core::MoveTypes::UNKNOWN_MOVE_TYPE:
            return "UNKNOWN_MOVE_TYPE";
        }
        return "UNKNOWN_MOVE_TYPE";
    }

    inline constexpr std::string_view toString(engine::core::Castlings v) noexcept
    {
        switch (v)
        {
        case engine::core::Castlings::WHITE_KING_SIDE:
            return "WHITE_KING_SIDE";
        case engine::core::Castlings::WHITE_QUEEN_SIDE:
            return "WHITE_QUEEN_SIDE";
        case engine::core::Castlings::BLACK_KING_SIDE:
            return "BLACK_KING_SIDE";
        case engine::core::Castlings::BLACK_QUEEN_SIDE:
            return "BLACK_QUEEN_SIDE";
        case engine::core::Castlings::CASTLINGS:
            return "4";
        case engine::core::Castlings::UNKNOWN_CASTLING:
            return "UNKNOWN_CASTLING";
        }
        return "UNKNOWN_CASTLING";
    }

} // namespace utils

#endif // ENUMS_TO_STRING_
