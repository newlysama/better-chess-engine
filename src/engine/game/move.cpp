/**
 * @file      move.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Move implementation.
 */

#include "move.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    using namespace engine::core;

    Move::Move(const int from, const int to, const MoveTypes type, const Pieces fromPiece) noexcept
        : squareFrom(from)
        , squareTo(to)
        , moveType(type)
        , fromPiece(fromPiece)
        , castling(Castlings::UNKNOWN_CASTLING)
    {
    }

    Move::Move(const int from, const int to, const core::MoveTypes type, const core::Pieces fromPiece,
               const Castlings castling) noexcept
        : squareFrom(from)
        , squareTo(to)
        , moveType(type)
        , fromPiece(fromPiece)
        , castling(castling)
    {
    }

    bool Move::operator==(Move& move) const noexcept
    {
        return this->squareFrom == move.squareFrom && this->squareTo == move.squareTo &&
               this->moveType == move.moveType;
    }

    bool Move::operator!=(Move& move) const noexcept
    {
        return this->squareFrom != move.squareFrom || this->squareTo != move.squareTo ||
               this->moveType != move.moveType;
    }

    bool Move::isSet() const noexcept
    {
        // If move is default constructed, both theese values will be 0 initialized.
        return !(this->squareFrom == 0 && this->squareTo == 0);
    }
} // namespace engine::game