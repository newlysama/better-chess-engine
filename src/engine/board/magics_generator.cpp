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
#include <omp.h>
#include <random>
#include <vector>

#include "engine/board/mask.h"
#include "logging/logging.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace engine::core;

    Bitboard slidingAttackRook(const int squareIndex, const Bitboard occupancy) noexcept
    {
        Bitboard attacks(0ULL);
        Bitboard occupancyMasked = occupancy & ROOK_RELEVANT_MASKS[squareIndex];

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

        return attacks;
    }

    Bitboard slidingAttackBishop(const int squareIndex, const Bitboard occupancy) noexcept
    {
        Bitboard attacks(0ULL);
        Bitboard occupancyMasked = occupancy & BISHOP_RELEVANT_MASKS[squareIndex];

        // NE
        Bitboard ray = shiftDir<Directions::NORTH_EAST>(Bitboard{1ULL << squareIndex});
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occupancyMasked).isEmpty() == false)
                break;

            ray = shiftDir<Directions::NORTH_EAST>(ray);
        }

        // NW
        ray = shiftDir<Directions::NORTH_WEST>(Bitboard{1ULL << squareIndex});
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occupancyMasked).isEmpty() == false)
                break;

            ray = shiftDir<Directions::NORTH_WEST>(ray);
        }

        // SE
        ray = shiftDir<Directions::SOUTH_EAST>(Bitboard{1ULL << squareIndex});
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occupancyMasked).isEmpty() == false)
                break;

            ray = shiftDir<Directions::SOUTH_EAST>(ray);
        }

        // SW
        ray = shiftDir<Directions::SOUTH_WEST>(Bitboard{1ULL << squareIndex});
        while (!ray.isEmpty())
        {
            attacks |= ray;
            if ((ray & occupancyMasked).isEmpty() == false)
                break;

            ray = shiftDir<Directions::SOUTH_WEST>(ray);
        }

        return attacks;
    }

    uint8_t findShiftRook(const int squareIndex) noexcept
    {
        return 64 - ROOK_RELEVANT_MASKS[squareIndex].popCount();
    }

    uint8_t findShiftBishop(int squareIndex) noexcept
    {
        return 64 - BISHOP_RELEVANT_MASKS[squareIndex].popCount();
    }

    Bitboard findMagicRook(int squareIndex) noexcept
    {
        const Bitboard mask = ROOK_RELEVANT_MASKS[squareIndex];
        const uint8_t shift = findShiftRook(squareIndex);

        const uint8_t bits = mask.popCount();
        const uint64_t subsetCount = 1ULL << bits;

        std::vector<uint64_t> used(subsetCount, 0ULL);
        std::vector<uint64_t> occupancies(subsetCount);

        // Precompute all occupancyupancy subsets
        std::vector<int> bitIndices;
        for (int i = 0; i < 64; i++)
        {
            if (mask.isSet(i))
            {
                bitIndices.push_back(i);
            }
        }

        for (uint64_t idx = 0; idx < subsetCount; idx++)
        {
            uint64_t occupancy = 0ULL;
            for (uint8_t j = 0; j < bits; j++)
            {
                if (idx & (1ULL << j))
                {
                    occupancy |= (1ULL << bitIndices[j]);
                }
            }
            occupancies[idx] = occupancy;
        }

        std::mt19937_64 rng(std::random_device{}());
        while (true)
        {
            uint64_t magic = rng() & rng() & rng();
            std::fill(used.begin(), used.end(), 0ULL);
            bool fail = false;

            for (uint64_t idx = 0; idx < subsetCount; idx++)
            {
                uint64_t occupancy = occupancies[idx];
                uint64_t key = (occupancy * magic) >> shift;
                uint64_t attack = slidingAttackRook(squareIndex, Bitboard{occupancy}).getData();

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
                return Bitboard{magic};
            }
        }
    }

    Bitboard findMagicBishop(const int squareIndex) noexcept
    {
        const Bitboard mask = BISHOP_RELEVANT_MASKS[squareIndex];
        const uint8_t shift = findShiftBishop(squareIndex);

        const uint8_t bits = mask.popCount();
        const uint64_t subsetCount = 1ULL << bits;

        std::vector<uint64_t> used(subsetCount, 0ULL);
        std::vector<uint64_t> occupancies(subsetCount);
        std::vector<int> bitIndices;

        for (int i = 0; i < 64; i++)
        {
            if (mask.isSet(i))
            {
                bitIndices.push_back(i);
            }
        }

        for (uint64_t idx = 0; idx < subsetCount; idx++)
        {
            uint64_t occupancy = 0ULL;
            for (uint8_t j = 0; j < bits; j++)
            {
                if (idx & (1ULL << j))
                {
                    occupancy |= (1ULL << bitIndices[j]);
                }
            }

            occupancies[idx] = occupancy;
        }

        std::mt19937_64 rng(std::random_device{}());
        while (true)
        {
            uint64_t magic = rng() & rng() & rng();
            std::fill(used.begin(), used.end(), 0ULL);
            bool fail = false;

            for (uint64_t idx = 0; idx < subsetCount; idx++)
            {
                uint64_t occupancy = occupancies[idx];
                uint64_t key = (occupancy * magic) >> shift;
                uint64_t attack = slidingAttackBishop(squareIndex, Bitboard{occupancy}).getData();

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
                return Bitboard{magic};
        }
    }

    void initMagics() noexcept
    {
        std::array<uint64_t, 64> rMagicVals;
        std::array<uint8_t, 64> rShiftVals;
        std::array<uint64_t, 64> bMagicVals;
        std::array<uint8_t, 64> bShiftVals;

        // Get the number of threads on host machine
        int nproc = omp_get_num_procs();

        // Turn off automatic thread adjustment
        // Optional in practice, but I mean we never know
        omp_set_dynamic(0);

        // Specify the exact number of thread we want to use
        omp_set_num_threads(nproc);

#pragma omp parallel for
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
        // clang-format off
        std::cout << "inline constexpr core::BitboardTable rookMagics = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << "Bitboard{0x"
                      << std::hex
                      << rMagicVals[i]
                      << "ULL}"
                      << (i < 63 ? ", " : i % 3 == 0 ? "\n" : "");
        }
        std::cout << "};\n\n";

        std::cout << "inline constexpr core::BitboardTable bishopMagics = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << "Bitboard{0x"
                      << std::hex
                      << bMagicVals[i]
                      << "ULL}"
                      << (i < 63 ? ", " : i % 3 == 0 ? "\n" : "");
        }
        std::cout << "};\n\n";

        std::cout << "inline constexpr std::array<uint8_t,64> rookShifts = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << std::dec
                      << static_cast<int>(rShiftVals[i])
                      << (i < 63 ? ", " : i % 3 == 0 ? "\n" : "");
        }
        std::cout << "};\n\n";

        std::cout << "inline constexpr std::array<uint8_t,64> bishopShifts = {";
        for (int i = 0; i < 64; i++)
        {
            std::cout << std::dec
                      << static_cast<int>(bShiftVals[i])
                      << (i < 63 ? ", " : i % 3 == 0 ? "\n" : "");
        }
        std::cout << "};\n\n";
    }

} // namespace engine::board
