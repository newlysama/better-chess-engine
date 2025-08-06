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

namespace engine::core
{
    /**
     * @typedef PieceBoards
     * @brief   2x6 array holding each piece's bitboards from each team.
     */
    typedef std::array<std::array<board::Bitboard, Piece::N_PIECES>, Color::N_COLORS> PiecesBitboards;

    /**
     * @typedef ColoredOccupancies
     * @brief   1x2 array holding each team's occupancy.
     */
    typedef std::array<board::Bitboard, Color::N_COLORS> ColoredOccupancies;

    /**
     * @typedef PieceAt
     * @brief   2x64 array holding each square's Piece.
     */
    typedef std::array<std::array<Piece, 64>, Color::N_COLORS> PieceAt;

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
    typedef std::array<board::Bitboard, File::N_FILES> FilesMasks;

    /**
     * @typedef RanksMasks
     * @brief   1x8 array holding each rank's mask.
     */
    typedef std::array<board::Bitboard, Rank::N_RANKS> RanksMasks;

    /**
     * @typedef DiagonalMasks
     * @brief   1x15 array holding each diagonal's mask.
     */
    typedef std::array<board::Bitboard, 15> DiagonalMasks;

    /**
     * @typedef RayMasks
     * @brief   64x8 array holding each ray mask for each direction from each square.
     */
    typedef std::array<std::array<board::Bitboard, Direction::N_DIRECTIONS>, 64> RayMasks;

    /**
     * @typedef BitboardTable
     * @brief   x64 array holding masks for each square.
     */
    typedef std::array<board::Bitboard, 64> BitboardTable;

    /**
     * @typedef CastlingMasks
     * @brief   1x4 array holding each castling right's mask (e.g., White King-side, White Queen-side, ...).
     */
    typedef std::array<board::Bitboard, Castling::N_CASTLINGS> CastlingMasks;

    /**
     * @typedef RookCastlingMap
     * @brief   1x4 array of pair<int, Castling> mapping
     * the 4 rooks starting squares to their according castling.
     */
    typedef std::array<std::pair<int, Castling>, Castling::N_CASTLINGS> RookCastlingMap;

    /**
     * @typedef BetweenMasks
     * @brief   Masks of squares strictly between any two squares (for line checks, pinned pieces, ...).
     */
    typedef std::array<std::array<board::Bitboard, 64>, 64> BetweenMasks;

    /**
     * @typedef KingSquares
     * @brief   1x2 array holding each king's square index.
     */
    typedef std::array<int, core::Color::N_COLORS> KingSquares;

    /**
     * @typedef PinnedPieces
     * @brief   2x64 array holding each team's pinned pieces bitboards
     *          of allowed destination squares.
     */
    typedef std::array<core::BitboardTable, core::Color::N_COLORS> PinnedPieces;

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
