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
            conf::types::PiecesBitboards piecesBBs;

            // Occupancy of the board (1 general + 1 for each team)
            conf::types::Occupancy generalOccupancyBB;
            conf::types::Occupancy whiteOccupancyBB;
            conf::types::Occupancy blackOccupancyBB;

            // Masks
            const conf::types::BorderFilesMasks borderFilesMasksBBs;
            const conf::types::RanksMasks ranksMasks;
    };
} // namespace engine

#endif // BOARD_H_