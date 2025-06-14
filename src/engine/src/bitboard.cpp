/**
 * @file bitboard.cpp
 * @author Thibault THOMAS
 *
 * @brief Implementation of the Bitboard class
 */

/*----- Project Headers -----*/
#include "engine/include/bitboard.h"

/**
 * @namespace engine
 */
namespace engine
{
    constexpr Bitboard::Bitboard(const uint64_t & bb_) noexcept
        : bb(bb_)
        {}

    constexpr Bitboard::Bitboard(const Bitboard & bitboard_) noexcept
        : bb(bitboard_.bb)
        {}
} // namespace engine