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
 * @namespace engine::core
 */
namespace engine::core
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
        PIECES = 6,
        UNKNOWN_PIECE = 255
    };

    /**
     * @enum  Colors
     * @brief Enum of teams.
     */
    enum Colors
    {
        WHITE = 0,
        BLACK = 1,
        COLORS = 2,
        UNKNOWN_COLOR = 255
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
        UNKNOWN_RANK = 255
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
        UNKNOWN_FILE = 255
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
        DIRECTIONS = 8,
        UNKNOWN_DIRECTION = 255
    };

    /**
     * @enum SpecialDirections
     * @brief Enum of special directions for
     * pawns double push / knight moves.
     */
    enum SpecialDirections
    {
        // Pawn specific dirs
        NORTH_NORTH = 0,
        SOUTH_SOUTH = 1,

        // Knight specific dirs
        NNE = 2,
        ENE = 3,
        ESE = 4,
        SSE = 5,
        SSW = 6,
        WSW = 7,
        WNW = 8,
        NNW = 9,
        SPECIAL_DIRS = 10,
        UNKNOWN_SPECIAL_DIR = 255
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
        DOUBLE_PUSH = 4,
        EN_PASSANT = 5,
        MOVE_TYPES = 6,
        UNKNOWN_MOVE_TYPE = 255
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
        CASTLINGS = 4,
        UNKNOWN_CASTLING = 255
    };
} // namespace engine::core

#endif // ENUMS_H_