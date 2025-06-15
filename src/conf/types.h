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
#include "engine/include/bitboard.h"

/**
 * @namespace conf::types
 */
namespace conf::types
{
    /**
     * @typedef PieceBoards
     * @brief   2x6 array holding each piece's bitboards from each team.
     */
    typedef std::array<std::array<engine::Bitboard, conf::enums::Pieces::PIECES>, conf::enums::Colors::COLORS> PiecesBitboards;

    /**
     * @typedef Occupancy
     * @brief   Single bitboard holding the board's occupancy.
     */
    typedef engine::Bitboard Occupancy;

    /**
     * @typedef BorderFilesMasks
     * @brief   1x2 array holding the border files (A and H) masks.
     * @details Used to prevent a piece from moving out of the board by the left / right side.
     */
    typedef std::array<engine::Bitboard, conf::enums::BorderFiles::FILES> BorderFilesMasks;

    /**
     * @typedef RanksMasks
     * @brief   1x8 array holding each rank's mask.
     * @details Used to check pawns promotions, etc...
     */
    typedef std::array<engine::Bitboard, conf::enums::Ranks::RANKS> RanksMasks;


} // namespace conf::types

#endif // TYPES_H_
