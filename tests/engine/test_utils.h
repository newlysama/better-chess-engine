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

        EXPECT_EQ(a.m_halfMoveClock, b.m_halfMoveClock);
        EXPECT_EQ(a.m_fullMoveClock, b.m_fullMoveClock);
        EXPECT_EQ(a.m_sideToMove, b.m_sideToMove);
        EXPECT_EQ(a.m_castlingRights, b.m_castlingRights);

        EXPECT_EQ(a.m_epSquare, b.m_epSquare);
        EXPECT_EQ(a.m_kgSquares, b.m_kgSquares);
        EXPECT_EQ(a.m_isChecked, b.m_isChecked);
        EXPECT_EQ(a.m_isDoubleChecked, b.m_isDoubleChecked);
        EXPECT_EQ(a.m_isCheckMate, b.m_isCheckMate);

        EXPECT_EQ(a.m_pinnedBB, b.m_pinnedBB);
        EXPECT_EQ(a.m_targetsBB, b.m_targetsBB);
        EXPECT_EQ(a.m_checkersBB, b.m_checkersBB);
        EXPECT_EQ(a.m_blockersBB, b.m_blockersBB);

        EXPECT_EQ(a.m_piecesBB, b.m_piecesBB);
        EXPECT_EQ(a.m_allOccBB, b.m_allOccBB);
        EXPECT_EQ(a.m_teamsOccBB, b.m_teamsOccBB);
    }
} // namespace test

#endif // TEST_UTILS_ENGINE_H_