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

#include "engine/board/bitboard.h"
#include "engine/core/enums.h"

/**
 * @namespace engine::core
 */
namespace engine::core
{
    /**
     * @typedef PieceBoards
     * @brief   2x6 array holding each piece's bitboards from each team.
     */
    typedef std::array<std::array<board::Bitboard, Pieces::PIECES>, Colors::COLORS> PiecesBitboards;

    /**
     * @typedef ColoredOccupancies
     * @brief   1x2 array holding each team's occupancy.
     */
    typedef std::array<board::Bitboard, Colors::COLORS> ColoredOccupancies;

    /**
     * @typedef CastlingRights
     * @brief   8 bits unsigned int where bits represent a type of castling right.
     * @details
     * Bit 0 : White King Side Castling |
     * Bit 1 : White Queen Side Castling |
     * Bit 2 : Black King Side Castling |
     * Bit 3 : Black Queen Side Castling
     */
    typedef uint8_t CastlingRights;

    /*----------------------------------*
     *              MASKS               *
     *----------------------------------*/

    /**
     * @typedef BorderFilesMasks
     * @brief   1x8 array holding each file's mask.
     */
    typedef std::array<board::Bitboard, Files::FILES> FilesMasks;

    /**
     * @typedef RanksMasks
     * @brief   1x8 array holding each rank's mask.
     */
    typedef std::array<board::Bitboard, Ranks::RANKS> RanksMasks;

    /**
     * @typedef DiagonalMasks
     * @brief   1x15 array holding each diagonal's mask.
     */
    typedef std::array<board::Bitboard, 15> DiagonalMasks;

    /**
     * @typedef BitboardTable
     * @brief   x64 array holding masks for each square.
     */
    typedef std::array<board::Bitboard, 64> BitboardTable;

    /**
     * @typedef CastlingMasks
     * @brief   Castling masks: one mask per castling right (e.g., White King-side, White Queen-side, ...).
     */
    typedef std::array<board::Bitboard, Castlings::CASTLINGS> CastlingMasks;

    /**
     * @typedef BetweenMasks
     * @brief   Masks of squares strictly between any two squares (for line checks, pinned pieces, ...).
     */
    typedef std::array<std::array<board::Bitboard, 64>, 64> BetweenMasks;

    /*----------------------------------*
     *      SLIDING ATTACK TABLES       *
     *----------------------------------*/

    /**
     * @typedef RookAttackTable
     * @brief   Attack Tables for sliding rook.
     */
    typedef std::array<std::array<board::Bitboard, 4096>, 64> RookAttacksTable;

    /**
     * @typedef BishopAttackTable
     * @brief   Attack Tables for sliding bishop.
     */
    typedef std::array<std::array<board::Bitboard, 512>, 64> BishopAttacksTable;

} // namespace engine::core

#endif // TYPES_H_
