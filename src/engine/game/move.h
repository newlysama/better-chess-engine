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

#include "utils/enums_to_string.h"
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

        /**
         * @brief Constructor specifying a castling type.
         */
        Move(const int from, const int to, const core::MoveTypes type, const core::Pieces fromPiece,
             const core::Castlings castling) noexcept;

        bool operator==(const Move& move) const noexcept;
        bool operator!=(const Move& move) const noexcept;

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
                std::cout << "Square from: " << fromSquare << "\n"
                          << "Square to:   " << toSquare   << "\n"
                          << "Move type:   " << utils::toString(moveType) << "\n";
            }
        #endif
        // clang-format on

        int fromSquare = 0;       // Start square
        int toSquare = 0;         // Target square
        core::MoveTypes moveType; // Move type (Capture, EnPassant, etc...)
        core::Pieces fromPiece;   // Piece type on fromSquare
        core::Castlings castling; // Type if castling
        bool promotion;           // Wether this move is a promotion
    };
} // namespace engine::game

#endif // MOVE_H
