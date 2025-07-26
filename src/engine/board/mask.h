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

    // 'Not edge' masks
    inline constexpr Bitboard NOT_RANK_EDGES_MASK =
        ~(RANKS_MASKS[core::Ranks::RANK_1] | RANKS_MASKS[core::Ranks::RANK_8]);

    inline constexpr Bitboard NOT_FILE_EDGES_MASK =
        ~(FILES_MASKS[core::Files::FILE_A] | FILES_MASKS[core::Files::FILE_H]);

    /**
     * @brief Shifts a given Bitboard in a given direction.
     *
     * @param [in] bb : Bitboard to shift
     * @return the shifted bitboard
     */
    template <core::Directions D>
    constexpr Bitboard shiftDir(Bitboard b)
    {
        // clang-format off
        switch (D)
        {
        // ——— Sliding / King / Pawn ———
        case core::Directions::NORTH:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8]) << 8;

        case core::Directions::NORTH_EAST:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~FILES_MASKS[core::Files::FILE_H]) << 9;

        case core::Directions::EAST:
            return (b & ~FILES_MASKS[core::Files::FILE_H]) << 1;

        case core::Directions::SOUTH_EAST:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~FILES_MASKS[core::Files::FILE_H]) >> 7;

        case core::Directions::SOUTH:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1]) >> 8;

        case core::Directions::SOUTH_WEST:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~FILES_MASKS[core::Files::FILE_A]) >> 9;

        case core::Directions::WEST:
            return (b & ~FILES_MASKS[core::Files::FILE_A]) >> 1;

        case core::Directions::NORTH_WEST:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~FILES_MASKS[core::Files::FILE_A]) << 7;
        }
        // clang-format on

        return Bitboard{0ULL};
    }

    /**
     * @brief Special shiftDir to handle Pawn double pushes and Knight directions
     *
     * @param [in] b : Bitboard to shift
     * @return : the shifted Bitboard
     */
    template <core::SpecialDirections D>
    constexpr Bitboard shiftDir(Bitboard b) noexcept
    {
        // clang-format off
        switch (D)
        {
        //  ——— Pawns double push ———
        case core::SpecialDirections::NORTH_NORTH:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~RANKS_MASKS[core::Ranks::RANK_7]) << 16;

        case core::SpecialDirections::SOUTH_SOUTH:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~RANKS_MASKS[core::Ranks::RANK_2]) >> 16;

        // ——— Knights ———
        // two North + one East
        case core::SpecialDirections::NNE:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~RANKS_MASKS[core::Ranks::RANK_7]&
                    ~FILES_MASKS[core::Files::FILE_H]) << 17;

        // one North + two East
        case core::SpecialDirections::ENE:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~FILES_MASKS[core::Files::FILE_H] &
                    ~FILES_MASKS[core::Files::FILE_G]) << 10;

        // one South + two East
        case core::SpecialDirections::ESE:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~FILES_MASKS[core::Files::FILE_H] &
                    ~FILES_MASKS[core::Files::FILE_G]) >> 6;

        // two South + one East
        case core::SpecialDirections::SSE:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~RANKS_MASKS[core::Ranks::RANK_2] &
                    ~FILES_MASKS[core::Files::FILE_H]) >> 15;

        // two South + one West
        case core::SpecialDirections::SSW:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~RANKS_MASKS[core::Ranks::RANK_2] &
                    ~FILES_MASKS[core::Files::FILE_A]) >> 17;

        // one South + two West
        case core::SpecialDirections::WSW:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_1] & ~FILES_MASKS[core::Files::FILE_A] &
                    ~FILES_MASKS[core::Files::FILE_B]) >> 10;

        // one North + two West
        case core::SpecialDirections::WNW:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~FILES_MASKS[core::Files::FILE_A] &
                    ~FILES_MASKS[core::Files::FILE_B]) << 6;

        // two North + one West
        case core::SpecialDirections::NNW:
            return (b & ~RANKS_MASKS[core::Ranks::RANK_8] & ~RANKS_MASKS[core::Ranks::RANK_7] &
                  ~FILES_MASKS[core::Files::FILE_A]) << 15;
        }
        // clang-format on

        return Bitboard{0ULL};
    }

    /**
     * @brief Builds diagonals masks.
     * @return 1x15 array of diagonals masks
     */
    inline consteval core::DiagonalMasks initDiagonalsMasks() noexcept
    {
        core::DiagonalMasks masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // file's index - rank's index + 7 -> range [0,14]
            int file = squareIndex & 7;
            int rank = squareIndex >> 3;

            int maskIndex = file - rank + 7;

            masks[maskIndex] |= Bitboard{1ULL << squareIndex};
        }

        return masks;
    }
    inline constexpr core::DiagonalMasks DIAGONALS_MASKS = initDiagonalsMasks();

    /**
     * @brief Builds anti-diagonals masks.
     * @return 1x15 array of anti-diagonals masks
     */
    inline consteval core::DiagonalMasks initAntiDiagonalsMasks() noexcept
    {
        core::DiagonalMasks masks{};

        for (int squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            int file = squareIndex & 7;
            int rank = squareIndex >> 3;

            // file's index + rank's index -> range [0,14]
            int maskIndex = file + rank;

            masks[maskIndex] |= Bitboard{1ULL << squareIndex};
        }

        return masks;
    }
    inline constexpr core::DiagonalMasks ANTI_DIAGONALS_MASKS = initAntiDiagonalsMasks();

    /**
     * @brief Init ray masks
     *
     * @return 64x8 array of each mask (each direction for each square)
     */
    inline consteval core::RayMasks initRayMasks() noexcept
    {
        core::RayMasks rayMasks{};

        for (int square = 0; square < 64; square++)
        {
            Bitboard squareBB = Bitboard{1ULL << square};

            for (int direction = 0; direction < core::DIRECTIONS; direction++)
            {
                core::Directions dir = static_cast<core::Directions>(direction);
                Bitboard ray{};
                Bitboard b = squareBB;

                switch (dir)
                {
                case core::Directions::NORTH:
                    while (true)
                    {
                        b = shiftDir<core::Directions::NORTH>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::SOUTH:
                    while (true)
                    {
                        b = shiftDir<core::Directions::SOUTH>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::EAST:
                    while (true)
                    {
                        b = shiftDir<core::Directions::EAST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::WEST:
                    while (true)
                    {
                        b = shiftDir<core::Directions::WEST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::NORTH_EAST:
                    while (true)
                    {
                        b = shiftDir<core::Directions::NORTH_EAST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::NORTH_WEST:
                    while (true)
                    {
                        b = shiftDir<core::Directions::NORTH_WEST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::SOUTH_EAST:
                    while (true)
                    {
                        b = shiftDir<core::Directions::SOUTH_EAST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Directions::SOUTH_WEST:
                    while (true)
                    {
                        b = shiftDir<core::Directions::SOUTH_WEST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                default:
                    break;
                }

                rayMasks[square][direction] = ray;
            }
        }

        return rayMasks;
    }
    inline constexpr core::RayMasks RAY_MASKS = initRayMasks();

    /**
     * @brief Builds 'between 2 squares' masks.
     * @return 2x64 array of between masks
     */
    inline consteval core::BetweenMasks initBetweenMasks() noexcept
    {
        core::BetweenMasks masks{};

        for (int fromSquare = 0; fromSquare < 64; fromSquare++)
        {
            int fromRank = fromSquare / 8;
            int fromFile = fromSquare % 8;

            for (int toSquare = 0; toSquare < 64; toSquare++)
            {
                if (fromSquare == toSquare)
                    continue;

                int toRank = toSquare / 8;
                int toFile = toSquare % 8;

                int deltaRank = toRank - fromRank;
                int deltaFile = toFile - fromFile;

                int delta = 0;

                // horizontal
                if (deltaRank == 0 && deltaFile != 0)
                    delta = (deltaFile > 0 ? 1 : -1);

                // vertical
                else if (deltaFile == 0 && deltaRank != 0)
                    delta = (deltaRank > 0 ? 8 : -8);

                // diagonal '\'
                else if (deltaRank == deltaFile)
                    delta = (deltaRank > 0 ? 9 : -9);

                // anti-diagonal '/'
                else if (deltaRank == -deltaFile)
                    delta = (deltaRank > 0 ? 7 : -7);

                else
                    continue; // not aligned

                Bitboard betweenMask = 0;
                for (int square = fromSquare + delta; square != toSquare; square += delta)
                {
                    betweenMask.set(square);
                }

                masks[fromSquare][toSquare] = betweenMask;
            }
        }

        return masks;
    }
    inline constexpr core::BetweenMasks BETWEEN_MASKS = initBetweenMasks();

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
     * @brief Builds pawns double pushes masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <core::Colors Color>
    inline consteval core::BitboardTable initPawnDoublePushesMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            Bitboard squareBB = Bitboard{1ULL << squareIndex};
            if constexpr (Color == core::Colors::WHITE)
            {
                int rankIndex = squareIndex >> 3;

                if (rankIndex == 1) // Second rank
                {
                    masks[squareIndex] |= shiftDir<core::SpecialDirections::NORTH_NORTH>(squareBB);
                }
            }
            else
            {
                int rankIndex = squareIndex >> 3;
                if (rankIndex == 6) // 7th rank
                {
                    masks[squareIndex] |= shiftDir<core::SpecialDirections::SOUTH_SOUTH>(squareBB);
                }
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
            }
            else
            {
                masks[squareIndex] = shiftDir<core::Directions::SOUTH>(squareBB);
            }
        }
        return masks;
    }
    // clang-format off
    inline constexpr std::array<core::BitboardTable, core::Colors::COLORS> PAWN_PUSHES_MASKS{
        initPawnPushesMasks<core::Colors::WHITE>(),
        initPawnPushesMasks<core::Colors::BLACK>()
    };

    inline constexpr std::array<core::BitboardTable, core::Colors::COLORS> PAWN_DOUBLE_PUSHES_MASKS{
        initPawnDoublePushesMasks<core::Colors::WHITE>(),
        initPawnDoublePushesMasks<core::Colors::BLACK>()
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
            masks[squareIndex] =
                shiftDir<core::SpecialDirections::NNE>(squareBB) | shiftDir<core::SpecialDirections::ENE>(squareBB) |
                shiftDir<core::SpecialDirections::ESE>(squareBB) | shiftDir<core::SpecialDirections::SSE>(squareBB) |
                shiftDir<core::SpecialDirections::SSW>(squareBB) | shiftDir<core::SpecialDirections::WSW>(squareBB) |
                shiftDir<core::SpecialDirections::WNW>(squareBB) | shiftDir<core::SpecialDirections::NNW>(squareBB);
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
     * @details Excludes edges squares on diags/anti-diags that goes through each squareIndex
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

                Bitboard attacks = 0ULL;
                Bitboard occupancyMasked = occupancyBB & ROOK_RELEVANT_MASKS[squareIndex];

                // directions rook : N, S, E, W
                constexpr std::array<core::Directions, 4> dirs = {core::Directions::NORTH, core::Directions::SOUTH,
                                                                  core::Directions::EAST, core::Directions::WEST};

                for (auto dir : dirs)
                {
                    Bitboard fullRay = RAY_MASKS[squareIndex][dir];

                    // Check for blockers
                    Bitboard blockers = fullRay & occupancyMasked;
                    if (blockers.isEmpty() == false)
                    {
                        // Get the nearest block
                        int blockerSquare =
                            (dir == core::NORTH || dir == core::EAST) ? blockers.lsbIndex() : blockers.msbIndex();

                        // Get all squares between squareIndex and the blocker
                        Bitboard rayToBlock =
                            BETWEEN_MASKS[squareIndex][blockerSquare] | Bitboard(1ULL << blockerSquare);
                        attacks |= rayToBlock;
                    }
                    else
                    {
                        // No blocker = we take the full ray
                        attacks |= fullRay;
                    }
                }

                // Get magic index
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

                // directions Bishop
                constexpr std::array<core::Directions, 4> dirs = {
                    core::Directions::NORTH_WEST, core::Directions::NORTH_EAST, core::Directions::SOUTH_WEST,
                    core::Directions::SOUTH_EAST};

                for (auto dir : dirs)
                {
                    Bitboard fullRay = RAY_MASKS[squareIndex][dir];

                    // Check for blockers
                    Bitboard blockers = fullRay & occupancyMasked;
                    if (blockers.isEmpty() == false)
                    {
                        // Get the nearest block
                        int blockerSquare =
                            (dir == core::NORTH || dir == core::EAST) ? blockers.lsbIndex() : blockers.msbIndex();

                        // Get all squares between squareIndex and the blocker
                        Bitboard rayToBlock =
                            BETWEEN_MASKS[squareIndex][blockerSquare] | Bitboard(1ULL << blockerSquare);
                        attacks |= rayToBlock;
                    }
                    else
                    {
                        // No blocker = we take the full ray
                        attacks |= fullRay;
                    }
                }

                // Get magic index
                uint64_t magicIndex =
                    (occupancyBB.getData() * bishopMagics[squareIndex].getData()) >> bishopShifts[squareIndex];

                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const core::BishopAttacksTable BISHOP_ATTACKS_TABLE = initBishopAttacksTable();

    inline constexpr std::array<int, core::Castlings::CASTLINGS> CASTLING_TO_SQUARE = {6, 2, 62, 58};

    inline constexpr core::CastlingMasks CASTLING_BETWEEN_MASKS = {
        Bitboard{(1ULL << 5) | (1ULL << 6)}, Bitboard{(1ULL << 1) | (1ULL << 2) | (1ULL << 3)},
        Bitboard{(1ULL << 61) | (1ULL << 62)}, Bitboard{(1ULL << 57) | (1ULL << 58) | (1ULL << 59)}};

    inline constexpr core::CastlingMasks CASTLING_KING_PATH_MASKS = {
        Bitboard{(1ULL << 4) | (1ULL << 5) | (1ULL << 6)}, Bitboard{(1ULL << 4) | (1ULL << 5) | (1ULL << 6)},
        Bitboard{(1ULL << 60) | (1ULL << 61) | (1ULL << 62)}, Bitboard{(1ULL << 60) | (1ULL << 59) | (1ULL << 58)}};

} // namespace engine::board

#endif // MASKS_H_