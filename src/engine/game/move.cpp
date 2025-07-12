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
 * @namespace engine::move
 */
namespace engine::move
{
    Move::Move(const std::size_t from, const std::size_t to, const conf::enums::MoveTypes type) noexcept
        : _squareFrom(from), _squareTo(to), _moveType(type)
    {
    }

    inline void Move::make(board::Board& board) noexcept
    {
    }

    inline void Move::unmake(board::Board& board) noexcept
    {
    }
} // namespace engine::move