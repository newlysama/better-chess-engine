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

        EXPECT_EQ(nodes, 20);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_2_DEPTH_1)
    {
        Game game(FEN_2);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {8, 0, 2, 0, 0, 0, 0};

        EXPECT_EQ(nodes, 48);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_3_DEPTH_1)
    {
        Game game(FEN_3);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {1, 0, 0, 0, 2, 0, 0};

        EXPECT_EQ(nodes, 14);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_4_DEPTH_1)
    {
        Game game(FEN_4);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);
        Counters expected = {0, 0, 0, 0, 0, 0, 0};

        EXPECT_EQ(nodes, 6);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_5_DEPTH_1)
    {
        Game game(FEN_5);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);

        EXPECT_EQ(nodes, 44);
    }

    TEST(PeftTest, FEN_6_DEPTH_1)
    {
        Game game(FEN_6);
        Counters counters;

        uint64_t nodes = perft(game, 1, counters);

        EXPECT_EQ(nodes, 46);
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

        EXPECT_EQ(nodes, 400);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_2_DEPTH_2)
    {
        Game game(FEN_2);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {351, 1, 91, 0, 3, 0, 0};

        EXPECT_EQ(nodes, 2039);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_3_DEPTH_2)
    {
        Game game(FEN_3);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {14, 0, 0, 0, 10, 0, 0};

        EXPECT_EQ(nodes, 191);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_4_DEPTH_2)
    {
        Game game(FEN_4);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);
        Counters expected = {87, 0, 6, 48, 10, 0, 0};

        EXPECT_EQ(nodes, 264);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_5_DEPTH_2)
    {
        Game game(FEN_5);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);

        EXPECT_EQ(nodes, 1486);
    }

    TEST(PeftTest, FEN_6_DEPTH_2)
    {
        Game game(FEN_6);
        Counters counters;

        uint64_t nodes = perft(game, 2, counters);

        EXPECT_EQ(nodes, 2079);
    }

    // ---------------------------------------------------------------------
    // DEPTH 3
    // ---------------------------------------------------------------------

    TEST(PeftTest, FEN_1_DEPTH_3)
    {
        Game game(FEN_1);
        Counters counters;
        uint64_t nodes = perft(game, 3, counters);
        Counters expected = {34, 0, 0, 0, 12, 0, 0};
        EXPECT_EQ(nodes, 8902);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_2_DEPTH_3)
    {
        Game game(FEN_2);
        Counters counters;
        uint64_t nodes = perft(game, 3, counters);
        Counters expected = {17102, 45, 3162, 0, 993, 0, 1};
        EXPECT_EQ(nodes, 97862);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_3_DEPTH_3)
    {
        Game game(FEN_3);
        Counters counters;
        uint64_t nodes = perft(game, 3, counters);
        Counters expected = {209, 2, 0, 0, 267, 0, 0};
        EXPECT_EQ(nodes, 2812);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_4_DEPTH_3)
    {
        Game game(FEN_4);
        Counters counters;
        uint64_t nodes = perft(game, 3, counters);
        Counters expected = {1021, 4, 0, 120, 38, 0, 22};
        EXPECT_EQ(nodes, 9467);
        EXPECT_EQ(counters, expected);
    }

    TEST(PeftTest, FEN_5_DEPTH_3)
    {
        Game game(FEN_5);
        Counters counters;
        uint64_t nodes = perft(game, 3, counters);
        EXPECT_EQ(nodes, 62379);
    }

    TEST(PeftTest, FEN_6_DEPTH_3)
    {
        Game game(FEN_6);
        Counters counters;
        uint64_t nodes = perft(game, 3, counters);
        EXPECT_EQ(nodes, 89890);
    }

} // namespace test
