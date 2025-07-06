/**
 * @file      magics_generator.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Magic numbers generation implementation.
 */

#include "engine/board/magics_generator.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

#include "logging/logging.h"

/**
 * @namespace engine::board::magics_generator
 */
namespace engine::board::magics_generator
{
    using namespace engine::board;
    using namespace engine::board::magics_generator;
    using namespace conf::enums;

    Bitboard getRookMaskAt(int squareIndex) noexcept
    {
        // Get file / rank indexes
        const int file = Board::getFileIndex(squareIndex);
        const int rank = Board::getRankIndex(squareIndex);

        // Exclude ranks 1 and 8
        Bitboard byFile = mask::FILES_MASKS[file] & mask::NOT_RANK_EDGES_MASK;

        // Exclude files A and H
        Bitboard byRank = mask::RANKS_MASKS[rank] & mask::NOT_FILE_EDGES_MASK;

        return (byFile | byRank) & ~Bitboard(1ULL << squareIndex);
    }

    Bitboard getBishopMaskAt(int squareIndex) noexcept
    {
        // Get file / rank indexes
        const int file = Board::getFileIndex(squareIndex);
        const int rank = Board::getRankIndex(squareIndex);

        // diag = file - rank + 7  → index [0..14]
        const int diag = file - rank + 7;

        // antidiag = file + rank → index [0..14]
        const int antiDiag = file + rank;

        // Squares on the same diags, excluding edges
        Bitboard byDiag = mask::DIAGONALS_MASKS[diag] & mask::NOT_FILE_EDGES_MASK & mask::NOT_RANK_EDGES_MASK;
        Bitboard byAntiDiag =
            mask::ANTI_DIAGONALS_MASKS[antiDiag] & mask::NOT_FILE_EDGES_MASK & mask::NOT_RANK_EDGES_MASK;

        // on retire la case sq
        return (byDiag | byAntiDiag) ^ Bitboard(1ULL << squareIndex);
    }

    Bitboard slidingAttackRook(int squareIndex, Bitboard occ) noexcept
    {
        Bitboard attacks(0ULL);
        Bitboard occMasked = occ & getRookMaskAt(squareIndex);

        // North
        Bitboard ray = Board::shiftDir<Directions::NORTH>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::NORTH>(ray);
        }

