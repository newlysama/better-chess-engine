/**
 * @file board.cpp
 * @author Thibault THOMAS
 *
 * @brief Implementation of the Board class
 */

/*----- Project Headers -----*/
#include "engine/include/board.h"

/**
 * @namespace engine
 */
namespace engine
{
    constexpr Board::Board() noexcept
    {
        return;
    }

    inline constexpr uint8_t Board::getSquareIndex(uint8_t & rankIndex, uint8_t & fileIndex) noexcept
    {
        return (8 * rankIndex) + fileIndex;
    }
} // namespace engine