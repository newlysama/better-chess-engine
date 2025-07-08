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
    inline constexpr conf::types::FilesMasks FILES_MASKS = {
        board::Bitboard{0x0101'0101'0101'0101ULL}, // file A
        board::Bitboard{0x0202'0202'0202'0202ULL}, // file B
        board::Bitboard{0x0404'0404'0404'0404ULL}, // file C
        board::Bitboard{0x0808'0808'0808'0808ULL}, // file D
        board::Bitboard{0x1010'1010'1010'1010ULL}, // file E
        board::Bitboard{0x2020'2020'2020'2020ULL}, // file F
        board::Bitboard{0x4040'4040'4040'4040ULL}, // file G
        board::Bitboard{0x8080'8080'8080'8080ULL}  // file H
    };

    inline constexpr conf::types::RanksMasks RANKS_MASKS = {
        board::Bitboard{0x0000'0000'0000'00FFULL}, // rank 1
        board::Bitboard{0x0000'0000'0000'FF00ULL}, // rank 2
        board::Bitboard{0x0000'0000'00FF'0000ULL}, // rank 3
        board::Bitboard{0x0000'0000'FF00'0000ULL}, // rank 4
        board::Bitboard{0x0000'00FF'0000'0000ULL}, // rank 5
        board::Bitboard{0x0000'FF00'0000'0000ULL}, // rank 6
        board::Bitboard{0x00FF'0000'0000'0000ULL}, // rank 7
        board::Bitboard{0xFF00'0000'0000'0000ULL}  // rank 8
    };

    // Ranks mask to allow clear access when we want to perform stuff on a specific rank
    inline constexpr board::Bitboard RANK_1_MASK = RANKS_MASKS[0];
    inline constexpr board::Bitboard RANK_2_MASK = RANKS_MASKS[1];
    inline constexpr board::Bitboard RANK_3_MASK = RANKS_MASKS[2];
    inline constexpr board::Bitboard RANK_4_MASK = RANKS_MASKS[3];
    inline constexpr board::Bitboard RANK_5_MASK = RANKS_MASKS[4];
    inline constexpr board::Bitboard RANK_6_MASK = RANKS_MASKS[5];
    inline constexpr board::Bitboard RANK_7_MASK = RANKS_MASKS[6];
    inline constexpr board::Bitboard RANK_8_MASK = RANKS_MASKS[7];

    // Files mask to allow clear access when we want to perform stuff on a specific file
    inline constexpr board::Bitboard FILE_A_MASK = FILES_MASKS[0];
    inline constexpr board::Bitboard FILE_B_MASK = FILES_MASKS[1];
    inline constexpr board::Bitboard FILE_C_MASK = FILES_MASKS[2];
    inline constexpr board::Bitboard FILE_D_MASK = FILES_MASKS[3];
    inline constexpr board::Bitboard FILE_E_MASK = FILES_MASKS[4];
    inline constexpr board::Bitboard FILE_F_MASK = FILES_MASKS[5];
    inline constexpr board::Bitboard FILE_G_MASK = FILES_MASKS[6];
    inline constexpr board::Bitboard FILE_H_MASK = FILES_MASKS[7];

    // 'Not file' masks
    inline constexpr board::Bitboard NOT_AB_MASK = ~(FILE_A_MASK | FILE_B_MASK);
    inline constexpr board::Bitboard NOT_GH_MASK = ~(FILE_G_MASK | FILE_H_MASK);

    // Corners masks
    inline constexpr board::Bitboard A1_MASK = FILE_A_MASK & RANK_1_MASK;
    inline constexpr board::Bitboard H8_MASK = FILE_H_MASK & RANK_8_MASK;
    inline constexpr board::Bitboard A8_MASK = FILE_A_MASK & RANK_8_MASK;
    inline constexpr board::Bitboard H1_MASK = FILE_H_MASK & RANK_1_MASK;

    // 'Not edge' masks
    inline constexpr board::Bitboard NOT_RANK_EDGES_MASK = ~(RANK_1_MASK | RANK_8_MASK);
    inline constexpr board::Bitboard NOT_FILE_EDGES_MASK = ~(FILE_A_MASK | FILE_H_MASK);

    /**
     * @brief Builds diagonals masks.
     * @return 1x15 array of attacks masks
     */
    inline consteval conf::types::DiagonalMasks initDiagonalsMasks() noexcept
    {
        conf::types::DiagonalMasks masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // file's index - rank's index + 7 -> range [0,14]
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            std::size_t maskIndex = static_cast<std::size_t>(static_cast<int>(file) - static_cast<int>(rank) + 7);

            masks[maskIndex] |= board::Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr conf::types::DiagonalMasks DIAGONALS_MASKS = initDiagonalsMasks();

    /**
     * @brief Builds anti-diagonals masks.
     * @return 1x15 array of attacks masks
     */
    inline consteval conf::types::DiagonalMasks initAntiDiagonalsMasks() noexcept
    {
        conf::types::DiagonalMasks masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            // file's index + rank's index -> range [0,14]
            std::size_t maskIndex = file + rank;

            masks[maskIndex] |= board::Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr conf::types::DiagonalMasks ANTI_DIAGONALS_MASKS = initAntiDiagonalsMasks();

    /**
     * @brief Builds pawns attacks masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <conf::enums::Colors Color>
    inline consteval conf::types::BitboardTable initPawnAttacksMasks() noexcept
    {
        conf::types::BitboardTable masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

            if constexpr (Color == conf::enums::Colors::WHITE)
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
    inline constexpr conf::types::BitboardTable WHITE_PAWN_ATTACKS_MASKS =
        initPawnAttacksMasks<conf::enums::Colors::WHITE>();
    inline constexpr conf::types::BitboardTable BLACK_PAWN_ATTACK_MASKS =
        initPawnAttacksMasks<conf::enums::Colors::BLACK>();

    /**
     * @brief Builds pawns pushes masks, depending on the color.
     * @return 1x64 array of attacks masks
     */
    template <conf::enums::Colors Color>
    inline consteval conf::types::BitboardTable initPawnPushesMasks() noexcept
    {
        conf::types::BitboardTable masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

            if constexpr (Color == conf::enums::Colors::WHITE)
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
    inline constexpr conf::types::BitboardTable WHITE_PAWN_PUSHES_MASKS =
        initPawnPushesMasks<conf::enums::Colors::WHITE>();
    inline constexpr conf::types::BitboardTable BLACK_PAWN_PUSHES_MASKS =
        initPawnPushesMasks<conf::enums::Colors::BLACK>();

    /**
     * @brief Builds knights attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval conf::types::BitboardTable initKnightAttacksMasks() noexcept
    {
        conf::types::BitboardTable masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

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
    inline constexpr conf::types::BitboardTable KNIGHT_ATTACKS_MASKS = initKnightAttacksMasks();

    /**
     * @brief Builds kings attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval conf::types::BitboardTable initKingAttacksMasks() noexcept
    {
        conf::types::BitboardTable masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

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
    inline constexpr conf::types::BitboardTable KING_ATTACKS_MASKS = initKingAttacksMasks();

    /**
     * @brief Builds rooks attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval conf::types::BitboardTable initRookAttacksMasks() noexcept
    {
        conf::types::BitboardTable masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;
            board::Bitboard mask = board::Bitboard(1ULL << squareIndex);

            // Same file and same rank without the square itself
            board::Bitboard byFile = FILES_MASKS[file] ^ mask;
            board::Bitboard byRank = RANKS_MASKS[rank] ^ mask;

            masks[squareIndex] = byFile | byRank;
        }

        return masks;
    }
    inline constexpr conf::types::BitboardTable ROOK_ATTACKS_MASKS = initRookAttacksMasks();

    /**
     * @brief Builds bishop attacks masks.
     * @return 1x64 array of attacks masks
     */
    inline consteval conf::types::BitboardTable initBishopAttacksMasks() noexcept
    {
        conf::types::BitboardTable masks{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            std::size_t file = squareIndex & 7;
            std::size_t rank = squareIndex >> 3;

            std::size_t diag = file - rank + 7;
            std::size_t antiDiag = file + rank;

            board::Bitboard mask = board::Bitboard(1ULL << squareIndex);
            board::Bitboard diag1 = DIAGONALS_MASKS[diag] ^ mask;
            board::Bitboard diag2 = ANTI_DIAGONALS_MASKS[antiDiag] ^ mask;

            masks[squareIndex] = diag1 | diag2;
        }

        return masks;
    }
    inline constexpr conf::types::BitboardTable BISHOP_ATTACKS_MASKS = initBishopAttacksMasks();

    /**
     * @brief Get the relevant occupancy masks for a rook.
     * @details
     * Excludes edges squares on ranks/files so that we only keep
     * those who can block a ray from each squareIndex
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval conf::types::BitboardTable initRookRelevantMasks() noexcept
    {
        conf::types::BitboardTable masks{};
        int file;
        int rank;

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // Get file / rank indexes
            file = squareIndex & 7;
            rank = squareIndex >> 3;

            // Exclude ranks 1 and 8
            Bitboard byFile = FILES_MASKS[file] & NOT_RANK_EDGES_MASK;

            // Exclude files A and H
            Bitboard byRank = RANKS_MASKS[rank] & NOT_FILE_EDGES_MASK;

            // remove squareIndex
            masks[squareIndex] = (byFile | byRank) & ~Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr conf::types::BitboardTable ROOK_RELEVANT_MASKS = initRookRelevantMasks();

    /**
     * @brief Get the relevant occupancy masks for a bishop.
     * @details Excludes edges squares on diags/anti-diags that goes through each sqaureIndex
     *
     * @return 1x64 array of relevant occupancy masks
     */
    inline consteval conf::types::BitboardTable initBishopRelevantMasks() noexcept
    {
        conf::types::BitboardTable masks{};
        int file;
        int rank;
        int diag;
        int antiDiag;

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            // Get file / rank indexes
            file = squareIndex & 7;
            rank = squareIndex >> 3;

            // diag = file - rank + 7  → index [0..14]
            diag = file - rank + 7;

            // antidiag = file + rank → index [0..14]
            antiDiag = file + rank;

            // Squares on the same diags, excluding edges
            Bitboard byDiag = DIAGONALS_MASKS[diag] & NOT_FILE_EDGES_MASK & NOT_RANK_EDGES_MASK;
            Bitboard byAntiDiag = ANTI_DIAGONALS_MASKS[antiDiag] & NOT_FILE_EDGES_MASK & NOT_RANK_EDGES_MASK;

            // remove squareIndex
            masks[squareIndex] = (byDiag | byAntiDiag) ^ Bitboard(1ULL << squareIndex);
        }

        return masks;
    }
    inline constexpr conf::types::BitboardTable BISHOP_RELEVANT_MASKS = initBishopRelevantMasks();

    // Direction arrays for sliding pieces
    inline constexpr std::array<conf::enums::Directions, 4> ROOK_DIRECTIONS = {
        conf::enums::Directions::NORTH, conf::enums::Directions::SOUTH, conf::enums::Directions::EAST,
        conf::enums::Directions::WEST};

    inline constexpr std::array<conf::enums::Directions, 4> BISHOP_DIRECTIONS = {
        conf::enums::Directions::NORTH_EAST, conf::enums::Directions::NORTH_WEST, conf::enums::Directions::SOUTH_EAST,
        conf::enums::Directions::SOUTH_WEST};

    /**
     * @brief Builds the rook's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x4096 array of rook attacks
     */
    inline consteval conf::types::rookAttacksTable initRookAttacksTable() noexcept
    {
        conf::types::rookAttacksTable table{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            uint8_t bits = 64 - board::magic_const::rookShifts[squareIndex];
            uint64_t nEntries = 1ULL << bits;

            // Get relevant occupancy mask for this square
            board::Bitboard relevantMask = ROOK_RELEVANT_MASKS[squareIndex];
            uint8_t relevantBits = relevantMask.popCount();

            // Generate all possible occupancies
            for (uint64_t occupancy = 0; occupancy < nEntries; occupancy++)
            {
                // Convert occupancy pattern to actual bitboard
                board::Bitboard occupancyBB = 0;
                board::Bitboard tempMask = relevantMask;

                for (uint8_t i = 0; i < relevantBits; i++)
                {
                    uint8_t square = tempMask.lsbIndex();
                    if (occupancy & (1ULL << i))
                    {
                        occupancyBB |= board::Bitboard(1ULL << square);
                    }
                    tempMask = board::Bitboard(tempMask.getData() & (tempMask.getData() - 1)); // Clear LSB
                }

                // Calculate attack pattern using direct bitwise operations
                board::Bitboard attacks = 0;
                board::Bitboard occupancyMasked = occupancyBB & ROOK_RELEVANT_MASKS[squareIndex];

                // North
                board::Bitboard ray = board::Bitboard(1ULL << squareIndex) << 8;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = ray << 8;
                }

                // South
                ray = board::Bitboard(1ULL << squareIndex) >> 8;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = ray >> 8;
                }

                // East
                ray = (board::Bitboard(1ULL << squareIndex) & ~FILE_H_MASK) << 1;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_H_MASK) << 1;
                }

                // West
                ray = (board::Bitboard(1ULL << squareIndex) & ~FILE_A_MASK) >> 1;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_A_MASK) >> 1;
                }

                // Store in table using magic index
                uint64_t magicIndex = (occupancyBB.getData() * board::magic_const::rookMagics[squareIndex].getData()) >>
                                      board::magic_const::rookShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline constexpr conf::types::rookAttacksTable ROOK_ATTACKS_TABLE = initRookAttacksTable();

    /**
     * @brief Builds the bishop's attacks table.
     * @details
     * Computes all relevant attacks for all squares and all possible occupancies.
     *
     * @return 64x512 array of bishop attacks
     */
    inline consteval conf::types::bishopAttacksTable initBishopAttacksTable() noexcept
    {
        conf::types::bishopAttacksTable table{};

        for (std::size_t squareIndex = 0; squareIndex < 64; squareIndex++)
        {
            uint8_t bits = 64 - board::magic_const::bishopShifts[squareIndex];
            uint64_t nEntries = 1ULL << bits;

            // Get relevant occupancy mask for this square
            board::Bitboard relevantMask = BISHOP_RELEVANT_MASKS[squareIndex];
            uint8_t relevantBits = relevantMask.popCount();

            // Generate all possible occupancies
            for (uint64_t occupancy = 0; occupancy < nEntries; occupancy++)
            {
                // Convert occupancy pattern to actual bitboard
                board::Bitboard occupancyBB = 0;
                board::Bitboard tempMask = relevantMask;

                for (uint8_t i = 0; i < relevantBits; i++)
                {
                    uint8_t square = tempMask.lsbIndex();
                    if (occupancy & (1ULL << i))
                    {
                        occupancyBB |= board::Bitboard(1ULL << square);
                    }
                    tempMask = board::Bitboard(tempMask.getData() & (tempMask.getData() - 1)); // Clear LSB
                }

                // Calculate attack pattern using direct bitwise operations (like in magics_generator.cpp)
                board::Bitboard attacks = 0;
                board::Bitboard occupancyMasked = occupancyBB & BISHOP_RELEVANT_MASKS[squareIndex];

                // North-East
                board::Bitboard ray = (board::Bitboard(1ULL << squareIndex) & ~FILE_H_MASK) << 9;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_H_MASK) << 9;
                }

                // North-West
                ray = (board::Bitboard(1ULL << squareIndex) & ~FILE_A_MASK) << 7;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_A_MASK) << 7;
                }

                // South-East
                ray = (board::Bitboard(1ULL << squareIndex) & ~FILE_H_MASK) >> 7;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_H_MASK) >> 7;
                }

                // South-West
                ray = (board::Bitboard(1ULL << squareIndex) & ~FILE_A_MASK) >> 9;
                while (!ray.isEmpty())
                {
                    attacks |= ray;
                    if ((ray & occupancyMasked) != board::Bitboard(0ULL))
                        break;
                    ray = (ray & ~FILE_A_MASK) >> 9;
                }

                // Store in table using magic index
                uint64_t magicIndex =
                    (occupancyBB.getData() * board::magic_const::bishopMagics[squareIndex].getData()) >>
                    board::magic_const::bishopShifts[squareIndex];
                table[squareIndex][magicIndex] = attacks;
            }
        }

        return table;
    }
    inline constexpr conf::types::bishopAttacksTable BISHOP_ATTACKS_TABLE = initBishopAttacksTable();

    /**
     * @brief Builds 'between 2 squares' masks.
     * @return 2x64 array of attacks masks
     */
    inline consteval conf::types::BetweenMasks initBetweenMasks() noexcept
    {
        conf::types::BetweenMasks masks{};

        for (std::size_t fromSquare = 0; fromSquare < 64; fromSquare++)
        {
            for (std::size_t toSquare = 0; toSquare < 64; toSquare++)
            {
                if (fromSquare == toSquare)
                    continue;

                // If there is any, extract common line / diag
                board::Bitboard line = ROOK_ATTACKS_MASKS[fromSquare] & ROOK_ATTACKS_MASKS[toSquare];
                board::Bitboard diag = BISHOP_ATTACKS_MASKS[fromSquare] & BISHOP_ATTACKS_MASKS[toSquare];
                board::Bitboard mask = line | diag;

                // Keeps only squares strictly between 'fromSquare' and 'toSquare'
                board::Bitboard both = (board::Bitboard{1ULL << fromSquare} | board::Bitboard{1ULL << toSquare});

                masks[fromSquare][toSquare] = mask & ~both;
            }
        }

        return masks;
    }
    inline constexpr conf::types::BetweenMasks BETWEEN_MASKS = initBetweenMasks();

    inline constexpr conf::types::CastlingMasks CASTLING_MASKS = {
        board::Bitboard((1ULL << 5) | (1ULL << 6)), board::Bitboard((1ULL << 1) | (1ULL << 2) | (1ULL << 3)),
        board::Bitboard((1ULL << 61) | (1ULL << 62)), board::Bitboard((1ULL << 57) | (1ULL << 58) | (1ULL << 59))};

} // namespace engine::board::mask

#endif // MASKS_H_