/**
 * @file      benchmark.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Benchmark of the perft function performance (move generation).
 */

#include <array>
#include <chrono>
#include <cstdint>
#include <oneapi/tbb.h>
#include <print>

#include "engine/game/game.h"
#include "engine/game/move_list.h"

namespace engine::benchmark
{
    const std::array<std::string, 35> BenchmarkStates = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 10",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 11",
        "4rrk1/pp1n3p/3q2pQ/2p1pb2/2PP4/2P3N1/P2B2PP/4RRK1 b - - 7 19",
        "rq3rk1/ppp2ppp/1bnpb3/3N2B1/3NP3/7P/PPPQ1PP1/2KR3R w - - 7 14",
        "r1bq1r1k/1pp1n1pp/1p1p4/4p2Q/4Pp2/1BNP4/PPP2PPP/3R1RK1 w - - 2 14",
        "r3r1k1/2p2ppp/p1p1bn2/8/1q2P3/2NPQN2/PPP3PP/R4RK1 b - - 2 15",
        "r1bbk1nr/pp3p1p/2n5/1N4p1/2Np1B2/8/PPP2PPP/2KR1B1R w kq - 0 13",
        "r1bq1rk1/ppp1nppp/4n3/3p3Q/3P4/1BP1B3/PP1N2PP/R4RK1 w - - 1 16",
        "4r1k1/r1q2ppp/ppp2n2/4P3/5Rb1/1N1BQ3/PPP3PP/R5K1 w - - 1 17",
        "2rqkb1r/ppp2p2/2npb1p1/1N1Nn2p/2P1PP2/8/PP2B1PP/R1BQK2R b KQ - 0 11",
        "r1bq1r1k/b1p1npp1/p2p3p/1p6/3PP3/1B2NN2/PP3PPP/R2Q1RK1 w - - 1 16",
        "3r1rk1/p5pp/bpp1pp2/8/q1PP1P2/b3P3/P2NQRPP/1R2B1K1 b - - 6 22",
        "r1q2rk1/2p1bppp/2Pp4/p6b/Q1PNp3/4B3/PP1R1PPP/2K4R w - - 2 18",
        "4k2r/1pb2ppp/1p2p3/1R1p4/3P4/2r1PN2/P4PPP/1R4K1 b - - 3 22",
        "3q2k1/pb3p1p/4pbp1/2r5/PpN2N2/1P2P2P/5PP1/Q2R2K1 b - - 4 26",
        "6k1/6p1/6Pp/ppp5/3pn2P/1P3K2/1PP2P2/3N4 b - - 0 1",
        "3b4/5kp1/1p1p1p1p/pP1PpP1P/P1P1P3/3KN3/8/8 w - - 0 1",
        "2K5/p7/7P/5pR1/8/5k2/r7/8 w - - 0 1",
        "8/6pk/1p6/8/PP3p1p/5P2/4KP1q/3Q4 w - - 0 1",
        "7k/3p2pp/4q3/8/4Q3/5Kp1/P6b/8 w - - 0 1",
        "8/2p5/8/2kPKp1p/2p4P/2P5/3P4/8 w - - 0 1",
        "8/1p3pp1/7p/5P1P/2k3P1/8/2K2P2/8 w - - 0 1",
        "8/pp2r1k1/2p1p3/3pP2p/1P1P1P1P/P5KR/8/8 w - - 0 1",
        "8/3p4/p1bk3p/Pp6/1Kp1PpPp/2P2P1P/2P5/5B2 b - - 0 1",
        "5k2/7R/4P2p/5K2/p1r2P1p/8/8/8 b - - 0 1",
        "6k1/6p1/P6p/r1N5/5p2/7P/1b3PP1/4R1K1 w - - 0 1",
        "1r3k2/4q3/2Pp3b/3Bp3/2Q2p2/1p1P2P1/1P2KP2/3N4 w - - 0 1",
        "6k1/4pp1p/3p2p1/P1pPb3/R7/1r2P1PP/3B1P2/6K1 w - - 0 1",
        "8/3p3B/5p2/5P2/p7/PP5b/k7/6K1 w - - 0 1",
        "5rk1/q6p/2p3bR/1pPp1rP1/1P1Pp3/P3B1Q1/1K3P2/R7 w - - 93 90",
        "4rrk1/1p1nq3/p7/2p1P1pp/3P2bp/3Q1Bn1/PPPB4/1K2R1NR w - - 40 21",
        "r3k2r/3nnpbp/q2pp1p1/p7/Pp1PPPP1/4BNN1/1P5P/R2Q1RK1 w kq - 0 16",
        "3Qb1k1/1r2ppb1/pN1n2q1/Pp1Pp1Pr/4P2p/4BP2/4B1R1/1R5K b - - 11 40",
        "4k3/3q1r2/1N2r1b1/3ppN2/2nPP3/1B1R2n1/2R1Q3/3K4 w - - 5 1"};

    using Clock = std::chrono::steady_clock;
    using NodesPerSec = std::array<uint64_t, BenchmarkStates.size()>;
    using Times = std::array<int64_t, BenchmarkStates.size()>;

    /**
     * @brief Simple perft function counting leaves from a given state and depth.
     *
     * @param [in] game  : Current state of the game
     * @param [in] depth : Current depth
     * @tparam isRoot    : Is the current depth the root depth
     * @return : the number of leaves
     */
    template <bool isRoot>
    inline uint64_t perft(engine::game::Game& game, int depth) noexcept
    {
        if (depth == 0)
            return 1ULL;

        game.m_moveList.generateAllMoves(game.m_state);

        // If we are at first exection, multi-task the each branch
        if constexpr (isRoot)
        {
            // Parallel reduce on each node
            uint64_t nodes = tbb::parallel_reduce(
                tbb::blocked_range<std::size_t>(0, game.m_moveList.size()), uint64_t{0},
                [&](const tbb::blocked_range<std::size_t>& range, uint64_t local) -> uint64_t {
                    // Local copy of game
                    engine::game::Game localGame = game;

                    for (std::size_t i = range.begin(); i != range.end(); ++i)
                    {
                        localGame.makeMove<true>(game.m_moveList[i]);

                        localGame.m_moveList.generateAllMoves(localGame.m_state);
                        local += perft<false>(localGame, depth - 1);

                        localGame.unmakeMove(game.m_moveList[i]);
                    }

                    return local;
                },
                // Reduce
                std::plus<uint64_t>());

            return nodes;
        }

        uint64_t nodes = 0;
        for (const engine::game::Move mv : game.m_moveList)
        {
            game.makeMove<true>(mv);
            game.m_moveList.generateAllMoves(game.m_state);
            nodes += perft<false>(game, depth - 1);
            game.unmakeMove(mv);
        }

        return nodes;
    }

    /**
     * @brief Prints benchmark results.
     *
     * @param [in] allNodesPerSec : array of all nodes/sec measures
     * @param [in] allTimes       : array of all perft execution times
     */
    void handleBenchmarkResults(const NodesPerSec& allNodesPerSec, const Times& allTimes)
    {
        std::locale::global(std::locale{"en_US.UTF-8"});

        uint64_t maxNodesPerSec = 0;
        uint64_t minNodesPerSec = UINT64_MAX;
        uint64_t totalNodesPerSecond = 0;

        int64_t maxTime = 0;
        int64_t minTime = INT64_MAX;
        int64_t totalTime = 0;

        for (const uint64_t& nodesPerSec : allNodesPerSec)
        {
            if (nodesPerSec > maxNodesPerSec)
                maxNodesPerSec = nodesPerSec;
            else if (nodesPerSec < minNodesPerSec)
                minNodesPerSec = nodesPerSec;

            totalNodesPerSecond += nodesPerSec;
        }
        uint64_t averageNodesPerSecond = totalNodesPerSecond / BenchmarkStates.size();

        for (const int64_t& time : allTimes)
        {
            if (time > maxTime)
                maxTime = time;
            else if (time < minTime)
                minTime = time;

            totalTime += time;
        }
        int64_t averageTime = totalTime / BenchmarkStates.size();

        std::println(">>>>>>>>>> BENCHMARK RESULTS <<<<<<<<<<");
        std::println();

        std::println("Maximum time reached : {:L} ms", maxTime);
        std::println("Minimum time reached : {:L} ms", minTime);
        std::println("Average time reached : {:L} ms", averageTime);

        std::println("Maximum nodes/sec reached : {:L}", maxNodesPerSec);
        std::println("Minimum nodes/sec reached : {:L}", minNodesPerSec);
        std::println("Average nodes/sec reached : {:L}", averageNodesPerSecond);
    }

    /**
     * @brief Launch perft's benchmark on each position give by BenchmarkStates.
     *
     * @param [in] depth : Depth we want to go too
     */
    void benchmark_perft(int depth)
    {
        std::locale::global(std::locale{"en_US.UTF-8"});

        std::println(">>>>>>>>>> STARTING BENCHMARK <<<<<<<<<<");

        NodesPerSec allNodesPerSec;
        Times allTimes;

        for (std::size_t i = 0; i < BenchmarkStates.size(); i++)
        {
            std::println("==============================================================================");
            std::println("Benchmark for FEN : {}", BenchmarkStates[i]);

            uint64_t nodes = 0;
            engine::game::Game game{BenchmarkStates[i]};
            const auto t0 = Clock::now();

            nodes = perft<true>(game, depth);

            const auto t1 = Clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            const double nodesPerSec = static_cast<double>(nodes) * 1'000'000.0 / micros;

            allNodesPerSec[i] = nodesPerSec;
            allTimes[i] = micros / 1000;

            std::println("Depth          : {}", depth);
            std::println("Nodes searched : {:L}", nodes);
            std::println("Time           : {:L} ms", micros / 1000);
            std::println("Nodes/second   : {:L}", static_cast<uint64_t>(nodesPerSec));

            std::println();
        }

        handleBenchmarkResults(allNodesPerSec, allTimes);
    }
} // namespace engine::benchmark