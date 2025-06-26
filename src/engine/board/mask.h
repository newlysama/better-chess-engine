/**
 * @file      masks.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Masks for move generation.
 */

#ifndef MASKS_H_
#define MASKS_H_

#include <cstdint>

#include "conf/enums.h"
#include "conf/types.h"

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
        /**
         * @brief Files masks.
         */
        inline constexpr conf::types::FilesMasks filesMasks = {
            board::Bitboard{0x0101'0101'0101'0101ULL}, // file A
            board::Bitboard{0x0202'0202'0202'0202ULL}, // file B
            board::Bitboard{0x0404'0404'0404'0404ULL}, // file C
            board::Bitboard{0x0808'0808'0808'0808ULL}, // file D
            board::Bitboard{0x1010'1010'1010'1010ULL}, // file E
            board::Bitboard{0x2020'2020'2020'2020ULL}, // file F
            board::Bitboard{0x4040'4040'4040'4040ULL}, // file G
            board::Bitboard{0x8080'8080'8080'8080ULL}  // file H
        };

        /**
         * @brief Ranks masks.
         */
        inline constexpr conf::types::RanksMasks ranksMasks = {
            board::Bitboard{0x0000'0000'0000'00FFULL}, // rank 1
            board::Bitboard{0x0000'0000'0000'FF00ULL}, // rank 2
            board::Bitboard{0x0000'0000'00FF'0000ULL}, // rank 3
            board::Bitboard{0x0000'0000'FF00'0000ULL}, // rank 4
            board::Bitboard{0x0000'00FF'0000'0000ULL}, // rank 5
            board::Bitboard{0x0000'FF00'0000'0000ULL}, // rank 6
            board::Bitboard{0x00FF'0000'0000'0000ULL}, // rank 7
            board::Bitboard{0xFF00'0000'0000'0000ULL}  // rank 8
        };

        /**
         * @brief Corners masks.
         */
        inline constexpr board::Bitboard A1Mask = filesMasks[0] & ranksMasks[0];
        inline constexpr board::Bitboard H8Mask = filesMasks[7] & ranksMasks[7];
        inline constexpr board::Bitboard A8Mask = filesMasks[0] & ranksMasks[7];
        inline constexpr board::Bitboard H1Mask = filesMasks[7] & ranksMasks[0];

        /**
         * @brief 'Not edge' masks.
         */
        inline constexpr board::Bitboard notRankEdgesMask = ~(ranksMasks[0] | ranksMasks[7]);
        inline constexpr board::Bitboard notFileEdgesMask = ~(filesMasks[0] | filesMasks[7]);

        /**
         * @brief 'Not corner' masks.
         */
        inline constexpr board::Bitboard notA1Mask = ~A1Mask;
        inline constexpr board::Bitboard notH8Mask = ~H8Mask;
        inline constexpr board::Bitboard notA8Mask = ~A8Mask;
        inline constexpr board::Bitboard notH1Mask = ~H1Mask;

        /**
         * @brief 'Not file' masks.
         */
        inline constexpr board::Bitboard notAMask = ~filesMasks[0];
        inline constexpr board::Bitboard notBMask = ~filesMasks[1];
        inline constexpr board::Bitboard notGMask = ~filesMasks[6];
        inline constexpr board::Bitboard notHMask = ~filesMasks[7];
        inline constexpr board::Bitboard notABMask = ~(filesMasks[0] | filesMasks[1]);
        inline constexpr board::Bitboard notGHMask = ~(filesMasks[6] | filesMasks[7]);

        /**
         * @brief   Build diagonals masks.
         */
        inline constexpr conf::types::DiagonalMasks makeDiagonalsMasks() noexcept
        {
            conf::types::DiagonalMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                // file's index - rank's index + 7 -> range [0,14]
                uint8_t file = board::Board::getFileIndex(squareIndex);
                uint8_t rank = board::Board::getRankIndex(squareIndex);

                uint8_t maskIndex = static_cast<uint8_t>(static_cast<int>(file) - static_cast<int>(rank) + 7);

                masks[maskIndex] |= board::Bitboard(1ULL << squareIndex);
            }

            return masks;
        }

        /**
         * @brief Builds anti-diagonals masks.
         */
        inline constexpr conf::types::DiagonalMasks makeAntiDiagonalsMasks() noexcept
        {
            conf::types::DiagonalMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                uint8_t file = board::Board::getFileIndex(squareIndex);
                uint8_t rank = board::Board::getRankIndex(squareIndex);

                // file's index + rank's index -> range [0,14]
                uint8_t maskIndex = file + rank;

                masks[maskIndex] |= board::Bitboard(1ULL << squareIndex);
            }

            return masks;
        }

        /**
         * @brief Builds pawns attacks masks, depending on the color.
         */
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

        /**
         * @brief Builds pawns pushes masks, depending on the color.
         */
        template <conf::enums::Colors Color> inline constexpr conf::types::AttackMasks makePawnsPushesMasks() noexcept
        {
            conf::types::AttackMasks pushes{};

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                if constexpr (Color == conf::enums::Colors::WHITE)
                {
                    // One step north or two steps from rank 2
                    pushes[squareIndex] = (mask << 8) | ((mask & ranksMasks[1]) << 16);
                }
                else
                {
                    // One step south or two steps from rank 7
                    pushes[squareIndex] = (mask >> 8) | ((mask & ranksMasks[6]) >> 16);
                }
            }

            return pushes;
        }

        /**
         * @brief Builds knights attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeKnightsAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                masks[squareIndex] = ((mask << 17) & notAMask)    // +2 North / +1 West
                                     | ((mask << 15) & notHMask)  // +2 North / +1 East
                                     | ((mask << 10) & notGHMask) // +1 North / +2 West
                                     | ((mask >> 6) & notGHMask)  // +1 South / +2 West
                                     | ((mask << 6) & notABMask)  // +1 North / +2 East
                                     | ((mask >> 10) & notABMask) // +1 South / +2 East
                                     | ((mask >> 15) & notAMask)  // +2 South / +1 West
                                     | ((mask >> 17) & notHMask); // +2 South / +1 East
            }

            return masks;
        }

        /**
         * @brief Builds kings attacks masks.
         */
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

        /**
         * @brief Builds rooks attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeRooksAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                uint8_t file = board::Board::getFileIndex(squareIndex);
                uint8_t rank = board::Board::getRankIndex(squareIndex);
                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

                // Same file and same rank without the square itself
                board::Bitboard byFile = filesMasks[file] ^ mask;
                board::Bitboard byRank = ranksMasks[rank] ^ mask;

                masks[squareIndex] = byFile | byRank;
            }

            return masks;
        }

        /**
         * @brief Builds bishop attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeBishopsAttacksMasks() noexcept
        {
            conf::types::AttackMasks masks{};

            for (uint8_t squareIndex = 0; squareIndex < 64; ++squareIndex)
            {
                uint8_t file = board::Board::getFileIndex(squareIndex);
                uint8_t rank = board::Board::getRankIndex(squareIndex);

                uint8_t diag = static_cast<uint8_t>(static_cast<int>(file) - static_cast<int>(rank) + 7);
                uint8_t antiDiag = file + rank;

                board::Bitboard mask = board::Bitboard(1ULL << squareIndex);
                board::Bitboard diag1 = diagonalsMasks[diag] ^ mask;
                board::Bitboard diag2 = antiDiagonalsMasks[antiDiag] ^ mask;

                masks[squareIndex] = diag1 | diag2;
            }

            /**
             * @brief Diagonals masks.
             */
            inline constexpr conf::types::DiagonalMasks diagonalsMasks = makeDiagonalsMasks();

            /**
             * @brief Anti-diagonals masks.
             */
            inline constexpr conf::types::DiagonalMasks antiDiagonalsMasks = makeAntiDiagonalsMasks();

            /**
             * @brief White pawns attacks masks.
             */
            inline constexpr conf::types::AttackMasks whitePawnsAttacksMasks =
                makePawnsAttacksMasks<conf::enums::Colors::WHITE>();

            /**
             * @brief Back pawns attacks masks.
             */
            inline constexpr conf::types::AttackMasks blackPawnsAttacksMasks =
                makePawnsAttacksMasks<conf::enums::Colors::BLACK>();

            /**
             * @brief White pawns pushes masks.
             */
            inline constexpr conf::types::AttackMasks whitePawnsPushesMasks =
                makePawnsPushesMasks<conf::enums::Colors::WHITE>();

            /**
             * @brief Black pawns pushes masks.
             */
            inline constexpr conf::types::AttackMasks blackPawnsPushesMasks =
                makePawnsPushesMasks<conf::enums::Colors::BLACK>();

            /**
             * @brief Knights attacks masks.
             */
            inline constexpr conf::types::AttackMasks knightsAttacksMasks = makeKnightsAttacksMasks();

            /**
             * @brief Kings attacks masks.
             */
            inline constexpr conf::types::AttackMasks kingsAttacksMasks = makeKingsAttacksMasks();

            /**
             * @brief Rooks attacks masks.
             */
            inline constexpr conf::types::AttackMasks rooksAttacksMasks = makeRooksAttacksMasks();

            /**
             * @brief Bishops attacks masks.
             */
            inline constexpr conf::types::AttackMasks bishopAttacksMasks = makeBishopsAttacksMasks();

            /**
             * @brief Castling rights masks.
             */
            inline constexpr conf::types::CastlingMasks castlingMasks = {
                board::Bitboard((1ULL << 5) | (1ULL << 6)), board::Bitboard((1ULL << 1) | (1ULL << 2) | (1ULL << 3)),
                board::Bitboard((1ULL << 61) | (1ULL << 62)),
                board::Bitboard((1ULL << 57) | (1ULL << 58) | (1ULL << 59))};
        } // namespace mask
    } // namespace engine

#endif // MASKS_H_