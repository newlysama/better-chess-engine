/**
 * @file types.h
 * @author Thibault THOMAS
 * 
 * @brief Provides various types
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
     * @typedef pieceBoards
     * @brief 2x6 array holding each piece's bitboards from each team
     */
    typedef std::array<std::array<engine::Bitboard, conf::enums::Piece::PIECES>, conf::enums::Team::COLORS> piecesBitboards;

    /**
     * @typedef teamPiecesBitboards;
     * @brief 1x6 array holding each piece's bitboard from a specific team
     */
    typedef std::array<engine::Bitboard, conf::enums::Piece::PIECES> teamPiecesBitboards;

    /**
     * @typedef occupancy
     * @brief Single bitboard holding occupancy
     */
    typedef engine::Bitboard occupancy;

} // namespace conf::types

#endif // TYPES_H_