        // South
        ray = Board::shiftDir<Directions::SOUTH>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::SOUTH>(ray);
        }

        // East
        ray = Board::shiftDir<Directions::EAST>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::EAST>(ray);
        }

        // West
        ray = Board::shiftDir<Directions::WEST>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::WEST>(ray);
        }

        return attacks;
    }

    Bitboard slidingAttackBishop(int squareIndex, Bitboard occ) noexcept
    {
        Bitboard attacks(0ULL);
        Bitboard occMasked = occ & getBishopMaskAt(squareIndex);

        // NE
        Bitboard ray = Board::shiftDir<Directions::NORTH_EAST>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::NORTH_EAST>(ray);
        }

        // NW
        ray = Board::shiftDir<Directions::NORTH_WEST>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::NORTH_WEST>(ray);
        }

        // SE
        ray = Board::shiftDir<Directions::SOUTH_EAST>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::SOUTH_EAST>(ray);
        }

        // SW
        ray = Board::shiftDir<Directions::SOUTH_WEST>(Bitboard(1ULL << squareIndex));
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occMasked) != Bitboard(0ULL))
                break;

            ray = Board::shiftDir<Directions::SOUTH_WEST>(ray);
        }

        return attacks;
    }

    uint8_t findShiftRook(int squareIndex) noexcept
    {
        return static_cast<uint8_t>(64 - getRookMaskAt(squareIndex).popCount());
    }

    uint8_t findShiftBishop(int squareIndex) noexcept
    {
        return static_cast<uint8_t>(64 - getBishopMaskAt(squareIndex).popCount());
    }

    Bitboard findMagicRook(int squareIndex) noexcept
    {
        const Bitboard mask = getRookMaskAt(squareIndex);
        const uint8_t shift = findShiftRook(squareIndex);

        const uint8_t bits = mask.popCount();
        const uint64_t subsetCount = 1ULL << bits;

        std::vector<uint64_t> used(subsetCount, 0ULL);
        std::vector<uint64_t> occupancies(subsetCount);

        // Precompute all occupancy subsets
        std::vector<int> bitIndices;
        for (int i = 0; i < 64; i++)
        {
            if ((mask.getData() >> i) & 1ULL)
            {
                bitIndices.push_back(i);
            }
        }

        for (uint64_t idx = 0; idx < subsetCount; idx++)
        {
            uint64_t occ = 0ULL;
            for (uint8_t j = 0; j < bits; j++)
            {
                if (idx & (1ULL << j))
                {
                    occ |= (1ULL << bitIndices[j]);
                }
            }
            occupancies[idx] = occ;
        }

        std::mt19937_64 rng(std::random_device{}());
        while (true)
        {
            uint64_t magic = rng() & rng() & rng();
            std::fill(used.begin(), used.end(), 0ULL);
            bool fail = false;

            for (uint64_t idx = 0; idx < subsetCount; idx++)
            {
                uint64_t occ = occupancies[idx];
                uint64_t key = (occ * magic) >> shift;
                uint64_t attack = slidingAttackRook(squareIndex, Bitboard(occ)).getData();

                if (used[key] == 0ULL)
                {
                    used[key] = attack;
                }
                else if (used[key] != attack)
                {
                    fail = true;
                    break;
                }
            }

            if (!fail)
            {
                return Bitboard(magic);
            }
        }
    }

    Bitboard findMagicBishop(int squareIndex) noexcept
    {
        const Bitboard mask = getBishopMaskAt(squareIndex);
        const uint8_t shift = findShiftBishop(squareIndex);

        const uint8_t bits = mask.popCount();
        const uint64_t subsetCount = 1ULL << bits;

        std::vector<uint64_t> used(subsetCount, 0ULL);
        std::vector<uint64_t> occupancies(subsetCount);
        std::vector<int> bitIndices;

        for (int i = 0; i < 64; i++)
        {
            if ((mask.getData() >> i) & 1ULL)
            {
                bitIndices.push_back(i);
            }
        }

        for (uint64_t idx = 0; idx < subsetCount; idx++)
        {
            uint64_t occ = 0ULL;
            for (uint8_t j = 0; j < bits; j++)
            {
                if (idx & (1ULL << j))
                {
                    occ |= (1ULL << bitIndices[j]);
                }
            }

            occupancies[idx] = occ;
        }

        std::mt19937_64 rng(std::random_device{}());
        while (true)
        {
            uint64_t magic = rng() & rng() & rng();
            std::fill(used.begin(), used.end(), 0ULL);
            bool fail = false;

            for (uint64_t idx = 0; idx < subsetCount; idx++)
            {
                uint64_t occ = occupancies[idx];
                uint64_t key = (occ * magic) >> shift;
                uint64_t attack = slidingAttackBishop(squareIndex, Bitboard(occ)).getData();

                if (used[key] == 0ULL)
                {
                    used[key] = attack;
                }
                else if (used[key] != attack)
                {
                    fail = true;
                    break;
                }
            }

            if (!fail)
                return Bitboard(magic);
        }
    }

    void initMagics() noexcept
    {
        std::array<uint64_t, 64> rMagicVals;
        std::array<uint8_t, 64> rShiftVals;
        std::array<uint64_t, 64> bMagicVals;
        std::array<uint8_t, 64> bShiftVals;

        for (int squareIndex = 0; squareIndex < 64; ++squareIndex)
        {
            LOG_INFO("Computing magics for square {}", squareIndex);
            auto rmagic = findMagicRook(squareIndex);
            auto bmagic = findMagicBishop(squareIndex);

            rMagicVals[squareIndex] = rmagic.getData();
            bMagicVals[squareIndex] = bmagic.getData();

            LOG_INFO("Computing shifts for square {}", squareIndex);
            rShiftVals[squareIndex] = findShiftRook(squareIndex);
            bShiftVals[squareIndex] = findShiftBishop(squareIndex);
        }

        // Print C++ constexpr initializers
        std::cout << "inline constexpr conf::types::BitboardTable rookMagics = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << "Bitboard{0x" << std::hex << rMagicVals[i] << "ULL}" << (i < 63 ? ", " : "");
        }
        std::cout << "};\n";

        std::cout << "inline constexpr conf::types::BitboardTable bishopMagics = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << "Bitboard{0x" << std::hex << bMagicVals[i] << "ULL}" << (i < 63 ? ", " : "");
        }
        std::cout << "};\n";

        std::cout << "inline constexpr std::array<uint8_t,64> rookShifts = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << std::dec << static_cast<int>(rShiftVals[i]) << (i < 63 ? ", " : "");
        }
        std::cout << "};\n";

        std::cout << "inline constexpr std::array<uint8_t,64> bishopShifts = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << std::dec << static_cast<int>(bShiftVals[i]) << (i < 63 ? ", " : "");
        }
        std::cout << "};\n";
    }

} // namespace engine::board::magics_generator
