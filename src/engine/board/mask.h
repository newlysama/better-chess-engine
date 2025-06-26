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
         * @brief Build diagonals masks.
         */
        inline constexpr conf::types::DiagonalMasks makeDiagonalsMasks() noexcept;

        /**
         * @brief Builds anti-diagonals masks.
         */
        inline constexpr conf::types::DiagonalMasks makeAntiDiagonalsMasks() noexcept;

        /**
         * @brief Builds pawns attacks masks, depending on the color.
         */
        template <conf::enums::Colors Color> inline constexpr conf::types::AttackMasks makePawnsAttacksMasks() noexcept;

        /**
         * @brief Builds pawns pushes masks, depending on the color.
         */
        template <conf::enums::Colors Color> inline constexpr conf::types::AttackMasks makePawnsPushesMasks() noexcept;

        /**
         * @brief Builds knights attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeKnightsAttacksMasks() noexcept;

        /**
         * @brief Builds kings attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeKingsAttacksMasks() noexcept;

        /**
         * @brief Builds rooks attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeRooksAttacksMasks() noexcept;

        /**
         * @brief Builds bishop attacks masks.
         */
        inline constexpr conf::types::AttackMasks makeBishopsAttacksMasks() noexcept;

        inline constexpr conf::types::DiagonalMasks diagonalsMasks = makeDiagonalsMasks();
        inline constexpr conf::types::DiagonalMasks antiDiagonalsMasks = makeAntiDiagonalsMasks();

        inline constexpr conf::types::AttackMasks whitePawnsAttacksMasks =
            makePawnsAttacksMasks<conf::enums::Colors::WHITE>();

        inline constexpr conf::types::AttackMasks blackPawnsAttacksMasks =
            makePawnsAttacksMasks<conf::enums::Colors::BLACK>();

        inline constexpr conf::types::AttackMasks whitePawnsPushesMasks =
            makePawnsPushesMasks<conf::enums::Colors::WHITE>();

        inline constexpr conf::types::AttackMasks blackPawnsPushesMasks =
            makePawnsPushesMasks<conf::enums::Colors::BLACK>();

        inline constexpr conf::types::AttackMasks knightsAttacksMasks = makeKnightsAttacksMasks();
        inline constexpr conf::types::AttackMasks kingsAttacksMasks = makeKingsAttacksMasks();

        inline constexpr conf::types::AttackMasks rooksAttacksMasks = makeRooksAttacksMasks();
        inline constexpr conf::types::AttackMasks bishopAttacksMasks = makeBishopsAttacksMasks();

        /**
         * @brief Castling rights masks.
         */
        inline constexpr conf::types::CastlingMasks castlingMasks = {
            board::Bitboard((1ULL << 5) | (1ULL << 6)), board::Bitboard((1ULL << 1) | (1ULL << 2) | (1ULL << 3)),
            board::Bitboard((1ULL << 61) | (1ULL << 62)), board::Bitboard((1ULL << 57) | (1ULL << 58) | (1ULL << 59))};
    } // namespace mask
} // namespace engine

#endif // MASKS_H_