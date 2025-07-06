/**
 * @file      magics_generator.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Magic numbers generation.
 */

#ifndef MAGICS_GENERATOR_H_
#define MAGICS_GENERATOR_H_

#include <array>
#include <cstdint>

#include "conf/enums.h"
#include "conf/types.h"
#include "engine/board/board.h"
#include "engine/board/mask.h"

/**
 * @namespace engine::board::magics_generator
 */
namespace engine::board::magics_generator
{
    // Generation routines (for offline use)
    inline Bitboard getRookMaskAt(int square) noexcept;
    inline Bitboard getBishopMaskAt(int square) noexcept;

    inline Bitboard slidingAttackRook(int square, Bitboard occ) noexcept;
    inline Bitboard slidingAttackBishop(int square, Bitboard occ) noexcept;

    inline Bitboard findMagicRook(int square) noexcept;
    inline Bitboard findMagicBishop(int square) noexcept;

    inline uint8_t findShiftRook(int square) noexcept;
    inline uint8_t findShiftBishop(int square) noexcept;

    // Orchestrator: compute and print tables for manual copy into this header
    void initMagics() noexcept;

} // namespace engine::board::magics_generator

#endif // MAGICS_GENERATOR_H_