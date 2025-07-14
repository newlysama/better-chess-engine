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

#include "engine/board/mask.h"
#include "engine/board/state.h"
#include "engine/core/enums.h"
#include "engine/core/types.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    /**
     * @brief Computes bitboards of attackable squares by a
     *        rook from squareIndex, based on a given occupancy.
     *
     * @param [in] squareIndex : the square we looking from
     * @param [in] occupancy   : the given occupancy
     * @return Bitboard : Exact attacks bitboard for that square and occupancy
     */
    inline Bitboard slidingAttackRook(int squareIndex, Bitboard occupancy) noexcept;

    /**
     * @brief Computes bitboards of attackable squares by a
     *        bishop from squareIndex, based on a given occupancy.
     *
     * @param [in] squareIndex : the square we looking from
     * @param [in] occupancy   : the given occupancy
     * @return Bitboard : Exact attacks bitboard for that square and occupancy
     */
    inline Bitboard slidingAttackBishop(int squareIndex, Bitboard occupancy) noexcept;

    /**
     * @brief Look for a valid magic bitboard for a rook on the given square.
     * @details
     * Generates all possible occupancies, tries random magics
     * and checks that no collision occurs
     *
     * @param [in] squareIndex : the square we looking from
     * @return Bitboard : the valid magic bitboard
     */
    inline Bitboard findMagicRook(int squareIndex) noexcept;

    /**
     * @brief Look for a valid magic bitboard for a bishop on the given square.
     * @details
     * Generates all possible occupancies, tries random magics
     * and checks that no collision occurs
     *
     * @param [in] squareIndex : the square we looking from
     * @return Bitboard : the valid magic bitboard
     */
    inline Bitboard findMagicBishop(int squareIndex) noexcept;

    /**
     * @brief Computed the necesarry shift for a rook on squareIndex.
     * @details
     * It is the number of bits you need to shift after multiplying,
     * so that we get an index < popcount(getRookMaskAt(squareIndex))
     *
     * @param [in] squareIndex : the square we looking from
     * @return uint8_t : the shift
     */
    inline uint8_t findShiftRook(int squareIndex) noexcept;

    /**
     * @brief Computed the necesarry shift for a bishop on squareIndex.
     * @details
     * It is the number of bits you need to shift after multiplying,
     * so that we get an index < popcount(getBishopMaskAt(squareIndex))
     *
     * @param [in] squareIndex : the square we looking from
     * @return uint8_t : the shift
     */
    inline uint8_t findShiftBishop(int squareIndex) noexcept;

    /**
     * @brief Main generation function.
     * @details
     * For each square :
     *  - computes findShiftRook & findShiftBishop
     *  - computes findMagicRook & findMagicBishop
     * Prints the values in stdout so that we can define them in magic_const.h
     */
    void initMagics() noexcept;

} // namespace engine::board

#endif // MAGICS_GENERATOR_H_