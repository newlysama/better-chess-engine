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
        ~(RANKS_MASKS[core::Rank::RANK_1] | RANKS_MASKS[core::Rank::RANK_8]);

    inline constexpr Bitboard NOT_FILE_EDGES_MASK =
        ~(FILES_MASKS[core::File::FILE_A] | FILES_MASKS[core::File::FILE_H]);

    /**
     * @brief Shifts a given Bitboard in a given direction.
     *
     * @param [in] bb : Bitboard to shift
     * @return the shifted bitboard
     */
    template <core::Direction D>
    constexpr Bitboard shiftDir(Bitboard b)
    {
        // clang-format off
        switch (D)
        {
        // ——— Sliding / King / Pawn ———
        case core::Direction::NORTH:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8]) << 8;

        case core::Direction::NORTH_EAST:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~FILES_MASKS[core::File::FILE_H]) << 9;

        case core::Direction::EAST:
            return (b & ~FILES_MASKS[core::File::FILE_H]) << 1;

        case core::Direction::SOUTH_EAST:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~FILES_MASKS[core::File::FILE_H]) >> 7;

        case core::Direction::SOUTH:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1]) >> 8;

        case core::Direction::SOUTH_WEST:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~FILES_MASKS[core::File::FILE_A]) >> 9;

        case core::Direction::WEST:
            return (b & ~FILES_MASKS[core::File::FILE_A]) >> 1;

        case core::Direction::NORTH_WEST:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~FILES_MASKS[core::File::FILE_A]) << 7;
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
    template <core::SpecialDirection D>
    constexpr Bitboard shiftDir(Bitboard b) noexcept
    {
        // clang-format off
        switch (D)
        {
        //  ——— Pawns double push ———
        case core::SpecialDirection::NORTH_NORTH:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~RANKS_MASKS[core::Rank::RANK_7]) << 16;

        case core::SpecialDirection::SOUTH_SOUTH:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~RANKS_MASKS[core::Rank::RANK_2]) >> 16;

        // ——— Knights ———
        // two North + one East
        case core::SpecialDirection::NNE:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~RANKS_MASKS[core::Rank::RANK_7]&
                    ~FILES_MASKS[core::File::FILE_H]) << 17;

        // one North + two East
        case core::SpecialDirection::ENE:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~FILES_MASKS[core::File::FILE_H] &
                    ~FILES_MASKS[core::File::FILE_G]) << 10;

        // one South + two East
        case core::SpecialDirection::ESE:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~FILES_MASKS[core::File::FILE_H] &
                    ~FILES_MASKS[core::File::FILE_G]) >> 6;

        // two South + one East
        case core::SpecialDirection::SSE:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~RANKS_MASKS[core::Rank::RANK_2] &
                    ~FILES_MASKS[core::File::FILE_H]) >> 15;

        // two South + one West
        case core::SpecialDirection::SSW:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~RANKS_MASKS[core::Rank::RANK_2] &
                    ~FILES_MASKS[core::File::FILE_A]) >> 17;

        // one South + two West
        case core::SpecialDirection::WSW:
            return (b & ~RANKS_MASKS[core::Rank::RANK_1] & ~FILES_MASKS[core::File::FILE_A] &
                    ~FILES_MASKS[core::File::FILE_B]) >> 10;

        // one North + two West
        case core::SpecialDirection::WNW:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~FILES_MASKS[core::File::FILE_A] &
                    ~FILES_MASKS[core::File::FILE_B]) << 6;

        // two North + one West
        case core::SpecialDirection::NNW:
            return (b & ~RANKS_MASKS[core::Rank::RANK_8] & ~RANKS_MASKS[core::Rank::RANK_7] &
                  ~FILES_MASKS[core::File::FILE_A]) << 15;
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

        for (int square = 0; square < 64; square++)
        {
            // file's index - rank's index + 7 -> range [0,14]
            int file = square & 7;
            int rank = square >> 3;

            int maskIndex = file - rank + 7;

            masks[maskIndex] |= Bitboard{1ULL << square};
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

        for (int square = 0; square < 64; square++)
        {
            int file = square & 7;
            int rank = square >> 3;

            // file's index + rank's index -> range [0,14]
            int maskIndex = file + rank;

            masks[maskIndex] |= Bitboard{1ULL << square};
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

            for (int direction = 0; direction < core::N_DIRECTIONS; direction++)
            {
                core::Direction dir = static_cast<core::Direction>(direction);
                Bitboard ray{};
                Bitboard b = squareBB;

                switch (dir)
                {
                case core::Direction::NORTH:
                    while (true)
                    {
                        b = shiftDir<core::Direction::NORTH>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::SOUTH:
                    while (true)
                    {
                        b = shiftDir<core::Direction::SOUTH>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::EAST:
                    while (true)
                    {
                        b = shiftDir<core::Direction::EAST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::WEST:
                    while (true)
                    {
                        b = shiftDir<core::Direction::WEST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::NORTH_EAST:
                    while (true)
                    {
                        b = shiftDir<core::Direction::NORTH_EAST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::NORTH_WEST:
                    while (true)
                    {
                        b = shiftDir<core::Direction::NORTH_WEST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::SOUTH_EAST:
                    while (true)
                    {
                        b = shiftDir<core::Direction::SOUTH_EAST>(b);
                        if (b == 0)
                            break;
                        ray |= b;
                    }
                    break;

                case core::Direction::SOUTH_WEST:
                    while (true)
                    {
                        b = shiftDir<core::Direction::SOUTH_WEST>(b);
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
    template <core::Color Color>
    inline consteval core::BitboardTable initPawnAttacksMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int square = 0; square < 64; ++square)
        {
            Bitboard squareBB = Bitboard{1ULL << square};
            if constexpr (Color == core::Color::WHITE)
            {
                masks[square] =
                    shiftDir<core::Direction::NORTH_WEST>(squareBB) | shiftDir<core::Direction::NORTH_EAST>(squareBB);
            }
            else
            {
                masks[square] =
                    shiftDir<core::Direction::SOUTH_EAST>(squareBB) | shiftDir<core::Direction::SOUTH_WEST>(squareBB);
            }
        }
        return masks;
    }

    /**
     * @brief Builds pawns double pushes masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <core::Color Color>
    inline consteval core::BitboardTable initPawnDoublePushesMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int square = 0; square < 64; ++square)
        {
            Bitboard squareBB = Bitboard{1ULL << square};
            if constexpr (Color == core::Color::WHITE)
            {
                int rankIndex = square >> 3;

                if (rankIndex == 1) // Second rank
                {
                    masks[square] |= shiftDir<core::SpecialDirection::NORTH_NORTH>(squareBB);
                }
            }
            else
            {
                int rankIndex = square >> 3;
                if (rankIndex == 6) // 7th rank
                {
                    masks[square] |= shiftDir<core::SpecialDirection::SOUTH_SOUTH>(squareBB);
                }
            }
        }
        return masks;
    }

    /**
     * @brief Builds pawns pushes masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <core::Color Color>
    inline consteval core::BitboardTable initPawnPushesMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int square = 0; square < 64; ++square)
        {
            Bitboard squareBB = Bitboard{1ULL << square};
            if constexpr (Color == core::Color::WHITE)
            {
                masks[square] = shiftDir<core::Direction::NORTH>(squareBB);
            }
            else
            {
                masks[square] = shiftDir<core::Direction::SOUTH>(squareBB);
            }
        }
        return masks;
    }
    // clang-format off
    inline constexpr std::array<core::BitboardTable, core::Color::N_COLORS> PAWN_PUSHES_MASKS{
        initPawnPushesMasks<core::Color::WHITE>(),
        initPawnPushesMasks<core::Color::BLACK>()
    };

    inline constexpr std::array<core::BitboardTable, core::Color::N_COLORS> PAWN_DOUBLE_PUSHES_MASKS{
        initPawnDoublePushesMasks<core::Color::WHITE>(),
        initPawnDoublePushesMasks<core::Color::BLACK>()
    };

    inline constexpr std::array<core::BitboardTable, core::Color::N_COLORS> PAWN_CAPTURES_MASKS{
        initPawnAttacksMasks<core::Color::WHITE>(),
        initPawnAttacksMasks<core::Color::BLACK>()
    };
    // clang-format on

    /**
     * @brief Builds knights attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval core::BitboardTable initKnightAttacksMasks() noexcept
    {
        core::BitboardTable masks{};
        for (int square = 0; square < 64; ++square)
        {
            Bitboard squareBB = Bitboard{1ULL << square};
            masks[square] =
                shiftDir<core::SpecialDirection::NNE>(squareBB) | shiftDir<core::SpecialDirection::ENE>(squareBB) |
                shiftDir<core::SpecialDirection::ESE>(squareBB) | shiftDir<core::SpecialDirection::SSE>(squareBB) |
                shiftDir<core::SpecialDirection::SSW>(squareBB) | shiftDir<core::SpecialDirection::WSW>(squareBB) |
                shiftDir<core::SpecialDirection::WNW>(squareBB) | shiftDir<core::SpecialDirection::NNW>(squareBB);
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

        for (int square = 0; square < 64; square++)
        {
            Bitboard mask = Bitboard{1ULL << square};

            masks[square] = shiftDir<core::Direction::NORTH>(mask) | shiftDir<core::Direction::SOUTH>(mask) |
                            shiftDir<core::Direction::EAST>(mask) | shiftDir<core::Direction::WEST>(mask) |
                            shiftDir<core::Direction::NORTH_EAST>(mask) | shiftDir<core::Direction::NORTH_WEST>(mask) |
                            shiftDir<core::Direction::SOUTH_EAST>(mask) | shiftDir<core::Direction::SOUTH_WEST>(mask);
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

        for (int square = 0; square < 64; square++)
        {
            std::size_t file = square & 7;
            std::size_t rank = square >> 3;
            Bitboard mask = Bitboard{1ULL << square};

            // Same file and same rank without the square itself
            Bitboard byFile = FILES_MASKS[file] ^ mask;
            Bitboard byRank = RANKS_MASKS[rank] ^ mask;

            masks[square] = byFile | byRank;
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

        for (int square = 0; square < 64; square++)
        {
            std::size_t file = square & 7;
            std::size_t rank = square >> 3;

            std::size_t diag = file - rank + 7;
            std::size_t antiDiag = file + rank;

            Bitboard mask = Bitboard{1ULL << square};
            Bitboard diag1 = DIAGONALS_MASKS[diag] ^ mask;
            Bitboard diag2 = ANTI_DIAGONALS_MASKS[antiDiag] ^ mask;

            masks[square] = diag1 | diag2;
        }

        return masks;
    }
    inline constexpr core::BitboardTable BISHOP_ATTACKS_MASKS = initBishopAttacksMasks();

    /**
     * @brief Get the relevant occupancy masks for a rook.
     * @details
     * Excludes edges squares on ranks/files so that we only keep
     * those who can block a ray from each square
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval core::BitboardTable initRookRelevantMasks() noexcept
    {
        core::BitboardTable masks{};

        for (int square = 0; square < 64; square++)
        {
            // Get file / rank indexes
            int file = square & 7;
            int rank = square >> 3;

            // Exclude ranks 1 and 8
            Bitboard byFile = FILES_MASKS[file] & NOT_RANK_EDGES_MASK;

            // Exclude files A and H
            Bitboard byRank = RANKS_MASKS[rank] & NOT_FILE_EDGES_MASK;

            masks[square] = (byFile | byRank);
            masks[square].unset(square);
        }

        return masks;
    }
    inline constexpr core::BitboardTable ROOK_RELEVANT_MASKS = initRookRelevantMasks();

    /**
     * @brief Get the relevant occupancy masks for a bishop.
     * @details Excludes edges squares on diags/anti-diags that goes through each square
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval core::BitboardTable initBishopRelevantMasks() noexcept
    {
        core::BitboardTable masks{};

        for (int square = 0; square < 64; square++)
        {
            // Get file / rank indexes
            int file = square & 7;
            int rank = square >> 3;

            // diag = file - rank + 7  → index [0..14]
            int diag = file - rank + 7;

            // antidiag = file + rank → index [0..14]
            int antiDiag = file + rank;

            // Squares on the same diags, excluding edges
            Bitboard byDiag = DIAGONALS_MASKS[diag] & NOT_FILE_EDGES_MASK & NOT_RANK_EDGES_MASK;
            Bitboard byAntiDiag = ANTI_DIAGONALS_MASKS[antiDiag] & NOT_FILE_EDGES_MASK & NOT_RANK_EDGES_MASK;

            // remove square
            masks[square] = (byDiag | byAntiDiag);
            masks[square].unset(square);
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

        for (int square = 0; square < 64; square++)
        {
            int bits = 64 - rookShifts[square];
            uint64_t nEntries = 1ULL << bits;

            // Get relevant occupancy mask for this square
            Bitboard relevantMask = ROOK_RELEVANT_MASKS[square];
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
                Bitboard occupancyMasked = occupancyBB & ROOK_RELEVANT_MASKS[square];

                // directions rook : N, S, E, W
                constexpr std::array<core::Direction, 4> dirs = {core::Direction::NORTH, core::Direction::SOUTH,
                                                                 core::Direction::EAST, core::Direction::WEST};

                for (auto dir : dirs)
                {
                    Bitboard fullRay = RAY_MASKS[square][dir];

                    // Check for blockers
                    Bitboard blockers = fullRay & occupancyMasked;
                    if (blockers.isEmpty() == false)
                    {
                        // Get the nearest block
                        int blockerSquare =
                            (dir == core::NORTH || dir == core::EAST) ? blockers.lsbIndex() : blockers.msbIndex();

                        // Get all squares between square and the blocker
                        Bitboard rayToBlock = BETWEEN_MASKS[square][blockerSquare] | Bitboard(1ULL << blockerSquare);
                        attacks |= rayToBlock;
                    }
                    else
                    {
                        // No blocker = we take the full ray
                        attacks |= fullRay;
                    }
                }

                // Get magic index
                uint64_t magicIndex = (occupancyBB.getData() * rookMagics[square].getData()) >> rookShifts[square];

                table[square][magicIndex] = attacks;
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

        for (int square = 0; square < 64; square++)
        {
            int bits = 64 - bishopShifts[square];
            uint64_t nEntries = 1ULL << bits;

            // Get relevant occupancy mask for this square
            Bitboard relevantMask = BISHOP_RELEVANT_MASKS[square];
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
                Bitboard occupancyMasked = occupancyBB & BISHOP_RELEVANT_MASKS[square];

                // directions Bishop
                constexpr std::array<core::Direction, 4> dirs = {
                    core::Direction::NORTH_WEST, core::Direction::NORTH_EAST, core::Direction::SOUTH_WEST,
                    core::Direction::SOUTH_EAST};

                for (auto dir : dirs)
                {
                    Bitboard fullRay = RAY_MASKS[square][dir];

                    // Check for blockers
                    Bitboard blockers = fullRay & occupancyMasked;
                    if (blockers.isEmpty() == false)
                    {
                        // Get the nearest block
                        int blockerSquare =
                            (dir == core::NORTH || dir == core::EAST) ? blockers.lsbIndex() : blockers.msbIndex();

                        // Get all squares between square and the blocker
                        Bitboard rayToBlock = BETWEEN_MASKS[square][blockerSquare] | Bitboard(1ULL << blockerSquare);
                        attacks |= rayToBlock;
                    }
                    else
                    {
                        // No blocker = we take the full ray
                        attacks |= fullRay;
                    }
                }

                // Get magic index
                uint64_t magicIndex = (occupancyBB.getData() * bishopMagics[square].getData()) >> bishopShifts[square];

                table[square][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline const core::BishopAttacksTable BISHOP_ATTACKS_TABLE = initBishopAttacksTable();

    inline constexpr std::array<int, core::Castling::N_CASTLINGS> CASTLING_TO_SQUARE = {6, 2, 62, 58};

    inline constexpr core::CastlingMasks CASTLING_BETWEEN_MASKS = {
        Bitboard{(1ULL << 5) | (1ULL << 6)}, Bitboard{(1ULL << 1) | (1ULL << 2) | (1ULL << 3)},
        Bitboard{(1ULL << 61) | (1ULL << 62)}, Bitboard{(1ULL << 57) | (1ULL << 58) | (1ULL << 59)}};

    inline constexpr core::CastlingMasks CASTLING_KING_PATH_MASKS = {
        Bitboard{(1ULL << 4) | (1ULL << 5) | (1ULL << 6)}, Bitboard{(1ULL << 4) | (1ULL << 5) | (1ULL << 6)},
        Bitboard{(1ULL << 60) | (1ULL << 61) | (1ULL << 62)}, Bitboard{(1ULL << 60) | (1ULL << 59) | (1ULL << 58)}};

} // namespace engine::board

#endif // MASKS_H_