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

#include "engine/board/magic_const.h"
#include "engine/core/enums.h"
#include "engine/core/types.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    inline constexpr core::FilesMasks FILES_MASKS = {
        Bitboard{0x0101'0101'0101'0101ULL}, // file A
        Bitboard{0x0202'0202'0202'0202ULL}, // file B
        Bitboard{0x0404'0404'0404'0404ULL}, // file C
        Bitboard{0x0808'0808'0808'0808ULL}, // file D
        Bitboard{0x1010'1010'1010'1010ULL}, // file E
        Bitboard{0x2020'2020'2020'2020ULL}, // file F
        Bitboard{0x4040'4040'4040'4040ULL}, // file G
        Bitboard{0x8080'8080'8080'8080ULL}  // file H
    };

    inline constexpr core::RanksMasks RANKS_MASKS = {
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
     * @brief Shifts a given Bitboard in a given direction.
     *
     * @param [in] bb : Bitboard to shift
     * @return Bitboard : the shifted bitboard
     */
    template <core::Directions D>
    constexpr Bitboard shiftDir(Bitboard b)
    {
        switch (D)
        {
        // ——— Glissants & roi/pion ———
        case core::Directions::NORTH:
            return (b & ~RANK_8_MASK) << 8;
        case core::Directions::NORTH_EAST:
            return (b & ~RANK_8_MASK & ~FILE_H_MASK) << 9;
        case core::Directions::EAST:
            return (b & ~FILE_H_MASK) << 1;
        case core::Directions::SOUTH_EAST:
            return (b & ~RANK_1_MASK & ~FILE_H_MASK) >> 7;
        case core::Directions::SOUTH:
            return (b & ~RANK_1_MASK) >> 8;
        case core::Directions::SOUTH_WEST:
            return (b & ~RANK_1_MASK & ~FILE_A_MASK) >> 9;
        case core::Directions::WEST:
            return (b & ~FILE_A_MASK) >> 1;
        case core::Directions::NORTH_WEST:
            return (b & ~RANK_8_MASK & ~FILE_A_MASK) << 7;
        case core::Directions::NORTH_NORTH:
            return (b & ~RANK_8_MASK & ~RANK_7_MASK) << 16;
        case core::Directions::SOUTH_SOUTH:
            return (b & ~RANK_1_MASK & ~RANK_2_MASK) >> 16;

        // ——— Cavaliers ———
        // two North + one East
        case core::Directions::NNE:
            return (b & ~RANK_8_MASK & ~RANK_7_MASK & ~FILE_H_MASK) << 17;
        // one North + two East
        case core::Directions::ENE:
            return (b & ~RANK_8_MASK & ~FILE_H_MASK & ~FILE_G_MASK) << 10;
        // one South + two East
        case core::Directions::ESE:
            return (b & ~RANK_1_MASK & ~FILE_H_MASK & ~FILE_G_MASK) >> 6;
        // two South + one East
        case core::Directions::SSE:
            return (b & ~RANK_1_MASK & ~RANK_2_MASK & ~FILE_H_MASK) >> 15;
        // two South + one West
        case core::Directions::SSW:
            return (b & ~RANK_1_MASK & ~RANK_2_MASK & ~FILE_A_MASK) >> 17;
        // one South + two West
        case core::Directions::WSW:
            return (b & ~RANK_1_MASK & ~FILE_A_MASK & ~FILE_B_MASK) >> 10;
        // one North + two West
        case core::Directions::WNW:
            return (b & ~RANK_8_MASK & ~FILE_A_MASK & ~FILE_B_MASK) << 6;
        // two North + one West
        case core::Directions::NNW:
            return (b & ~RANK_8_MASK & ~RANK_7_MASK & ~FILE_A_MASK) << 15;
        }

        return 0ULL; // (jamais atteint)
    }

    /**
     * @brief Builds diagonals masks.
     * @return 1x15 array of attacks masks
     */
    inline consteval core::DiagonalMasks initDiagonalsMasks() noexcept
    {
        core::DiagonalMasks masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // file's index - rank's index + 7 -> range [0,14]
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            std::size_t maskIndex = file - rank + 7;

            masks[maskIndex] |= Bitboard{1ULL << squareIndex};
        }

        return masks;
    }
    inline constexpr core::DiagonalMasks DIAGONALS_MASKS = initDiagonalsMasks();

    /**
     * @brief Builds anti-diagonals masks.
     * @return 1x15 array of attacks masks
     */
    inline consteval core::DiagonalMasks initAntiDiagonalsMasks() noexcept
    {
        core::DiagonalMasks masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            // file's index + rank's index -> range [0,14]
            std::size_t maskIndex = file + rank;

            masks[maskIndex] |= Bitboard{1ULL << squareIndex};
        }

        return masks;
    }
    inline constexpr core::DiagonalMasks ANTI_DIAGONALS_MASKS = initAntiDiagonalsMasks();

    /**
     * @brief Builds pawns attacks masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <core::Colors Color>
    inline consteval core::BitboardTable initPawnAttacksMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            if constexpr (Color == core::Colors::WHITE)
            {
                masks[squareIndex] =
                    shiftDir<core::Directions::NORTH_WEST>(squareBB) | shiftDir<core::Directions::NORTH_EAST>(squareBB);
            }
            else
            {
                masks[squareIndex] =
                    shiftDir<core::Directions::SOUTH_EAST>(squareBB) | shiftDir<core::Directions::SOUTH_WEST>(squareBB);
            }
        }
        return masks;
    }

    /**
     * @brief Builds pawns pushes masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <core::Colors Color>
    inline consteval core::BitboardTable initPawnPushesMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            if constexpr (Color == core::Colors::WHITE)
            {
                masks[squareIndex] = shiftDir<core::Directions::NORTH>(squareBB);

                int rankIndex = squareIndex >> 3;

                if (rankIndex == 1) // Second rank
                {
                    masks[squareIndex] |= shiftDir<core::Directions::NORTH_NORTH>(squareBB);
                }
            }
            else
            {
                masks[squareIndex] = shiftDir<core::Directions::SOUTH>(squareBB);

                int rankIndex = squareIndex >> 3;
                if (rankIndex == 6) // 7th rank
                {
                    masks[squareIndex] |= shiftDir<core::Directions::SOUTH_SOUTH>(squareBB);
                }
            }
        }
        return masks;
    }
    // clang-format off
    inline constexpr std::array<core::BitboardTable, core::Colors::COLORS> PAWN_PUSHES_MASKS{
        initPawnPushesMasks<core::Colors::WHITE>(),
        initPawnPushesMasks<core::Colors::BLACK>()
    };

    inline constexpr std::array<core::BitboardTable, core::Colors::COLORS> PAWN_CAPTURES_MASKS{
        initPawnAttacksMasks<core::Colors::WHITE>(),
        initPawnAttacksMasks<core::Colors::BLACK>()
    };
    // clang-format on

    /**
     * @brief Builds knights attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval core::BitboardTable initKnightAttacksMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            masks[squareIndex] = shiftDir<core::Directions::NNE>(squareBB) | shiftDir<core::Directions::ENE>(squareBB) |
                                 shiftDir<core::Directions::ESE>(squareBB) | shiftDir<core::Directions::SSE>(squareBB) |
                                 shiftDir<core::Directions::SSW>(squareBB) | shiftDir<core::Directions::WSW>(squareBB) |
                                 shiftDir<core::Directions::WNW>(squareBB) | shiftDir<core::Directions::NNW>(squareBB);
        }
        return masks;
    }
    inline constexpr core::BitboardTable KNIGHT_ATTACKS_MASKS = initKnightAttacksMasks();

    /**
     * @brief Builds kings attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval core::BitboardTable initKingAttacksMasks() noexcept
    {
        core::BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            Bitboard mask = Bitboard{1ULL << squareIndex};

            masks[squareIndex] =
                shiftDir<core::Directions::NORTH>(mask) | shiftDir<core::Directions::SOUTH>(mask) |
                shiftDir<core::Directions::EAST>(mask) | shiftDir<core::Directions::WEST>(mask) |
                shiftDir<core::Directions::NORTH_EAST>(mask) | shiftDir<core::Directions::NORTH_WEST>(mask) |
                shiftDir<core::Directions::SOUTH_EAST>(mask) | shiftDir<core::Directions::SOUTH_WEST>(mask);
        }

        return masks;
    }
    inline constexpr core::BitboardTable KING_ATTACKS_MASKS = initKingAttacksMasks();

    /**
     * @brief Builds rooks attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval core::BitboardTable initRookAttacksMasks() noexcept
    {
        core::BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;
            Bitboard mask = Bitboard{1ULL << squareIndex};

            // Same file and same rank without the square itself
            Bitboard byFile = FILES_MASKS[file] ^ mask;
            Bitboard byRank = RANKS_MASKS[rank] ^ mask;

            masks[squareIndex] = byFile | byRank;
        }

        return masks;
    }
    inline constexpr core::BitboardTable ROOK_ATTACKS_MASKS = initRookAttacksMasks();

    /**
     * @brief Builds bishop attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval core::BitboardTable initBishopAttacksMasks() noexcept
    {
        core::BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            std::size_t diag = file - rank + 7;
            std::size_t antiDiag = file + rank;

            Bitboard mask = Bitboard{1ULL << squareIndex};
            Bitboard diag1 = DIAGONALS_MASKS[diag] ^ mask;
            Bitboard diag2 = ANTI_DIAGONALS_MASKS[antiDiag] ^ mask;

            masks[squareIndex] = diag1 | diag2;
        }

        return masks;
    }
    inline constexpr core::BitboardTable BISHOP_ATTACKS_MASKS = initBishopAttacksMasks();

    /**
     * @brief Get the relevant occupancy masks for a rook.
     * @details
     * Excludes edges squares on ranks/files so that we only keep
     * those who can block a ray from each squareIndex
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval core::BitboardTable initRookRelevantMasks() noexcept
    {
        core::BitboardTable masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // Get file / rank indexes
            int file = squareIndex & 7;
            int rank = squareIndex >> 3;

            // Exclude ranks 1 and 8
            Bitboard byFile = FILES_MASKS[file] & NOT_RANK_EDGES_MASK;

            // Exclude files A and H
            Bitboard byRank = RANKS_MASKS[rank] & NOT_FILE_EDGES_MASK;

            masks[squareIndex] = (byFile | byRank);
            masks[squareIndex].unset(squareIndex);
        }

        return masks;
    }
    inline constexpr core::BitboardTable ROOK_RELEVANT_MASKS = initRookRelevantMasks();

    /**
     * @brief Get the relevant occupancy masks for a bishop.
     * @details Excludes edges squares on diags/anti-diags that goes through each sqaureIndex
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval core::BitboardTable initBishopRelevantMasks() noexcept
    {
        core::BitboardTable masks{};

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
            masks[squareIndex] = (byDiag | byAntiDiag);
            masks[squareIndex].unset(squareIndex);
        }

        return masks;
    }
    inline constexpr core::BitboardTable BISHOP_RELEVANT_MASKS = initBishopRelevantMasks();

    /**
     * @brief Builds the rook's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x4096 array of rook attacks
     */
    inline constexpr core::RookAttacksTable initRookAttacksTable() noexcept
    {
        core::RookAttacksTable table{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            int bits = 64 - rookShifts[squareIndex];
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
                        occupancyBB.set(square);
                    }
                    tempMask.unset(square); // Clear LSB
                }

                Bitboard attacks = 0;
                Bitboard occupancyMasked = occupancyBB & ROOK_RELEVANT_MASKS[squareIndex];

                // North
                Bitboard ray = shiftDir<core::Directions::NORTH>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::NORTH>(ray);
                }

                // South
                ray = shiftDir<core::Directions::SOUTH>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::SOUTH>(ray);
                }

                // East
                ray = shiftDir<core::Directions::EAST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::EAST>(ray);
                }

                // West
                ray = shiftDir<core::Directions::WEST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::WEST>(ray);
                }

                // Store in table using magic index
                uint64_t magicIndex =
                    (occupancyBB.getData() * rookMagics[squareIndex].getData()) >> rookShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const core::RookAttacksTable ROOK_ATTACKS_TABLE = initRookAttacksTable();

    /**
     * @brief Builds the bishop's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x512 array of bishop attacks
     */
    inline constexpr core::BishopAttacksTable initBishopAttacksTable() noexcept
    {
        core::BishopAttacksTable table{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            int bits = 64 - bishopShifts[squareIndex];
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
                        occupancyBB.set(square);
                    }
                    tempMask.unset(square); // Clear LSB
                }
                Bitboard attacks = 0;
                Bitboard occupancyMasked = occupancyBB & BISHOP_RELEVANT_MASKS[squareIndex];

                // North-East
                Bitboard ray = shiftDir<core::Directions::NORTH_EAST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::NORTH_EAST>(ray);
                }

                // North-West
                ray = shiftDir<core::Directions::NORTH_WEST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::NORTH_WEST>(ray);
                }

                // South-East
                ray = shiftDir<core::Directions::SOUTH_EAST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::SOUTH_EAST>(ray);
                }

                // South-West
                ray = shiftDir<core::Directions::SOUTH_WEST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<core::Directions::SOUTH_WEST>(ray);
                }

                // Store in table using magic index
                uint64_t magicIndex =
                    (occupancyBB.getData() * bishopMagics[squareIndex].getData()) >> bishopShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const core::BishopAttacksTable BISHOP_ATTACKS_TABLE = initBishopAttacksTable();

    /**
     * @brief Builds 'between 2 squares' masks.
     * @return 2x64 array of attacks masks
     */
    inline consteval core::BetweenMasks initBetweenMasks() noexcept
    {
        core::BetweenMasks masks{};

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
    inline constexpr core::BetweenMasks BETWEEN_MASKS = initBetweenMasks();

    inline constexpr core::CastlingMasks CASTLING_MASKS = {
        Bitboard((1ULL << 5) | (1ULL << 6)), Bitboard((1ULL << 1) | (1ULL << 2) | (1ULL << 3)),
        Bitboard((1ULL << 61) | (1ULL << 62)), Bitboard((1ULL << 57) | (1ULL << 58) | (1ULL << 59))};

} // namespace engine::board

#endif // MASKS_H_