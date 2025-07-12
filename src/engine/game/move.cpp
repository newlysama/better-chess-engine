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
 * @namespace engine::game::move
 */
namespace engine::game::move
{
    using namespace conf::enums;
    using namespace board;

    Move::Move(const int from, const int to, const MoveTypes type) noexcept
        : _squareFrom(from)
        , _squareTo(to)
        , _moveType(type)
    {
    }

    inline void Move::make(Board& board) noexcept
    {
    }

    inline void Move::unmake(Board& board) noexcept
    {
    }
} // namespace engine::game::move