/**
 * @file      test_fen.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Tests the Game/State class initialization from FEN.
 */

#include "test_utils.h"

namespace test
{
    using namespace engine::core;
    using namespace engine::game;
    using namespace engine::board;

    // Helper macro to shorten castling tests
#define ASSERT_CASTLING(RK, RQ, BK, BQ)                                    \
    do                                                                     \
    {                                                                      \
        ASSERT_##RK(state.hasCastlingRight<Castling::WHITE_KING_SIDE>());  \
        ASSERT_##RQ(state.hasCastlingRight<Castling::WHITE_QUEEN_SIDE>()); \
        ASSERT_##BK(state.hasCastlingRight<Castling::BLACK_KING_SIDE>());  \
        ASSERT_##BQ(state.hasCastlingRight<Castling::BLACK_QUEEN_SIDE>()); \
    } while (false)

    // Base board
    TEST(GameInitTest, FEN1)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, TRUE, TRUE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);

        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));

        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    // En Passant square test
    TEST(GameInitTest, FEN_EP)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e7 0 1";

        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, TRUE, TRUE, TRUE);

        ASSERT_EQ(state.m_epSquare, SQUARE_INDEX.at("e7"));

        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));

        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    /************************************
     *          CASTLING TESTS          *
     ************************************/

    TEST(GameInitTest, FEN_C1)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQk - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, TRUE, TRUE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C2)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, TRUE, FALSE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C3)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kkq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, FALSE, TRUE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C4)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qkq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, TRUE, TRUE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C5)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, TRUE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C6)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kk - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, FALSE, TRUE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C7)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, FALSE, FALSE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C8)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qk - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, TRUE, TRUE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C9)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Qq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, TRUE, FALSE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C10)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w kq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, FALSE, TRUE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C11)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w K - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(TRUE, FALSE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C12)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Q - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, TRUE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C13)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w k - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, FALSE, TRUE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C14)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w q - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, FALSE, FALSE, TRUE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_C15)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, FALSE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));
        ASSERT_FALSE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    /************************************
     *            KING CHECKS           *
     ************************************/

    TEST(GameInitTest, FEN_K1)
    {
        const std::string fen = "4k3/4Q3/8/8/8/8/8/6K1 b - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::BLACK);

        ASSERT_CASTLING(FALSE, FALSE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("g1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));

        ASSERT_TRUE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_K2)
    {
        const std::string fen = "4k3/8/8/1B6/8/8/8/4R1K1 b - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::BLACK);

        ASSERT_CASTLING(FALSE, FALSE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("g1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));

        ASSERT_TRUE(state.m_isChecked);
        ASSERT_TRUE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_K3)
    {
        const std::string fen = "7k/5Q2/7R/8/8/8/8/7K b - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::BLACK);

        ASSERT_CASTLING(FALSE, FALSE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("h1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("h8"));

        ASSERT_TRUE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_TRUE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_K4)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPqPPP/RNBQKBNR w KQkq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_TRUE(state.hasCastlingRight<Castling::WHITE_KING_SIDE>());
        ASSERT_TRUE(state.hasCastlingRight<Castling::WHITE_QUEEN_SIDE>());
        ASSERT_TRUE(state.hasCastlingRight<Castling::BLACK_KING_SIDE>());
        ASSERT_TRUE(state.hasCastlingRight<Castling::BLACK_QUEEN_SIDE>());

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));

        ASSERT_TRUE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_K5)
    {
        const std::string fen = "4r1k1/8/8/8/1b6/8/8/4K3 w - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 0);
        ASSERT_EQ(state.m_fullMoveClock, 1);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_CASTLING(FALSE, FALSE, FALSE, FALSE);

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("g8"));

        ASSERT_TRUE(state.m_isChecked);
        ASSERT_TRUE(state.m_isDoubleChecked);
        ASSERT_FALSE(state.m_isCheckMate);
    }

    TEST(GameInitTest, FEN_K6)
    {
        const std::string fen = "rnb1kbnr/pppppppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.m_halfMoveClock, 1);
        ASSERT_EQ(state.m_fullMoveClock, 3);
        ASSERT_EQ(state.m_sideToMove, Color::WHITE);

        ASSERT_TRUE(state.hasCastlingRight<Castling::WHITE_KING_SIDE>());
        ASSERT_TRUE(state.hasCastlingRight<Castling::WHITE_QUEEN_SIDE>());
        ASSERT_TRUE(state.hasCastlingRight<Castling::BLACK_KING_SIDE>());
        ASSERT_TRUE(state.hasCastlingRight<Castling::BLACK_QUEEN_SIDE>());

        ASSERT_EQ(state.m_epSquare, -1);
        ASSERT_EQ(state.m_kgSquares[0], SQUARE_INDEX.at("e1"));
        ASSERT_EQ(state.m_kgSquares[1], SQUARE_INDEX.at("e8"));

        ASSERT_TRUE(state.m_isChecked);
        ASSERT_FALSE(state.m_isDoubleChecked);
        ASSERT_TRUE(state.m_isCheckMate);
    }

#undef ASSERT_CASTLING

    // ===========================
    //   State::getFenOccupancy
    // ===========================

    TEST(FenOccupancyTest, OCC_Startpos)
    {
        const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.getFenOccupancy(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    }

    TEST(FenOccupancyTest, OCC_OnlyKingsCorners)
    {
        const std::string fen = "7k/8/8/8/8/8/8/K7 w - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.getFenOccupancy(), "7k/8/8/8/8/8/8/K7");
    }

    TEST(FenOccupancyTest, OCC_CompressionMixed)
    {
        const std::string fen = "7k/pppppppp/8/8/8/8/PPPPPPPP/K7 b - - 0 1";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.getFenOccupancy(), "7k/pppppppp/8/8/8/8/PPPPPPPP/K7");
    }

    TEST(FenOccupancyTest, OCC_FromExistingSuiteLikeK6)
    {
        const std::string fen = "rnb1kbnr/pppppppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3";
        Game game(fen);
        const State& state = game.m_state;

        ASSERT_EQ(state.getFenOccupancy(), "rnb1kbnr/pppppppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR");
    }

} // namespace test
