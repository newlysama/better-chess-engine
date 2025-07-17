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
        : fromSquare(from)
        , toSquare(to)
        , moveType(type)
        , fromPiece(fromPiece)
        , castling(Castlings::UNKNOWN_CASTLING)
        , promotion(false)
    {
    }

    Move::Move(const int from, const int to, const core::MoveTypes type, const core::Pieces fromPiece,
               const Castlings castling) noexcept
        : fromSquare(from)
        , toSquare(to)
        , moveType(type)
        , fromPiece(fromPiece)
        , castling(castling)
        , promotion(false)
    {
    }

    bool Move::operator==(const Move& move) const noexcept
    {
        return this->fromSquare == move.fromSquare && this->toSquare == move.toSquare &&
               this->moveType == move.moveType;
    }

    bool Move::operator!=(const Move& move) const noexcept
    {
        return this->fromSquare != move.fromSquare || this->toSquare != move.toSquare ||
               this->moveType != move.moveType;
    }

    bool Move::isSet() const noexcept
    {
        // If move is default constructed, both theese values will be 0 initialized.
        return !(this->fromSquare == 0 && this->toSquare == 0);
    }
} // namespace engine::game