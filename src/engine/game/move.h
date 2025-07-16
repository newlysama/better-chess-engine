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
        Move(const int from, const int to, const core::MoveTypes type, const core::Pieces fromPiece) noexcept;

        bool operator==(Move& move) const noexcept;
        bool operator!=(Move& move) const noexcept;

        /**
         * @brief Check if a move is set (not set when default constructed).
         *
         * @return bool : weather the move is set or not
         */
        bool isSet() const noexcept;

        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            inline void print() const noexcept
            {
                std::string_view typeStr =
                      moveType == core::MoveTypes::CAPTURE    ? "Capture"
                    : moveType == core::MoveTypes::QUIET      ? "Quiet"
                    : moveType == core::MoveTypes::PROMOTION  ? "Promotion"
                    : moveType == core::MoveTypes::CASTLE     ? "Castle"
                    : moveType == core::MoveTypes::ENPASSANT  ? "En Passant"
                    : "";

                std::cout << "Square from: " << squareFrom << "\n"
                          << "Square to:   " << squareTo   << "\n"
                          << "Move type:   " << typeStr      << "\n";
            }
        #endif
        // clang-format on

        int squareFrom;           // Start square
        int squareTo;             // Target square
        core::MoveTypes moveType; // Move type (Capture, EnPassant, etc...)
        core::Pieces fromPiece;   // Piece type on squareFrom
    };
} // namespace engine::game

#endif // MOVE_H
