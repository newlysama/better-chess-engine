/**
 * @file      masks.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Implements masks generation.
 */

#include "engine/board/mask.h"

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace mask
     */
    namespace mask
    {
        inline constexpr conf::types::DiagonalMasks makeDiagonalsMasks() noexcept
        {
            conf::types::DiagonalMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                // file's index - rank's index + 7
                uint8_t maskIndex = (squareIndex % 8) + (squareIndex / 8) + 7;

                masks[maskIndex] |= board::Bitboard(1ULL << squareIndex);
            }

            return masks;
        }

        inline constexpr conf::types::DiagonalMasks makeAntiDiagonalsMasks() noexcept
        {
            conf::types::DiagonalMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                // file's index + rank's index
                uint8_t maskIndex = (squareIndex % 8) + (squareIndex / 8);

                masks[maskIndex] |= board::Bitboard(1ULL << squareIndex);
            }

            return masks;
        }

        template <conf::enums::Colors Color> inline constexpr conf::types::AttackMasks makePawnsAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                if constexpr (Color == conf::enums::Colors::WHITE)
                {
                    // North-West (<<7) excludes file A, North-East (<<9) excludes file H
                    masks[squareIndex] = ((mask << 7) & notAMask) | ((mask << 9) & notHMask);
                }
                else
                {
                    // South-East (>>7) excludes file H, South-West (>>9) excludes file A
                    masks[squareIndex] = ((mask >> 7) & notHMask) | ((mask >> 9) & notAMask);
                }
            }

            return masks;
        }

        template <conf::enums::Colors Color> inline constexpr conf::types::AttackMasks makePawnsPushesMasks() noexcept
        {
            conf::types::AttackMasks pushes{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                // always shift 8 (+1 vertically), or shift 16 (+2) from the 2nd rank
                if constexpr (Color == conf::enums::Colors::WHITE)
                {
                    pushes[squareIndex] = (mask << 8) | ((mask & ranksMasks[1]) << 16);
                }
                else
                {
                    pushes[squareIndex] = (mask >> 8) | ((mask & ranksMasks[6]) >> 16);
                }
            }

            return pushes;
        }

        inline constexpr conf::types::AttackMasks makeKnightsAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                masks[squareIndex] = ((mask << 17) & notAMask)    // +2 North / +1 West
                                     | ((mask << 15) & notHMask)  // +2 North / +1 East
                                     | ((mask << 10) & notGHMask) // +2 West  / +1 North
                                     | ((mask >> 6) & notGHMask)  // +2 West  / +1 South
                                     | ((mask << 6) & notABMask)  // +2 East  / +1 North
                                     | ((mask >> 10) & notABMask) // +2 East  / +1 South
                                     | ((mask >> 15) & notAMask)  // +2 South / +1 West
                                     | ((mask >> 17) & notHMask); // +2 South / +1 East
            }

            return masks;
        }

        inline constexpr conf::types::AttackMasks makeKingsAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                masks[squareIndex] = (mask << 8)                 // North
                                     | (mask >> 8)               // South
                                     | ((mask << 1) & notAMask)  // East
                                     | ((mask >> 1) & notHMask)  // West
                                     | ((mask << 9) & notAMask)  // North-East
                                     | ((mask << 7) & notHMask)  // North-West
                                     | ((mask >> 7) & notAMask)  // South-East
                                     | ((mask >> 9) & notHMask); // South-West
            }

            return masks;
        }

        inline constexpr conf::types::AttackMasks makeRooksAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                // On the same file, remove ranks 1&8
                board::Bitboard byFile = filesMasks[squareIndex] & notABMask;

                // On the same rank, remove files A&H
                board::Bitboard byRank = ranksMasks[squareIndex] & notGHMask;

                masks[squareIndex] = byFile | byRank;
            }

            return masks;
        }

        inline constexpr conf::types::AttackMasks makeBishopsAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                // Diagonal A1 ↗ H8, excluding A1 and H8
                board::Bitboard diag1 = diagonalsMasks[squareIndex] & ~A1Mask & ~H8Mask;

                // Anti-diag A8 ↘ H1, excluding A8 and H1
                board::Bitboard diag2 = antiDiagonalsMasks[squareIndex] & ~A8Mask & ~H1Mask;

                masks[squareIndex] = diag1 | diag2;
            }

            return masks;
        }
    } // namespace mask
} // namespace engine