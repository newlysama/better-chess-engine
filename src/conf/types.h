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
    typedef std::array<std::array<engine::board::Bitboard, conf::enums::PIECES>, conf::enums::COLORS> PiecesBitboards;

    /*----------------------------------*
     *              MASKS               *
     *----------------------------------*/

    /**
     * @typedef BorderFilesMasks
     * @brief   1x8 array holding each file's mask.
     */
    typedef std::array<engine::board::Bitboard, conf::enums::FILES> FilesMasks;

    /**
     * @typedef RanksMasks
     * @brief   1x8 array holding each rank's mask.
     */
    typedef std::array<engine::board::Bitboard, conf::enums::RANKS> RanksMasks;

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
    typedef std::array<engine::board::Bitboard, conf::enums::CASTLINGS> CastlingMasks;

    /**
     * @typedef BetweenMasks
     * @brief   Masks of squares strictly between any two squares (for line checks, pinned pieces, ...).
     */
    typedef std::array<std::array<engine::board::Bitboard, 64>, 64> BetweenMasks;

    /**
     * @typedef rookAttackTable
     * @brief   Attack Tables for sliding rook.
     */
    typedef std::array<std::array<engine::board::Bitboard, 4096>, 64> rookAttacksTable;

    /**
     * @typedef bishopAttackTable
     * @brief   Attack Tables for sliding bishop.
     */
    typedef std::array<std::array<engine::board::Bitboard, 512>, 64> bishopAttacksTable;

} // namespace conf::types

#endif // TYPES_H_
