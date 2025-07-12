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

#if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
#include <iostream>
#include <string_view>
#endif

#include "conf/enums.h"
#include "engine/board/board.h"

/**
 * @namespace engine::game::move
 */
namespace engine::game::move
{
    using namespace conf::enums;
    using namespace board;

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
        Move(const int from, const int to, const MoveTypes type) noexcept;

        /**
         * @brief Make a move.
         *
         * @param [in] board : The board to make the move on
         */
        void make(Board& board) noexcept;

        /**
         * @brief Unmake a move.
         *
         * @param [in] board : The board to unmake the move on
         */
        void unmake(Board& board) noexcept;

        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            inline void print() const noexcept
            {
                using namespace conf::enums;

                std::string_view typeStr =
                      _moveType == CAPTURE    ? "Capture"
                    : _moveType == QUIET      ? "Quiet"
                    : _moveType == PROMOTION  ? "Promotion"
                    : _moveType == CASTLE     ? "Castle"
                    : _moveType == ENPASSANT ? "En Passant"
                    : "";

                std::cout << "Square from: " << _squareFrom << "\n"
                          << "Square to:   " << _squareTo   << "\n"
                          << "Move type:   " << typeStr      << "\n";
            }
        #endif
        // clang-format on

      private:
        int _squareFrom;     // Start square
        int _squareTo;       // Target square
        MoveTypes _moveType; // Move type (Capture, EnPassant, etc...)
    };
} // namespace engine::game::move

#endif // MOVE_H
