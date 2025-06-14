/**
 * @file enums.h
 * @author Thibault THOMAS
 * 
 * @brief Provides various enums
 */

#ifndef ENUMS_H_
#define ENUMS_H_

/**
 * @namespace conf::enums
 */
namespace conf::enums
{
    /**
     * @enum Piece
     * @brief Enum of pieces types
     */
    enum Piece
    {
        PAWN   = 0,
        KNIGHT = 1,
        BISHOP = 2,
        ROOK   = 3,
        QUEEN  = 4,
        KING   = 5,
        PIECES = 6
    };

    /**
     * @enum Team
     * @brief Enum of teams
     */
    enum Team
    {
        WHITE  = 0,
        BLACK  = 1,
        COLORS = 2
    };
} // namespace conf::enums

#endif // ENUMS_H_