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
    using namespace engine::board;

    Move::Move(const int from, const int to, const MoveTypes type) noexcept
        : _squareFrom(from)
        , _squareTo(to)
        , _moveType(type)
    {
    }

    inline constexpr bool Move::operator==(Move& move) noexcept
    {
        return this->_squareFrom == move._squareFrom && this->_squareTo == move._squareTo &&
               this->_moveType == move._moveType;
    }

    inline constexpr bool Move::operator!=(Move& move) noexcept
    {
        return this->_squareFrom != move._squareFrom && this->_squareTo != move._squareTo &&
               this->_moveType != move._moveType;
    }
} // namespace engine::game