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
    inline constexpr std::string_view toString(engine::core::Piece v) noexcept
    {
        switch (v)
        {
        case engine::core::Piece::PAWN:
            return "PAWN";
        case engine::core::Piece::KNIGHT:
            return "KNIGHT";
        case engine::core::Piece::BISHOP:
            return "BISHOP";
        case engine::core::Piece::ROOK:
            return "ROOK";
        case engine::core::Piece::QUEEN:
            return "QUEEN";
        case engine::core::Piece::KING:
            return "KING";
        case engine::core::Piece::N_PIECES:
            return "6";
        case engine::core::Piece::UNKNOWN_PIECE:
            return "UNKNOWN_PIECE";
        }
        return "UNKNOWN_PIECE";
    }

    inline constexpr engine::core::Piece fromString(std::string_view s) noexcept
    {
        using namespace engine::core;
        return (s == "pawn")   ? Piece::PAWN
             : (s == "knight") ? Piece::KNIGHT
             : (s == "bishop") ? Piece::BISHOP
             : (s == "rook")   ? Piece::ROOK
             : (s == "queen")  ? Piece::QUEEN
             : (s == "king")   ? Piece::KING
             : (s == "pieces") ? Piece::N_PIECES
                               : Piece::UNKNOWN_PIECE;
    }

    inline constexpr std::string_view toString(engine::core::Color v) noexcept
    {
        switch (v)
        {
        case engine::core::Color::WHITE:
            return "WHITE";
        case engine::core::Color::BLACK:
            return "BLACK";
        case engine::core::Color::N_COLORS:
            return "2";
        case engine::core::Color::UNKNOWN_COLOR:
            return "UNKNOWN_COLOR";
        }
        return "UNKNOWN_COLOR";
    }

    inline constexpr std::string_view toString(engine::core::Rank v) noexcept
    {
        switch (v)
        {
        case engine::core::Rank::RANK_1:
            return "RANK_1";
        case engine::core::Rank::RANK_2:
            return "RANK_2";
        case engine::core::Rank::RANK_3:
            return "RANK_3";
        case engine::core::Rank::RANK_4:
            return "RANK_4";
        case engine::core::Rank::RANK_5:
            return "RANK_5";
        case engine::core::Rank::RANK_6:
            return "RANK_6";
        case engine::core::Rank::RANK_7:
            return "RANK_7";
        case engine::core::Rank::RANK_8:
            return "RANK_8";
        case engine::core::Rank::N_RANKS:
            return "8";
        case engine::core::Rank::UNKNOWN_RANK:
            return "UNKNOWN_RANK";
        }
        return "UNKNOWN_RANK";
    }

    inline constexpr std::string_view toString(engine::core::File v) noexcept
    {
        switch (v)
        {
        case engine::core::File::FILE_A:
            return "FILE_A";
        case engine::core::File::FILE_B:
            return "FILE_B";
        case engine::core::File::FILE_C:
            return "FILE_C";
        case engine::core::File::FILE_D:
            return "FILE_D";
        case engine::core::File::FILE_E:
            return "FILE_E";
        case engine::core::File::FILE_F:
            return "FILE_F";
        case engine::core::File::FILE_G:
            return "FILE_G";
        case engine::core::File::FILE_H:
            return "FILE_H";
        case engine::core::File::N_FILES:
            return "8";
        case engine::core::File::UNKNOWN_FILE:
            return "UNKNOWN_FILE";
        }
        return "UNKNOWN_FILE";
    }

    inline constexpr std::string_view toString(engine::core::Direction v) noexcept
    {
        switch (v)
        {
        case engine::core::Direction::NORTH:
            return "NORTH";
        case engine::core::Direction::SOUTH:
            return "SOUTH";
        case engine::core::Direction::EAST:
            return "EAST";
        case engine::core::Direction::WEST:
            return "WEST";
        case engine::core::Direction::NORTH_EAST:
            return "NORTH_EAST";
        case engine::core::Direction::NORTH_WEST:
            return "NORTH_WEST";
        case engine::core::Direction::SOUTH_EAST:
            return "SOUTH_EAST";
        case engine::core::Direction::SOUTH_WEST:
            return "SOUTH_WEST";
        case engine::core::Direction::N_DIRECTIONS:
            return "8";
        case engine::core::Direction::UNKNOWN_DIRECTION:
            return "UNKNOWN_DIRECTION";
        }
        return "UNKNOWN_DIRECTION";
    }

    inline constexpr std::string_view toString(engine::core::SpecialDirection v)
    {
        switch (v)
        {
        case engine::core::SpecialDirection::NORTH_NORTH:
            return "NORTH_NORTH";
        case engine::core::SpecialDirection::SOUTH_SOUTH:
            return "SOUTH_SOUTH";
        case engine::core::SpecialDirection::NNE:
            return "NNE";
        case engine::core::SpecialDirection::ENE:
            return "ENE";
        case engine::core::SpecialDirection::ESE:
            return "ESE";
        case engine::core::SpecialDirection::SSE:
            return "SSE";
        case engine::core::SpecialDirection::SSW:
            return "SSW";
        case engine::core::SpecialDirection::WSW:
            return "WSW";
        case engine::core::SpecialDirection::WNW:
            return "WNW";
        case engine::core::SpecialDirection::NNW:
            return "NNW";
        case engine::core::SpecialDirection::N_SPECIAL_DIRS:
            return "10";
        case engine::core::SpecialDirection::UNKNOWN_SPECIAL_DIR:
            return "UNKNOWN_SPECIAL_DIR";
        }

        return "UNKNOWN_SPECIAL_DIR";
    }

    inline constexpr std::string_view toString(engine::core::MoveType v) noexcept
    {
        switch (v)
        {
        case engine::core::MoveType::QUIET:
            return "QUIET";
        case engine::core::MoveType::CAPTURE:
            return "CAPTURE";
        case engine::core::MoveType::PROMOTION:
            return "PROMOTION";
        case engine::core::MoveType::CASTLE:
            return "CASTLE";
        case engine::core::MoveType::DOUBLE_PUSH:
            return "DOUBLE_PUSH";
        case engine::core::MoveType::EN_PASSANT:
            return "EN_PASSANT";
        case engine::core::MoveType::N_MOVE_TYPES:
            return "5";
        case engine::core::MoveType::UNKNOWN_MOVE_TYPE:
            return "UNKNOWN_MOVE_TYPE";
        }
        return "UNKNOWN_MOVE_TYPE";
    }

    inline constexpr std::string_view toString(engine::core::Castling v) noexcept
    {
        switch (v)
        {
        case engine::core::Castling::WHITE_KING_SIDE:
            return "WHITE_KING_SIDE";
        case engine::core::Castling::WHITE_QUEEN_SIDE:
            return "WHITE_QUEEN_SIDE";
        case engine::core::Castling::BLACK_KING_SIDE:
            return "BLACK_KING_SIDE";
        case engine::core::Castling::BLACK_QUEEN_SIDE:
            return "BLACK_QUEEN_SIDE";
        case engine::core::Castling::N_CASTLINGS:
            return "4";
        case engine::core::Castling::UNKNOWN_CASTLING:
            return "UNKNOWN_CASTLING";
        }
        return "UNKNOWN_CASTLING";
    }

} // namespace utils

#endif // ENUMS_TO_STRING_
