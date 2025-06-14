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
     * @brief 2x6 array of pieces bitboards (1 for each piece from each piece)
     */
    typedef std::array<std::array<engine::Bitboard, conf::enums::Piece::PIECES>, conf::enums::Team::COLORS> pieceBitboards;
} // namespace conf::types

#endif // TYPES_H_