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

#include <cstdint>

#include "engine/board/bitboard.h"

namespace engine::magics_generator
{
    /**
     * @brief Computes bitboards of attackable squares by a
     *        rook from square, based on a given occupancy.
     *
     * @param [in] square    : the square we looking from
     * @param [in] occupancy : the given occupancy
     * @return Bitboard : Exact attacks bitboard for that square and occupancy
     */
    inline board::Bitboard slidingAttackRook(const int square, const board::Bitboard occupancy) noexcept;

    /**
     * @brief Computes bitboards of attackable squares by a
     *        bishop from square, based on a given occupancy.
     *
     * @param [in] square    : the square we looking from
     * @param [in] occupancy : the given occupancy
     * @return Bitboard : Exact attacks bitboard for that square and occupancy
     */
    inline board::Bitboard slidingAttackBishop(const int square, const board::Bitboard occupancy) noexcept;

    /**
     * @brief Look for a valid magic bitboard for a rook on the given square.
     * @details
     * Generates all possible occupancies, tries random magics
     * and checks that no collision occurs
     *
     * @param [in] square : the square we looking from
     * @return Bitboard : the valid magic bitboard
     */
    inline board::Bitboard findMagicRook(const int square) noexcept;

    /**
     * @brief Look for a valid magic bitboard for a bishop on the given square.
     * @details
     * Generates all possible occupancies, tries random magics
     * and checks that no collision occurs
     *
     * @param [in] square : the square we looking from
     * @return Bitboard : the valid magic bitboard
     */
    inline board::Bitboard findMagicBishop(const int square) noexcept;

    /**
     * @brief Computed the necessary shift for a rook on square.
     * @details
     * It is the number of bits you need to shift after multiplying,
     * so that we get an index < popcount(getRookMaskAt(square))
     *
     * @param [in] square : the square we looking from
     * @return uint8_t : the shift
     */
    inline uint8_t findShiftRook(const int square) noexcept;

    /**
     * @brief Computed the necessary shift for a bishop on square.
     * @details
     * It is the number of bits you need to shift after multiplying,
     * so that we get an index < popcount(getBishopMaskAt(square))
     *
     * @param [in] square : the square we looking from
     * @return uint8_t : the shift
     */
    inline uint8_t findShiftBishop(const int square) noexcept;

    /**
     * @brief Main generation function.
     * @details
     * For each square :
     *  - computes findShiftRook & findShiftBishop
     *  - computes findMagicRook & findMagicBishop
     * Prints the values in stdout so that we can define them in magic_const.h
     */
    void initMagics() noexcept;

} // namespace engine::magics_generator

#endif // MAGICS_GENERATOR_H_