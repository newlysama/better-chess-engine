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

#include "conf/enums.h"
#include "engine/board/bitboard.h"

/**
 * @namespace conf::types
 */
namespace conf::types
{
    /**
     * @typedef PieceBoards
     * @brief   2x6 array holding each piece's bitboards from each team.
     */
    typedef std::array<std::array<engine::board::Bitboard, conf::enums::Pieces::PIECES>, conf::enums::Colors::COLORS>
        PiecesBitboards;

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
        Move(const uint8_t from, const uint8_t to, const conf::enums::MoveTypes type) noexcept
            : squareFrom(from), squareTo(to), moveType(type)
        {
        }
    } Move;

    /**
     * @typedef LegalMoves
     * @brief   1x5 array storing vectors of legal moves for each move type.
     */
    typedef std::array<std::vector<Move>, conf::enums::MoveTypes::MOVE_TYPES> LegalMoves;

    /*----------------------------------*
     *              MASKS               *
     *----------------------------------*/

    /**
     * @typedef BorderFilesMasks
     * @brief   1x8 array holding each file's mask.
     */
    typedef std::array<engine::board::Bitboard, conf::enums::Files::FILES> FilesMasks;

    /**
     * @typedef RanksMasks
     * @brief   1x8 array holding each rank's mask.
     */
    typedef std::array<engine::board::Bitboard, conf::enums::Ranks::RANKS> RanksMasks;

    /**
     * @typedef DiagonalMasks
     * @brief   1x15 array holing each diagonal's mask.
     */
    typedef std::array<engine::board::Bitboard, 15> DiagonalMasks;

    /**
     * @typedef BitboardTable
     * @brief   x64 array holding masks for each square.
     */
    typedef std::array<engine::board::Bitboard, 64> BitboardTable;

    /**
     * @typedef CastlingMasks
     * @brief   Castling masks: one mask per castling right (e.g., White King-side, White Queen-side, ...).
     */
    typedef std::array<engine::board::Bitboard, conf::enums::Castlings::CASTLINGS> CastlingMasks;

    /**
     * @typedef BetweenMasks
     * @brief   Masks of squares strictly between any two squares (for line checks, pinned pieces, ...).
     */
    typedef std::array<std::array<engine::board::Bitboard, 64>, 64> BetweenMasks;

    /**
     * @typedef SlidingAttackTable
     * @brief   Attack Tables for sliding pieces.
     */
    typedef std::array<std::vector<engine::board::Bitboard>, 64> SlidingAttackTable;

} // namespace conf::types

#endif // TYPES_H_
