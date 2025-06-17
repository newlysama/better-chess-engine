/**
 * @file board.h
 * @author Thibault THOMAS
 *
 * @brief Board representation.
 */

#ifndef BOARD_H_
#define BOARD_H_

/*----- System -----*/
#include <cstdint>
#include <memory>

/*----- Project Headers -----*/
#include "conf/types.h"

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
                constexpr Board() noexcept;

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
                static inline constexpr uint8_t getSquareIndex(uint8_t rankIndex, uint8_t fileIndex) noexcept { return (8 * rankIndex) + fileIndex; };

                /**
                 * @brief Get a rank's index, bases on a square's index
                 *
                 * @param [in] squareIndex : square's index
                 * @return uint8_t : the rank's index
                 */
                static inline constexpr uint8_t getRankIndex(const uint8_t squareIndex) noexcept { return squareIndex >> 3; };

                /**
                 * @brief Get a files's index, bases on a square's index.
                 *
                 * @param [in] squareIndex : square's index
                 * @return uint8_t : the file's index
                 */
                static inline constexpr uint8_t getFileIndex(uint8_t squareIndex) noexcept { return squareIndex & 7; };

            private:
                // Bitboards for each piece from each team
                conf::types::PiecesBitboards allPieces;

                // Occupancy of the board (1 general + 1 for each team)
                engine::board::Bitboard generalOccupancy;
                engine::board::Bitboard whiteOccupancy;
                engine::board::Bitboard blackOccupancy;

                // Masks
                const engine::conf::types::BorderFilesMasks borderFilesMasks;
                const engine::conf::types::RanksMasks ranksMasks;
                const engine::conf::types::DiagonalMasks diagonalMasks;
                const engine::conf::types::DiagonalMasks antiDiagonalMasks;
        };
    } // namespace board
} // namespace engine

#endif // BOARD_H_