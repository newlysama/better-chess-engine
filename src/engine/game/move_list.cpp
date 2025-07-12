/**
 * @file      move_list.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Implements MoveList class.
 */

#include "move_list.h"

#include <assert.h>

/**
 * @namespace engine::move
 */
namespace engine::move
{
    void MoveList::add(move::Move& move) noexcept
    {
        // clang-format off
            #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
                assert(this->_size < 256);
            #endif
        // clang-format on

        this->_moves[this->_size++] = move;
    }

    void MoveList::clear() noexcept
    {
        this->_size = 0;
    }

    std::size_t MoveList::size() noexcept
    {
        return this->_size;
    }

    move::Move& MoveList::operator[](std::size_t index) noexcept
    {
        return this->_moves[index];
    }
} // namespace engine::move
