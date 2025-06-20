/**
 * @file      types.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Provides various types.
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <array>
#include <vector>

#include "board/bitboard.h"
#include "conf/enums.h"

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace conf::types
     */
    namespace conf::types
    {
        /**
         * @typedef PieceBoards
         * @brief   2x6 array holding each piece's bitboards from each team.
         */
        typedef std::array<std::array<board::Bitboard, conf::enums::Pieces::PIECES>, conf::enums::Colors::COLORS>
            PiecesBitboards;

        /**
         * @typedef BorderFilesMasks
         * @brief   1x8 array holding each file's mask.
         * @details Used to prevent a piece from moving out of the board by the left / right side.
         */
        typedef std::array<board::Bitboard, conf::enums::Files::FILES> FilesMasks;

        /**
         * @typedef RanksMasks
         * @brief   1x8 array holding each rank's mask.
         * @details Used to check pawns promotions, etc...
         */
        typedef std::array<board::Bitboard, conf::enums::Ranks::RANKS> RanksMasks;

        /**
         * @typedef DiagonalMasks
         * @brief   1x15 array holing each diagonal's mask.
         * @details Used to check for Bishop's and Queen's legal moves, among other things.
         */
        typedef std::array<board::Bitboard, 15> DiagonalMasks;

        /**
         * @typedef Move
         * @brief   Structure representing a move.
         */
        typedef struct Move
        {
            uint8_t squareFrom; // Start square
            uint8_t squareTo;   // End square

            conf::enums::MoveTypes moveType; // Move type (Capture, EnPassant, etc...)

            /**
             * @brief Constructor.
             */
            Move(const uint8_t from, const uint8_t to, conf::enums::MoveTypes type) noexcept
                : squareFrom(from), squareTo(to), moveType(type)
            {
            }
        } Move;

        /**
         * @typedef LegalMoves
         * @brief   1x5 array storing vectors of legal moves for each move type.
         */
        typedef std::array<std::vector<Move>, conf::enums::MoveTypes::MOVE_TYPES> LegalMoves;

    } // namespace conf::types
} // namespace engine

#endif // TYPES_H_
