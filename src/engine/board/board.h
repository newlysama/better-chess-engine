/**
 * @file      board.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Board representation.
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <memory>

#include "conf/types.h"
#include "engine/board/bitboard.h"
#include "engine/board/mask.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace conf::enums;
    using namespace conf::types;
    using namespace engine::board::mask;

    /**
     * @class Board
     * @brief Board represented by multiple Bitboards.
     */
    class Board
    {
      public:
        /**
         * @brief Constructor.
         */
        Board() noexcept;

        /**
         * @brief Constructor from FEN notation.
         */
        Board(const std::string_view fenNotation) noexcept;

        /**
         * @brief Destructor.
         */
        ~Board() = default;

        /**
         * @brief Get a square's index, based on the rank's and file's indexes.
         *
         * @param [in] rankInex  : rank's index
         * @param [in] fileIndex : file's index
         * @return int : the square's index
         */
        static inline constexpr int getSquareIndex(const int rankIndex, const int fileIndex) noexcept
        {
            return (8 * rankIndex) + fileIndex;
        };

        /**
         * @brief Get a rank's index, bases on a square's index
         *
         * @param [in] squareIndex : square's index
         * @return int : the rank's index
         */
        static inline constexpr int getRankIndex(const int squareIndex) noexcept
        {
            return squareIndex >> 3;
        };

        /**
         * @brief Get a files's index, bases on a square's index.
         *
         * @param [in] squareIndex : square's index
         * @return int : the file's index
         */
        static inline constexpr int getFileIndex(const int squareIndex) noexcept
        {
            return squareIndex & 7;
        };

        template <Directions Dir>
        static inline constexpr Bitboard shiftDir(Bitboard bb) noexcept
        {
            return Dir == NORTH       ? Bitboard(bb << 8)
                 : Dir == SOUTH       ? Bitboard(bb >> 8)
                 : Dir == EAST        ? Bitboard((bb & ~FILE_H_MASK) << 1)
                 : Dir == WEST        ? Bitboard((bb & ~FILE_A_MASK) >> 1)
                 : Dir == NORTH_EAST  ? Bitboard((bb & ~FILE_H_MASK) << 9)
                 : Dir == NORTH_WEST  ? Bitboard((bb & ~FILE_A_MASK) << 7)
                 : Dir == SOUTH_EAST  ? Bitboard((bb & ~FILE_H_MASK) >> 7)
                 : Dir == SOUTH_WEST  ? Bitboard((bb & ~FILE_A_MASK) >> 9)
                 : Dir == NORTH_NORTH ? Bitboard(bb << 16)
                 : Dir == SOUTH_SOUTH ? Bitboard(bb >> 16)
                                      : Bitboard(0ULL);
        }

        // Bitboards for each piece from each team
        PiecesBitboards allPieces;

        // Occupancy of the board (1 general + 1 for each team)
        Bitboard generalOccupancy;
        std::array<Bitboard, Colors::COLORS> coloredOccupancies;
    };

} // namespace engine::board

#endif // BOARD_H_