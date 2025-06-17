/**
 * @file types.h
 * @author Thibault THOMAS
 * 
 * @brief Provides various types.
 */

#ifndef TYPES_H_
#define TYPES_H_

/*----- System -----*/
#include <array>

/*----- Project Headers -----*/
#include "conf/enums.h"
#include "board/include/bitboard.h"

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
        typedef std::array<std::array<board::Bitboard, conf::enums::Pieces::PIECES>, conf::enums::Colors::COLORS> PiecesBitboards;

        /**
         * @typedef BorderFilesMasks
         * @brief   1x2 array holding the border files (A and H) masks.
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

    } // namespace conf::types
} // namespace engine


#endif // TYPES_H_
