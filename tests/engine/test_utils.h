/**
 * @file      test_utils.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Utility methods to perform tests on the engine.
 */

#ifndef TEST_UTILS_ENGINE_H_
#define TEST_UTILS_ENGINE_H_

#include <cstdint>
#include <gtest/gtest.h>

#include "engine/board/state.h"
#include "engine/core/const.h"
#include "engine/core/enums.h"
#include "engine/game/game.h"
#include "engine/game/move.h"
#include "utils/enums_to_string.h"
#include "utils/utils.h"

namespace test
{
    /**
     * @brief Check if two states equals
     *
     * @param [in] a : first state
     * @param [in] b : second state
     *
     * @return Whether the two states are equal
     */
    inline void compareStates(const engine::board::State& a, const engine::board::State& b)
    {
        using engine::board::State;

        ASSERT_EQ(a.m_halfMoveClock, b.m_halfMoveClock);
        ASSERT_EQ(a.m_fullMoveClock, b.m_fullMoveClock);
        ASSERT_EQ(a.m_sideToMove, b.m_sideToMove);
        ASSERT_EQ(a.m_castlingRights, b.m_castlingRights);

        ASSERT_EQ(a.m_epSquare, b.m_epSquare);
        ASSERT_EQ(a.m_kgSquares, b.m_kgSquares);
        ASSERT_EQ(a.m_isChecked, b.m_isChecked);
        ASSERT_EQ(a.m_isDoubleChecked, b.m_isDoubleChecked);
        ASSERT_EQ(a.m_isCheckMate, b.m_isCheckMate);

        ASSERT_EQ(a.m_pinnedBB, b.m_pinnedBB);
        ASSERT_EQ(a.m_targetsBB, b.m_targetsBB);
        ASSERT_EQ(a.m_checkersBB, b.m_checkersBB);
        ASSERT_EQ(a.m_blockersBB, b.m_blockersBB);

        ASSERT_EQ(a.m_piecesBB, b.m_piecesBB);
        ASSERT_EQ(a.m_allOccBB, b.m_allOccBB);
        ASSERT_EQ(a.m_teamsOccBB, b.m_teamsOccBB);
    }

    struct Counters
    {
        uint64_t captures = 0;
        uint64_t enPassants = 0;
        uint64_t castlings = 0;
        uint64_t promotions = 0;
        uint64_t checks = 0;
        uint64_t doubleChecks = 0;
        uint64_t checkmates = 0;

        bool operator==(const Counters& other) const noexcept
        {
            return captures == other.captures && enPassants == other.enPassants && castlings == other.castlings &&
                   promotions == other.promotions && checks == other.checks && doubleChecks == other.doubleChecks &&
                   checkmates == other.checkmates;
        }
    };

    inline uint64_t perft(engine::game::Game& game, int depth, Counters& counters) noexcept
    {
        if (depth == 0)
        {
            return 1ULL;
        }

        uint64_t nodes = 0;

        for (const engine::game::Move move : game.m_moveList)
        {
            switch (move.getMoveType())
            {
            case engine::core::MoveType::CAPTURE:
                counters.captures++;
                break;
            case engine::core::MoveType::CASTLE:
                counters.castlings++;
                break;
            case engine::core::MoveType::EN_PASSANT:
                counters.enPassants++;
                break;
            default:
                break;
            }

            if (move.isPromotion())
            {
                counters.promotions++;
            }

            game.makeMove<true>(move);

            if (game.m_state.m_isChecked)
            {
                counters.checks++;
            }
            if (game.m_state.m_isDoubleChecked)
            {
                counters.doubleChecks++;
            }
            if (game.m_state.m_isCheckMate)
            {
                counters.checkmates++;
            }

            nodes += perft(game, depth - 1, counters);
            game.unmakeMove(move);
        }

        return nodes;
    }

    const std::string FEN_1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string FEN_2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    const std::string FEN_3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    const std::string FEN_4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    const std::string FEN_5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    const std::string FEN_6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
} // namespace test

#endif // TEST_UTILS_ENGINE_H_