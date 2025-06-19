/**
 * @file enums.h
 * @author Thibault THOMAS
 *
 * @brief Provides various enums.
 */

#ifndef ENUMS_H_
#define ENUMS_H_

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace conf::enums
     */
    namespace conf::enums
    {
        /**
         * @enum  Piece
         * @brief Enum of pieces types.
         */
        enum Pieces
        {
            PAWN = 0,
            KNIGHT = 1,
            BISHOP = 2,
            ROOK = 3,
            QUEEN = 4,
            KING = 5,
            PIECES = 6
        };

        /**
         * @enum  Colors
         * @brief Enum of teams.
         */
        enum Colors
        {
            WHITE = 0,
            BLACK = 1,
            COLORS = 2
        };

        /**
         * @enum  Ranks
         * @brief Enum of board's ranks.
         */
        enum Ranks
        {
            RANK_1 = 0,
            RANK_2 = 1,
            RANK_3 = 2,
            RANK_4 = 3,
            RANK_5 = 4,
            RANK_6 = 5,
            RANK_7 = 6,
            RANK_8 = 7,
            RANKS = 8,
        };

        /**
         * @enum  Files
         * @brief Enum of board's files.
         */
        enum Files
        {
            FILE_A = 0,
            FILE_B = 1,
            FILE_C = 2,
            FILE_D = 3,
            FILE_E = 4,
            FILE_F = 5,
            FILE_G = 6,
            FILE_H = 7,
            FILES = 8,
        };

        /**
         * @enum  Directions
         * @brief Enum of possible moves directions.
         */
        enum Directions
        {
            NORTH = 0,
            SOUTH = 1,
            EAST = 2,
            WEST = 3,
            NORTH_EAST = 4,
            NORTH_WEST = 5,
            SOUTH_EAST = 6,
            SOUTH_WEST = 7,
            DIRECTIONS = 8
        };

        /**
         * @enum MoveTypes
         * @brief Enum of possible move types.
         */
        enum MoveTypes
        {
            QUIET = 0,
            CAPTURE = 1,
            PROMOTION = 2,
            CASTLE = 3,
            ENPASSANT = 4,
            MOVE_TYPES = 5
        };
    } // namespace conf::enums
} // namespace engine

#endif // ENUMS_H_