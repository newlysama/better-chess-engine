/**
 * @file      test_game.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Tests the Game::makeMove and Game::unmakeMove methods.
 */

#include "test_utils.h"

namespace test
{
    using namespace engine::core;
    using namespace engine::game;
    using namespace engine::board;

    TEST(GameTest, PawnDoublePushWhite)
    {
        Game game("7k/8/8/8/8/8/P7/7K w - - 3 1");
        State initialState = game.m_state;

        Move move(8, 24, MoveType::DOUBLE_PUSH, Piece::PAWN); // a2 -> a4
        game.makeMove<true>(move);

        Game expected("7k/8/8/8/P7/8/8/7K b - a3 0 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnEnPassantCaptureWhite)
    {
        Game game("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 2");
        State initialState = game.m_state;

        Move move(36, 43, MoveType::EN_PASSANT, Piece::PAWN); // e5 -> d6 en passant
        game.makeMove<true>(move);

        Game expected("4k3/8/8/8/8/8/8/4K3 b - - 0 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnEnPassantNotUsed)
    {
        // White pawn ignores en passant capture and advances forward
        Game game("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 2");
        State initialState = game.m_state;

        Move move(36, 44, MoveType::QUIET, Piece::PAWN); // e5 -> e6
        game.makeMove<true>(move);

        Game expected("4k3/8/8/8/3p4/4P3/8/4K3 b - - 0 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnPromotionWhiteQuiet)
    {
        Game game("7k/4P3/8/8/8/8/8/7K w - - 5 1");
        State initialState = game.m_state;

        Move move(52, 60, MoveType::PROMOTION, Piece::PAWN); // e7 -> e8 promotion
        move.setPromotionPiece(Piece::QUEEN);
        game.makeMove<true>(move);

        Game expected("4Q2k/8/8/8/8/8/8/7K b - - 0 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnPromotionWhiteCapture)
    {
        Game game("k4n2/4P3/8/8/8/8/8/7K w - - 5 1");
        State initialState = game.m_state;

        Move move(52, 61, MoveType::CAPTURE, Piece::PAWN); // e7 -> f8 capture & promote
        move.setPromotionPiece(Piece::KNIGHT);

        game.makeMove<true>(move);
        Game expected("k4N2/8/8/8/8/8/8/7K b - - 0 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingWhiteKingSide)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 1 1");
        State initialState = game.m_state;

        Move move(4, 6, MoveType::CASTLE, Piece::KING, Castling::WHITE_KING_SIDE); // white O-O
        game.makeMove<true>(move);

        Game expected("r3k2r/8/8/8/8/8/8/R4RK1 b Qkq - 2 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingWhiteQueenSide)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 1 1");
        State initialState = game.m_state;

        Move move(4, 2, MoveType::CASTLE, Piece::KING, Castling::WHITE_QUEEN_SIDE); // white O-O-O
        game.makeMove<true>(move);

        Game expected("r3k2r/8/8/8/8/8/8/2KR3R b kq - 2 1");
        // After white queen-side castling: white king at c1, rook from a1 to d1
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingBlackKingSide)
    {
        Game game("r3k3/8/8/8/8/8/8/4K3 b k - 0 1");
        // Only black king at e8 and rook at h8 have rights "k"
        State initialState = game.m_state;

        Move move(60, 62, MoveType::CASTLE, Piece::KING, Castling::BLACK_KING_SIDE); // black O-O
        game.makeMove<true>(move);

        Game expected("r3k3/8/8/8/8/8/8/4K3 w - - 1 1");
        // Black king should move to g8 and rook from h8 to f8, leaving no castling rights
        expected.m_state.m_castlingRights = 0; // no rights
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingBlackQueenSide)
    {
        Game game("r3k3/8/8/8/8/8/8/4K3 b q - 3 2");
        // Black king at e8, rook at a8, queen-side right only
        State initialState = game.m_state;

        Move move(60, 58, MoveType::CASTLE, Piece::KING, Castling::BLACK_QUEEN_SIDE); // black O-O-O
        game.makeMove<true>(move);

        Game expected("2kr4/8/8/8/8/8/8/4K3 w - - 4 2");
        // Black king should move to c8, rook from a8 to d8, no castling rights left
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, RookMoveClearsCastlingRights)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
        // Full castling rights for both; white will move h1 rook
        State initialState = game.m_state;

        Move move(7, 23, MoveType::QUIET, Piece::ROOK); // white rook h1 -> h3
        game.makeMove<true>(move);

        Game expected("r3k2r/8/8/7R/8/8/8/R3K3 b qk - 1 2");
        // After move: white rook at h3, white loses king-side castling (K removed), black rights remain
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CaptureRemovesCastlingRights)
    {
        Game game("r3k3/8/8/Q7/8/8/8/7K w q - 5 3");
        // Black has queen-side castling (rook at a8), white queen at a5 will capture it
        State initialState = game.m_state;

        Move move(32, 56, MoveType::CAPTURE, Piece::QUEEN); // white queen a5 -> a8 captures rook
        game.makeMove<true>(move);

        Game expected("Q3k3/8/8/8/8/8/8/7K b - - 0 4");
        // After capture: white queen at a8, black rook gone, black loses castling rights
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, KnightQuietMove)
    {
        Game game("7k/8/8/8/8/8/8/1N2K3 w - - 4 1");
        State initialState = game.m_state;

        Move move(1, 18, MoveType::QUIET, Piece::KNIGHT); // white knight b1 -> c3
        game.makeMove<true>(move);

        Game expected("7k/8/8/2N5/8/8/8/4K3 b - - 5 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, MakeUnmakeMaintainsStateOnCheckmate)
    {
        // Scenario where a move delivers checkmate (no moves for opponent).
        Game game("k7/8/1K6/3Q4/8/8/8/8 w - - 1 1");
        State initialState = game.m_state;

        Move move(35, 49, MoveType::QUIET, Piece::QUEEN); // Qd5 -> Qb7 (checkmate)
        game.makeMove<true>(move);

        // After move: Black has no legal moves (checkmate). Unmake and compare states.
        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }
} // namespace test