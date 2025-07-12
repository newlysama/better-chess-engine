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
#include <cstdlib>

#include "conf/enums.h"
#include "conf/types.h"
#include "engine/board/magic_const.h"

/**
 * @namespace engine::board::mask
 */
namespace engine::board::mask
{
    using namespace conf::enums;
    using namespace conf::types;

    inline constexpr FilesMasks FILES_MASKS = {
        Bitboard{0x0101'0101'0101'0101ULL}, // file A
        Bitboard{0x0202'0202'0202'0202ULL}, // file B
        Bitboard{0x0404'0404'0404'0404ULL}, // file C
        Bitboard{0x0808'0808'0808'0808ULL}, // file D
        Bitboard{0x1010'1010'1010'1010ULL}, // file E
        Bitboard{0x2020'2020'2020'2020ULL}, // file F
        Bitboard{0x4040'4040'4040'4040ULL}, // file G
        Bitboard{0x8080'8080'8080'8080ULL}  // file H
    };

    inline constexpr RanksMasks RANKS_MASKS = {
        Bitboard{0x0000'0000'0000'00FFULL}, // rank 1
        Bitboard{0x0000'0000'0000'FF00ULL}, // rank 2
        Bitboard{0x0000'0000'00FF'0000ULL}, // rank 3
        Bitboard{0x0000'0000'FF00'0000ULL}, // rank 4
        Bitboard{0x0000'00FF'0000'0000ULL}, // rank 5
        Bitboard{0x0000'FF00'0000'0000ULL}, // rank 6
        Bitboard{0x00FF'0000'0000'0000ULL}, // rank 7
        Bitboard{0xFF00'0000'0000'0000ULL}  // rank 8
    };

    // Ranks mask to allow clear access when we want to perform stuff on a specific rank
    inline constexpr Bitboard RANK_1_MASK = RANKS_MASKS[0];
    inline constexpr Bitboard RANK_2_MASK = RANKS_MASKS[1];
    inline constexpr Bitboard RANK_3_MASK = RANKS_MASKS[2];
    inline constexpr Bitboard RANK_4_MASK = RANKS_MASKS[3];
    inline constexpr Bitboard RANK_5_MASK = RANKS_MASKS[4];
    inline constexpr Bitboard RANK_6_MASK = RANKS_MASKS[5];
    inline constexpr Bitboard RANK_7_MASK = RANKS_MASKS[6];
    inline constexpr Bitboard RANK_8_MASK = RANKS_MASKS[7];

    // Files mask to allow clear access when we want to perform stuff on a specific file
    inline constexpr Bitboard FILE_A_MASK = FILES_MASKS[0];
    inline constexpr Bitboard FILE_B_MASK = FILES_MASKS[1];
    inline constexpr Bitboard FILE_C_MASK = FILES_MASKS[2];
    inline constexpr Bitboard FILE_D_MASK = FILES_MASKS[3];
    inline constexpr Bitboard FILE_E_MASK = FILES_MASKS[4];
    inline constexpr Bitboard FILE_F_MASK = FILES_MASKS[5];
    inline constexpr Bitboard FILE_G_MASK = FILES_MASKS[6];
    inline constexpr Bitboard FILE_H_MASK = FILES_MASKS[7];

    // 'Not file' masks
    inline constexpr Bitboard NOT_AB_MASK = ~(FILE_A_MASK | FILE_B_MASK);
    inline constexpr Bitboard NOT_GH_MASK = ~(FILE_G_MASK | FILE_H_MASK);

    // Corners masks
    inline constexpr Bitboard A1_MASK = FILE_A_MASK & RANK_1_MASK;
    inline constexpr Bitboard H8_MASK = FILE_H_MASK & RANK_8_MASK;
    inline constexpr Bitboard A8_MASK = FILE_A_MASK & RANK_8_MASK;
    inline constexpr Bitboard H1_MASK = FILE_H_MASK & RANK_1_MASK;

    // 'Not edge' masks
    inline constexpr Bitboard NOT_RANK_EDGES_MASK = ~(RANK_1_MASK | RANK_8_MASK);
    inline constexpr Bitboard NOT_FILE_EDGES_MASK = ~(FILE_A_MASK | FILE_H_MASK);

    /**
     * @brief Builds diagonals masks.
     * @return 1x15 array of attacks masks
     */
    inline consteval DiagonalMasks initDiagonalsMasks() noexcept
    {
        DiagonalMasks masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // file's index - rank's index + 7 -> range [0,14]
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            std::size_t maskIndex = file - rank + 7;

            masks[maskIndex] |= Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr DiagonalMasks DIAGONALS_MASKS = initDiagonalsMasks();

    /**
     * @brief Builds anti-diagonals masks.
     * @return 1x15 array of attacks masks
     */
    inline consteval DiagonalMasks initAntiDiagonalsMasks() noexcept
    {
        DiagonalMasks masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            // file's index + rank's index -> range [0,14]
            std::size_t maskIndex = file + rank;

            masks[maskIndex] |= Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr DiagonalMasks ANTI_DIAGONALS_MASKS = initAntiDiagonalsMasks();

    /**
     * @brief Builds pawns attacks masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <Colors Color>
    inline consteval BitboardTable initPawnAttacksMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            Bitboard mask = Bitboard(1ULL << squareIndex);

            if constexpr (Color == Colors::WHITE)
            {
                // North-West (<<7) excludes file A, North-East (<<9) excludes file H
                masks[squareIndex] = ((mask << 7) & ~FILE_A_MASK) | ((mask << 9) & ~FILE_H_MASK);
            }
            else
            {
                // South-East (>>7) excludes file H, South-West (>>9) excludes file A
                masks[squareIndex] = ((mask >> 7) & ~FILE_H_MASK) | ((mask >> 9) & ~FILE_A_MASK);
            }
        }

        return masks;
    }

    /**
     * @brief Builds pawns pushes masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <Colors Color>
    inline consteval BitboardTable initPawnPushesMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            Bitboard mask = Bitboard(1ULL << squareIndex);

            if constexpr (Color == Colors::WHITE)
            {
                // One step north or two steps from rank 2
                masks[squareIndex] = (mask << 8) | ((mask & RANKS_MASKS[1]) << 16);
            }
            else
            {
                // One step south or two steps from rank 7
                masks[squareIndex] = (mask >> 8) | ((mask & RANKS_MASKS[6]) >> 16);
            }
        }

        return masks;
    }
    // clang-format off
    inline constexpr std::array<BitboardTable, Colors::COLORS> PAWN_PUSHES_MASKS{
        initPawnPushesMasks<Colors::WHITE>(),
        initPawnPushesMasks<Colors::BLACK>()
    };

    inline constexpr std::array<BitboardTable, Colors::COLORS> PAWN_CAPTURES_MASKS{
        initPawnAttacksMasks<Colors::WHITE>(),
        initPawnAttacksMasks<Colors::BLACK>()
    };
    // clang-format on

    /**
     * @brief Builds knights attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval BitboardTable initKnightAttacksMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            Bitboard mask = Bitboard(1ULL << squareIndex);

            masks[squareIndex] = ((mask << 17) & ~FILE_A_MASK)  // +2 North / +1 West
                               | ((mask << 15) & ~FILE_H_MASK)  // +2 North / +1 East
                               | ((mask << 10) & NOT_GH_MASK)   // +1 North / +2 West
                               | ((mask >> 6) & NOT_GH_MASK)    // +1 South / +2 West
                               | ((mask << 6) & NOT_AB_MASK)    // +1 North / +2 East
                               | ((mask >> 10) & NOT_AB_MASK)   // +1 South / +2 East
                               | ((mask >> 15) & ~FILE_A_MASK)  // +2 South / +1 West
                               | ((mask >> 17) & ~FILE_H_MASK); // +2 South / +1 East
        }

        return masks;
    }
    inline constexpr BitboardTable KNIGHT_ATTACKS_MASKS = initKnightAttacksMasks();

    /**
     * @brief Builds kings attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval BitboardTable initKingAttacksMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            Bitboard mask = Bitboard(1ULL << squareIndex);

            masks[squareIndex] = (mask << 8)                   // North
                               | (mask >> 8)                   // South
                               | ((mask << 1) & ~FILE_A_MASK)  // East
                               | ((mask >> 1) & ~FILE_H_MASK)  // West
                               | ((mask << 9) & ~FILE_A_MASK)  // North-East
                               | ((mask << 7) & ~FILE_H_MASK)  // North-West
                               | ((mask >> 7) & ~FILE_A_MASK)  // South-East
                               | ((mask >> 9) & ~FILE_H_MASK); // South-West
        }

        return masks;
    }
    inline constexpr BitboardTable KING_ATTACKS_MASKS = initKingAttacksMasks();

    /**
     * @brief Builds rooks attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval BitboardTable initRookAttacksMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;
            Bitboard mask = Bitboard(1ULL << squareIndex);

            // Same file and same rank without the square itself
            Bitboard byFile = FILES_MASKS[file] ^ mask;
            Bitboard byRank = RANKS_MASKS[rank] ^ mask;

            masks[squareIndex] = byFile | byRank;
        }

        return masks;
    }
    inline constexpr BitboardTable ROOK_ATTACKS_MASKS = initRookAttacksMasks();

    /**
     * @brief Builds bishop attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval BitboardTable initBishopAttacksMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            std::size_t diag = file - rank + 7;
            std::size_t antiDiag = file + rank;

            Bitboard mask = Bitboard(1ULL << squareIndex);
            Bitboard diag1 = DIAGONALS_MASKS[diag] ^ mask;
            Bitboard diag2 = ANTI_DIAGONALS_MASKS[antiDiag] ^ mask;

            masks[squareIndex] = diag1 | diag2;
        }

        return masks;
    }
    inline constexpr BitboardTable BISHOP_ATTACKS_MASKS = initBishopAttacksMasks();

    /**
     * @brief Get the relevant occupancy masks for a rook.
     * @details
     * Excludes edges squares on ranks/files so that we only keep
     * those who can block a ray from each squareIndex
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval BitboardTable initRookRelevantMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // Get file / rank indexes
            int file = squareIndex & 7;
            int rank = squareIndex >> 3;

            // Exclude ranks 1 and 8
            Bitboard byFile = FILES_MASKS[file] & NOT_RANK_EDGES_MASK;

            // Exclude files A and H
            Bitboard byRank = RANKS_MASKS[rank] & NOT_FILE_EDGES_MASK;

            // remove squareIndex
            masks[squareIndex] = (byFile | byRank) & ~Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr BitboardTable ROOK_RELEVANT_MASKS = initRookRelevantMasks();

    /**
     * @brief Get the relevant occupancy masks for a bishop.
     * @details Excludes edges squares on diags/anti-diags that goes through each sqaureIndex
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval BitboardTable initBishopRelevantMasks() noexcept
    {
        BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // Get file / rank indexes
            int file = squareIndex & 7;
            int rank = squareIndex >> 3;

            // diag = file - rank + 7  → index [0..14]
            int diag = file - rank + 7;

            // antidiag = file + rank → index [0..14]
            int antiDiag = file + rank;

            // Squares on the same diags, excluding edges
            Bitboard byDiag = DIAGONALS_MASKS[diag] & NOT_FILE_EDGES_MASK & NOT_RANK_EDGES_MASK;
            Bitboard byAntiDiag = ANTI_DIAGONALS_MASKS[antiDiag] & NOT_FILE_EDGES_MASK & NOT_RANK_EDGES_MASK;

            // remove squareIndex
            masks[squareIndex] = (byDiag | byAntiDiag) ^ Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr BitboardTable BISHOP_RELEVANT_MASKS = initBishopRelevantMasks();

    /**
     * @brief Builds the rook's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x4096 array of rook attacks
     */
    inline constexpr rookAttacksTable initRookAttacksTable() noexcept
    {
        rookAttacksTable table{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            int bits = 64 - magic_const::rookShifts[squareIndex];
            uint64_t nEntries = 1ULL << bits;

            // Get relevant occupancy mask for this square
            Bitboard relevantMask = ROOK_RELEVANT_MASKS[squareIndex];
            int relevantBits = relevantMask.popCount();

            // Generate all possible occupancies
            for (uint64_t occupancy = 0; occupancy < nEntries; occupancy++)
            {
                // Convert occupancy pattern to actual bitboard
                Bitboard occupancyBB = 0;
                Bitboard tempMask = relevantMask;

                for (int i = 0; i < relevantBits; i++)
                {
                    int square = tempMask.lsbIndex();
                    if (occupancy & (1ULL << i))
                    {
                        occupancyBB |= Bitboard(1ULL << square);
                    }
                    tempMask = Bitboard(tempMask.getData() & (tempMask.getData() - 1)); // Clear LSB
                }

                // Calculate attack pattern using direct bitwise operations
                Bitboard attacks = 0;
                Bitboard occupancyMasked = occupancyBB & ROOK_RELEVANT_MASKS[squareIndex];

                // North
                Bitboard ray = Bitboard(1ULL << squareIndex) << 8;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = ray << 8;
                }

                // South
                ray = Bitboard(1ULL << squareIndex) >> 8;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = ray >> 8;
                }

                // East
                ray = (Bitboard(1ULL << squareIndex) & ~FILE_H_MASK) << 1;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_H_MASK) << 1;
                }

                // West
                ray = (Bitboard(1ULL << squareIndex) & ~FILE_A_MASK) >> 1;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_A_MASK) >> 1;
                }

                // Store in table using magic index
                uint64_t magicIndex = (occupancyBB.getData() * magic_const::rookMagics[squareIndex].getData()) >>
                                      magic_const::rookShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const rookAttacksTable ROOK_ATTACKS_TABLE = initRookAttacksTable();

    /**
     * @brief Builds the bishop's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x512 array of bishop attacks
     */
    inline constexpr bishopAttacksTable initBishopAttacksTable() noexcept
    {
        bishopAttacksTable table{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            int bits = 64 - magic_const::bishopShifts[squareIndex];
            uint64_t nEntries = 1ULL << bits;

            // Get relevant occupancy mask for this square
            Bitboard relevantMask = BISHOP_RELEVANT_MASKS[squareIndex];
            int relevantBits = relevantMask.popCount();

            // Generate all possible occupancies
            for (uint64_t occupancy = 0; occupancy < nEntries; occupancy++)
            {
                // Convert occupancy pattern to actual bitboard
                Bitboard occupancyBB = 0;
                Bitboard tempMask = relevantMask;

                for (int i = 0; i < relevantBits; i++)
                {
                    int square = tempMask.lsbIndex();
                    if (occupancy & (1ULL << i))
                    {
                        occupancyBB |= Bitboard(1ULL << square);
                    }
                    tempMask = Bitboard(tempMask.getData() & (tempMask.getData() - 1)); // Clear LSB
                }

                // Calculate attack pattern using direct bitwise operations (like in magics_generator.cpp)
                Bitboard attacks = 0;
                Bitboard occupancyMasked = occupancyBB & BISHOP_RELEVANT_MASKS[squareIndex];

                // North-East
                Bitboard ray = (Bitboard(1ULL << squareIndex) & ~FILE_H_MASK) << 9;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_H_MASK) << 9;
                }

                // North-West
                ray = (Bitboard(1ULL << squareIndex) & ~FILE_A_MASK) << 7;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_A_MASK) << 7;
                }

                // South-East
                ray = (Bitboard(1ULL << squareIndex) & ~FILE_H_MASK) >> 7;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_H_MASK) >> 7;
                }

                // South-West
                ray = (Bitboard(1ULL << squareIndex) & ~FILE_A_MASK) >> 9;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_A_MASK) >> 9;
                }

                // Store in table using magic index
                uint64_t magicIndex = (occupancyBB.getData() * magic_const::bishopMagics[squareIndex].getData()) >>
                                      magic_const::bishopShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const bishopAttacksTable BISHOP_ATTACKS_TABLE = initBishopAttacksTable();

    /**
     * @brief Builds 'between 2 squares' masks.
     * @return 2x64 array of attacks masks
     */
    inline consteval BetweenMasks initBetweenMasks() noexcept
    {
        BetweenMasks masks{};

        for (std::size_t fromSquare = 0; fromSquare < 64; fromSquare++)
        {
            for (std::size_t toSquare = 0; toSquare < 64; toSquare++)
            {
                if (fromSquare == toSquare)
                    continue;

                // If there is any, extract common line / diag
                Bitboard line = ROOK_ATTACKS_MASKS[fromSquare] & ROOK_ATTACKS_MASKS[toSquare];
                Bitboard diag = BISHOP_ATTACKS_MASKS[fromSquare] & BISHOP_ATTACKS_MASKS[toSquare];
                Bitboard mask = line | diag;

                // Keeps only squares strictly between 'fromSquare' and 'toSquare'
                Bitboard both = (Bitboard{1ULL << fromSquare} | Bitboard{1ULL << toSquare});

                masks[fromSquare][toSquare] = mask & ~both;
            }
        }

        return masks;
    }
    inline constexpr BetweenMasks BETWEEN_MASKS = initBetweenMasks();

    inline constexpr CastlingMasks CASTLING_MASKS = {
        Bitboard((1ULL << 5) | (1ULL << 6)), Bitboard((1ULL << 1) | (1ULL << 2) | (1ULL << 3)),
        Bitboard((1ULL << 61) | (1ULL << 62)), Bitboard((1ULL << 57) | (1ULL << 58) | (1ULL << 59))};

} // namespace engine::board::mask

#endif // MASKS_H_