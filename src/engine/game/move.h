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

#include "engine/core/enums.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    using namespace engine::core;

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

        inline constexpr bool operator==(Move& move) noexcept;
        inline constexpr bool operator!=(Move& move) noexcept;

        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            inline void print() const noexcept
            {
                std::string_view typeStr =
                      _moveType == CAPTURE    ? "Capture"
                    : _moveType == QUIET      ? "Quiet"
                    : _moveType == PROMOTION  ? "Promotion"
                    : _moveType == CASTLE     ? "Castle"
                    : _moveType == ENPASSANT  ? "En Passant"
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
} // namespace engine::game

#endif // MOVE_H
