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
         * @return std::size_t : the square's index
         */
        static inline constexpr std::size_t getSquareIndex(const std::size_t rankIndex,
                                                           const std::size_t fileIndex) noexcept
        {
            return (8 * rankIndex) + fileIndex;
        };

        /**
         * @brief Get a rank's index, bases on a square's index
         *
         * @param [in] squareIndex : square's index
         * @return std::size_t : the rank's index
         */
        static inline constexpr std::size_t getRankIndex(const std::size_t squareIndex) noexcept
        {
            return squareIndex >> 3;
        };

        /**
         * @brief Get a files's index, bases on a square's index.
         *
         * @param [in] squareIndex : square's index
         * @return std::size_t : the file's index
         */
        static inline constexpr std::size_t getFileIndex(const std::size_t squareIndex) noexcept
        {
            return squareIndex & 7;
        };

        template <conf::enums::Directions Dir>
        static inline constexpr Bitboard shiftDir(Bitboard bb) noexcept
        {
            using namespace conf::enums;
            using namespace board::mask;

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

        template <conf::enums::Pieces Piece>
        inline void updateSlidingAttacksTable() noexcept;

        // Bitboards for each piece from each team
        conf::types::PiecesBitboards allPieces;

        // Occupancy of the board (1 general + 1 for each team)
        board::Bitboard generalOccupancy;
        board::Bitboard whiteOccupancy;
        board::Bitboard blackOccupancy;

        // Sliding attacks tables
        conf::types::SlidingAttackTable rookAttacksTables;
        conf::types::SlidingAttackTable bishopAttacksTables;
        conf::types::SlidingAttackTable queenAttacksTables;
    };

} // namespace engine::board

#endif // BOARD_H_