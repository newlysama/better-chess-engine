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
    using namespace engine::core;

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
     * @brief Shifts a given Bitboard in a given direction.
     *
     * @param [in] bb : Bitboard to shift
     * @return Bitboard : the shifted bitboard
     */
    template <Directions Dir>
    inline constexpr Bitboard shiftDir(Bitboard bb) noexcept
    {
        return Dir == Directions::NORTH       ? Bitboard(bb << 8)
             : Dir == Directions::SOUTH       ? Bitboard(bb >> 8)
             : Dir == Directions::EAST        ? Bitboard((bb & ~FILE_H_MASK) << 1)
             : Dir == Directions::WEST        ? Bitboard((bb & ~FILE_A_MASK) >> 1)
             : Dir == Directions::NORTH_EAST  ? Bitboard((bb & ~FILE_H_MASK) << 9)
             : Dir == Directions::NORTH_WEST  ? Bitboard((bb & ~FILE_A_MASK) << 7)
             : Dir == Directions::SOUTH_EAST  ? Bitboard((bb & ~FILE_H_MASK) >> 7)
             : Dir == Directions::SOUTH_WEST  ? Bitboard((bb & ~FILE_A_MASK) >> 9)
             : Dir == Directions::NORTH_NORTH ? Bitboard(bb << 16)
             : Dir == Directions::SOUTH_SOUTH ? Bitboard(bb >> 16)
             : Dir == Directions::NNE         ? Bitboard((bb & ~FILE_H_MASK) << 17)
             : Dir == Directions::ENE         ? Bitboard((bb & ~(FILE_H_MASK | FILE_G_MASK)) << 10)
             : Dir == Directions::ESE         ? Bitboard((bb & ~(FILE_H_MASK | FILE_G_MASK)) >> 6)
             : Dir == Directions::SSE         ? Bitboard((bb & ~FILE_H_MASK) >> 15)
             : Dir == Directions::SSW         ? Bitboard((bb & ~FILE_A_MASK) >> 17)
             : Dir == Directions::WSW         ? Bitboard((bb & ~(FILE_A_MASK | FILE_B_MASK)) >> 10)
             : Dir == Directions::WNW         ? Bitboard((bb & ~(FILE_A_MASK | FILE_B_MASK)) << 6)
             : Dir == Directions::NNW         ? Bitboard((bb & ~FILE_A_MASK) << 15)
                                              : Bitboard(0ULL);
    }

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

            masks[maskIndex] |= Bitboard{1ULL << squareIndex};
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

            masks[maskIndex] |= Bitboard{1ULL << squareIndex};
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
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            if constexpr (Color == Colors::WHITE)
            {
                masks[squareIndex] =
                    shiftDir<Directions::NORTH_WEST>(squareBB) | shiftDir<Directions::NORTH_EAST>(squareBB);
            }
            else
            {
                masks[squareIndex] =
                    shiftDir<Directions::SOUTH_EAST>(squareBB) | shiftDir<Directions::SOUTH_WEST>(squareBB);
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
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            if constexpr (Color == Colors::WHITE)
            {
                masks[squareIndex] =
                    shiftDir<Directions::NORTH>(squareBB) | shiftDir<Directions::NORTH_NORTH>(squareBB);
            }
            else
            {
                masks[squareIndex] =
                    shiftDir<Directions::SOUTH>(squareBB) | shiftDir<Directions::SOUTH_SOUTH>(squareBB);
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
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            masks[squareIndex] = shiftDir<Directions::NNE>(squareBB) | shiftDir<Directions::ENE>(squareBB) |
                                 shiftDir<Directions::ESE>(squareBB) | shiftDir<Directions::SSE>(squareBB) |
                                 shiftDir<Directions::SSW>(squareBB) | shiftDir<Directions::WSW>(squareBB) |
                                 shiftDir<Directions::WNW>(squareBB) | shiftDir<Directions::NNW>(squareBB);
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
            Bitboard mask = Bitboard{1ULL << squareIndex};

            masks[squareIndex] = shiftDir<Directions::NORTH>(mask) | shiftDir<Directions::SOUTH>(mask) |
                                 shiftDir<Directions::EAST>(mask) | shiftDir<Directions::WEST>(mask) |
                                 shiftDir<Directions::NORTH_EAST>(mask) | shiftDir<Directions::NORTH_WEST>(mask) |
                                 shiftDir<Directions::SOUTH_EAST>(mask) | shiftDir<Directions::SOUTH_WEST>(mask);
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
            Bitboard mask = Bitboard{1ULL << squareIndex};

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

            Bitboard mask = Bitboard{1ULL << squareIndex};
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

            masks[squareIndex] = (byFile | byRank);
            masks[squareIndex].unset(squareIndex);
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
            masks[squareIndex] = (byDiag | byAntiDiag);
            masks[squareIndex].unset(squareIndex);
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
    inline constexpr RookAttacksTable initRookAttacksTable() noexcept
    {
        RookAttacksTable table{};

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
                Bitboard ray = shiftDir<Directions::NORTH>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::NORTH>(ray);
                }

                // South
                ray = shiftDir<Directions::SOUTH>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::SOUTH>(ray);
                }

                // East
                ray = shiftDir<Directions::EAST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::EAST>(ray);
                }

                // West
                ray = shiftDir<Directions::WEST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::WEST>(ray);
                }

                // Store in table using magic index
                uint64_t magicIndex =
                    (occupancyBB.getData() * rookMagics[squareIndex].getData()) >> rookShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const RookAttacksTable ROOK_ATTACKS_TABLE = initRookAttacksTable();

    /**
     * @brief Builds the bishop's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x512 array of bishop attacks
     */
    inline constexpr BishopAttacksTable initBishopAttacksTable() noexcept
    {
        BishopAttacksTable table{};

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
                Bitboard ray = shiftDir<Directions::NORTH_EAST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::NORTH_EAST>(ray);
                }

                // North-West
                ray = shiftDir<Directions::NORTH_WEST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::NORTH_WEST>(ray);
                }

                // South-East
                ray = shiftDir<Directions::SOUTH_EAST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::SOUTH_EAST>(ray);
                }

                // South-West
                ray = shiftDir<Directions::SOUTH_WEST>(Bitboard{1ULL << squareIndex});
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked).isEmpty() == false)
                        break;
                    ray = shiftDir<Directions::SOUTH_WEST>(ray);
                }

                // Store in table using magic index
                uint64_t magicIndex =
                    (occupancyBB.getData() * bishopMagics[squareIndex].getData()) >> bishopShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const BishopAttacksTable BISHOP_ATTACKS_TABLE = initBishopAttacksTable();

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

} // namespace engine::board

#endif // MASKS_H_