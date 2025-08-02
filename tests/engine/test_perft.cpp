/**
 * @file      test_game.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Tests the legal move .
 */

#include "test_utils.h"

namespace test
{
    using namespace engine::core;
    using namespace engine::game;
    using namespace engine::board;

    // ---------------------------------------------------------------------
    // DEPTH 1
    // ---------------------------------------------------------------------

    TEST(PeftTest, FEN_1_DEPTH_1)
    {
        Game game(FEN_1);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {0, 0, 0, 0, 0, 0, 0};

        ASSERT_EQ(nodes, 20);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_2_DEPTH_1)
    {
        LOG_DEBUG("FEN 2 STARTING");
        Game game(FEN_2);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {8, 0, 2, 0, 0, 0, 0};

        ASSERT_EQ(nodes, 48);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_3_DEPTH_1)
    {
        Game game(FEN_3);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {1, 0, 0, 0, 2, 0, 0};

        ASSERT_EQ(nodes, 14);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_4_DEPTH_1)
    {
        Game game(FEN_4);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {0, 0, 0, 0, 0, 0, 0};

        ASSERT_EQ(nodes, 6);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_5_DEPTH_1)
    {
        Game game(FEN_5);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);

        ASSERT_EQ(nodes, 44);
    }

    TEST(PeftTest, FEN_6_DEPTH_1)
    {
        Game game(FEN_6);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);

        ASSERT_EQ(nodes, 46);
    }

    // ---------------------------------------------------------------------
    // DEPTH 2
    // ---------------------------------------------------------------------

    TEST(PeftTest, FEN_1_DEPTH_2)
    {
        Game game(FEN_1);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {0, 0, 0, 0, 0, 0, 0};

        ASSERT_EQ(nodes, 400);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_2_DEPTH_2)
    {
        Game game(FEN_2);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {351, 1, 91, 0, 3, 0, 0};

        ASSERT_EQ(nodes, 2039);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_3_DEPTH_2)
    {
        Game game(FEN_3);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {14, 0, 0, 0, 10, 0, 0};

        ASSERT_EQ(nodes, 191);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_4_DEPTH_2)
    {
        Game game(FEN_4);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {87, 0, 6, 48, 10, 0, 0};

        ASSERT_EQ(nodes, 264);
        ASSERT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_5_DEPTH_2)
    {
        Game game(FEN_5);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);

        ASSERT_EQ(nodes, 1486);
    }

    TEST(PeftTest, FEN_6_DEPTH_2)
    {
        Game game(FEN_6);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);

        ASSERT_EQ(nodes, 2079);
    }

} // namespace test
