/**
 * @file      move.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Move definition.
 */

#ifndef MOVE_H_
#define MOVE_H_

#include <cstdint>

#include "conf/enums.h"
#include "engine/board/board.h"

/**
 * @namespace engine::move
 */
namespace engine::move
{
    /**
     * @class Move
     */
    class Move
    {
      public:
        /**
         * @brief Default constructor.
         */
        Move() noexcept = default;

        /**
         * @brief Constructor.
         */
        Move(const std::size_t from, const std::size_t to, const conf::enums::MoveTypes type) noexcept;

        /**
         * @brief Make a move.
         *
         * @param [in] board : The board to make the move on
         */
        inline void make(board::Board& board) noexcept;

        /**
         * @brief Unmake a move.
         *
         * @param [in] board : The board to unmake the move on
         */
        inline void unmake(board::Board& board) noexcept;

      private:
        std::size_t _squareFrom;          // Start square
        std::size_t _squareTo;            // Target square
        conf::enums::MoveTypes _moveType; // Move type (Capture, EnPassant, etc...)
    };
} // namespace engine::move

#endif // MOVE_H
