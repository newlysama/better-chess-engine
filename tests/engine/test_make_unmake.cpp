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

    // ---------------------------------------------------------------------
    // 1. PAWNS
    // ---------------------------------------------------------------------

    TEST(GameTest, PawnDoublePushWhite)
    {
        Game game("7k/8/8/8/8/8/P7/7K w - - 3 1");
        State initialState = game.m_state;

        Move move(8, 24, MoveType::DOUBLE_PUSH, Piece::PAWN); // a2 -> a4
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("7k/8/8/8/P7/8/8/7K b - a3 0 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnDoublePushBlack)
    {
        Game game("7K/7p/8/8/8/8/8/7k b - - 0 1");
        State snapshot = game.m_state;

        Move move(55, 39, MoveType::DOUBLE_PUSH, Piece::PAWN); // h7 -> h5
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("7K/8/8/7p/8/8/8/7k w - h6 0 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, snapshot);
    }

    TEST(GameTest, PawnEnPassantCaptureWhite)
    {
        Game game("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 2");
        State initialState = game.m_state;

        Move move(36, 43, MoveType::EN_PASSANT, Piece::PAWN); // e5 -> d6 e.p.
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("4k3/8/3P4/8/8/8/8/4K3 b - - 0 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnEnPassantCaptureBlack)
    {
        Game game("4k3/8/8/8/3pP3/8/8/4K3 b - e3 0 1");
        State snap = game.m_state;

        Move move(27, 20, MoveType::EN_PASSANT, Piece::PAWN); // d4 -> e3 e.p.
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("4k3/8/8/8/8/4p3/8/4K3 w - - 0 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, snap);
    }

    TEST(GameTest, PawnEnPassantNotUsed)
    {
        Game game("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 2");
        State initialState = game.m_state;

        Move move(36, 44, MoveType::QUIET, Piece::PAWN); // e5 -> e6
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("4k3/8/4P3/3p4/8/8/8/4K3 b - - 0 2");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnPromotionWhiteQuiet)
    {
        Game game("7k/4P3/8/8/8/8/8/7K w - - 5 1");
        State initialState = game.m_state;

        Move move(52, 60, MoveType::QUIET, Piece::PAWN); // e7 -> e8=Q
        ASSERT_EQ(move.getMoveType(), MoveType::QUIET);
        move.setPromotionPiece(Piece::QUEEN);
        ASSERT_TRUE(move.isPromotion());
        ASSERT_EQ(move.getPromotionPiece(), Piece::QUEEN);
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("4Q2k/8/8/8/8/8/8/7K b - - 0 1");
        expected.m_state.computeEnemyTargetedSquares();
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, PawnPromotionBlackQuiet)
    {
        // Pion noir e2 va en e1 = Dame
        Game game("7K/8/8/8/8/8/4p3/7k b - - 0 1");
        State snap = game.m_state;

        Move move(12, 4, MoveType::QUIET, Piece::PAWN); // e2->e1
        ASSERT_EQ(move.getMoveType(), MoveType::QUIET);
        move.setPromotionPiece(Piece::QUEEN);
        ASSERT_TRUE(move.isPromotion());
        ASSERT_EQ(move.getPromotionPiece(), Piece::QUEEN);
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("7K/8/8/8/8/8/8/4q2k w - - 0 2");
        expected.m_state.computeEnemyTargetedSquares();
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, snap);
    }

    // ---------------------------------------------------------------------
    // 2. CASTLINGS
    // ---------------------------------------------------------------------

    TEST(GameTest, CastlingWhiteKingSide)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 1 1");
        State initialState = game.m_state;

        Move move(4, 6, MoveType::CASTLE, Piece::KING, Castling::WHITE_KING_SIDE);
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("r3k2r/8/8/8/8/8/8/R4RK1 b kq - 2 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingWhiteQueenSide)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 1 1");
        State initialState = game.m_state;

        Move move(4, 2, MoveType::CASTLE, Piece::KING, Castling::WHITE_QUEEN_SIDE);
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("r3k2r/8/8/8/8/8/8/2KR3R b kq - 2 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingBlackKingSide)
    {
        Game game("r3k2r/8/8/8/8/8/8/4K3 b k - 0 1");
        State initialState = game.m_state;

        Move move(60, 62, MoveType::CASTLE, Piece::KING, Castling::BLACK_KING_SIDE);
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("r4rk1/8/8/8/8/8/8/4K3 w - - 1 2");
        expected.m_state.m_castlingRights = 0;
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CastlingBlackQueenSide)
    {
        Game game("r3k3/8/8/8/8/8/8/4K3 b q - 3 2");
        State initialState = game.m_state;

        Move move(60, 58, MoveType::CASTLE, Piece::KING, Castling::BLACK_QUEEN_SIDE);
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("2kr4/8/8/8/8/8/8/4K3 w - - 4 3");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, RookMoveClearsCastlingRights)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
        State initialState = game.m_state;

        Move move(7, 23, MoveType::QUIET, Piece::ROOK); // h1 -> h3
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("r3k2r/8/8/8/8/7R/8/R3K3 b Qkq - 1 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, KingMoveClearsBothCastlingRights)
    {
        Game game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
        State snap = game.m_state;

        Move move(4, 5, MoveType::QUIET, Piece::KING); // e1 -> f1
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("r3k2r/8/8/8/8/8/8/R4K1R b kq - 1 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, snap);
    }

    // ---------------------------------------------------------------------
    // 3. QUIET MOVES
    // ---------------------------------------------------------------------

    TEST(GameTest, KnightQuietMove)
    {
        Game game("7k/8/8/8/8/8/8/1N2K3 w - - 4 1");
        State initialState = game.m_state;

        Move move(1, 18, MoveType::QUIET, Piece::KNIGHT); // b1 -> c3
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("7k/8/8/8/8/2N5/8/4K3 b - - 5 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

    TEST(GameTest, CaptureNeutralNoCastlingEffect)
    {
        Game game("7k/8/8/3b4/8/4N3/8/7K w - - 0 1");
        State snap = game.m_state;

        Move move(20, 35, MoveType::CAPTURE, Piece::KNIGHT); // e3 -> d5
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        Game expected("7k/8/8/3N4/8/8/8/7K b - - 0 1");
        compareStates(game.m_state, expected.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, snap);
    }

    // ---------------------------------------------------------------------
    // 4. Check flags
    // ---------------------------------------------------------------------

    TEST(GameTest, MakeUnmakeMaintainsStateOnCheckmate)
    {
        Game game("k7/8/1K6/3Q4/8/8/8/8 w - - 1 1");
        State initialState = game.m_state;

        Move move(35, 49, MoveType::QUIET, Piece::QUEEN); // Qd5 -> Qb7 (checkmate)
        game.makeMove<true>(move);
        game.m_moveList.generateAllMoves(game.m_state);

        game.unmakeMove(move);
        compareStates(game.m_state, initialState);
    }

} // namespace test
