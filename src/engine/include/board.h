/**
 * @file board.h
 * @author Thibault THOMAS
 *
 * @brief Board representation
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
     * @brief Board represented by multiple Bitboards
     */
    class Board
    {
        public:

            /**
             * @brief Constructor
             */
            constexpr Board() noexcept;

            /**
             * @brief Destructor
             */
            ~Board() = default;



            /**
             * @brief Returns the a square's index, based on the rank's and file's indexes
             *
             * @param [in] rankInex  : rank's index
             * @param [in] fileIndex : file's index
             * @return uint8_t : desired index
             */
            static inline constexpr uint8_t getSquareIndex(uint8_t& rankIndex, uint8_t& fileIndex) noexcept { return (8 * rankIndex) + fileIndex; };

            /**
             * @brief Returns a rank's index, base on a square's index
             *
             * @param [in] squareIndex : square's index
             * @return uint8_t : disired index
             */
            static inline constexpr uint8_t getRankIndex(uint8_t& squareIndex) noexcept { return squareIndex >> 3; };

            /**
             * @brief Returns a files's index, base on a square's index
             *
             * @param [in] squareIndex : square's index
             * @return uint8_t : disired index
             */
            static inline constexpr uint8_t getFileIndex(uint8_t& squareIndex) noexcept { return squareIndex & 7; };

        private:
            // Bitboards for each piece from each team
            conf::types::piecesBitboards piecesBB;

            // Bitboards for each piece from 1 team
            conf::types::teamPiecesBitboards whitePiecesBB;
            conf::types::teamPiecesBitboards blackPiecesBB;

            // Occupancy of the board (1 general + 1 for each team)
            conf::types::occupancy generalOccupancy;
            conf::types::occupancy whiteOccupancy;
            conf::types::occupancy blackOccupancy;
    };
} // namespace engine

#endif // BOARD_H_