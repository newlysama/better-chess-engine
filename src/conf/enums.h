/**
 * @file      enums.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Provides various enums.
 */

#ifndef ENUMS_H_
#define ENUMS_H_

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

        // Pawn specific dirs
        NORTH_NORTH = 8,
        SOUTH_SOUTH = 9,

        // Knight specific dirs
        NNE = 10,
        ENE = 11,
        ESE = 12,
        SSE = 13,
        SSW = 14,
        WSW = 15,
        WNW = 16,
        NNW = 17,
        DIRECTIONS = 18
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

    /**
     * @enum Castlings
     * @brief Enum of castlings options.
     */
    enum Castlings
    {
        WHITE_KING_SIDE = 0,
        WHITE_QUEEN_SIDE = 1,
        BLACK_KING_SIDE = 2,
        BLACK_QUEEN_SIDE = 3,
        CASTLINGS = 4
    };
} // namespace conf::enums

#endif // ENUMS_H_