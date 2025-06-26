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

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace board
     */
    namespace board
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
            Board(const std::string fenNotation) noexcept;

            /**
             * @brief Destructor.
             */
            ~Board() = default;

            /**
             * @brief Get a square's index, based on the rank's and file's indexes.
             *
             * @param [in] rankInex  : rank's index
             * @param [in] fileIndex : file's index
             * @return uint8_t : the square's index
             */
            static inline constexpr uint8_t getSquareIndex(const uint8_t rankIndex, const uint8_t fileIndex) noexcept
            {
                return (8 * rankIndex) + fileIndex;
            };

            /**
             * @brief Get a rank's index, bases on a square's index
             *
             * @param [in] squareIndex : square's index
             * @return uint8_t : the rank's index
             */
            static inline constexpr uint8_t getRankIndex(const uint8_t squareIndex) noexcept
            {
                return squareIndex >> 3;
            };

            /**
             * @brief Get a files's index, bases on a square's index.
             *
             * @param [in] squareIndex : square's index
             * @return uint8_t : the file's index
             */
            static inline constexpr uint8_t getFileIndex(const uint8_t squareIndex) noexcept
            {
                return squareIndex & 7;
            };

            // Bitboards for each piece from each team
            conf::types::PiecesBitboards allPieces;

            // Occupancy of the board (1 general + 1 for each team)
            board::Bitboard generalOccupancy;
            board::Bitboard whiteOccupancy;
            board::Bitboard blackOccupancy;
        };

    } // namespace board
} // namespace engine

#endif // BOARD_H_