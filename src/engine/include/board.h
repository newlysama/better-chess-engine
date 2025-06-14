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
#include "engine/include/bitboard.h"

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @class Board
     * @brief Board represented by multiple bitboards
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
             * @brief Returns the index of a square, based on the rank's and file's indexes
             *
             * @param [in] rankInex  : rank's index
             * @param [in] fileIndex : file's index
             */
            static inline constexpr uint8_t getSquareIndex(uint8_t & rankIndex, uint8_t & fileIndex) noexcept;

        private:
            // Bitboards for each piece of each team
            std::unique_ptr<conf::types::pieceBitboards> pieceBB;
    };
} // namespace engine

#endif // BOARD_H_