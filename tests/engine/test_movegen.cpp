/**
 * @file      test_movegen.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Unit testing of various legal and illegal moves.
 */

#include "test_utils.h"

namespace test
{
    using namespace engine::core;
    using namespace engine::game;
    using namespace engine::board;

    // ---------------------------------------------------------------------
    // QUIET MOVES (non-capturing moves)
    // ---------------------------------------------------------------------

    // Scenario: White pawn single-step move from initial position (e2 to e3).
    TEST(MoveGen, QuietPawnPush_singleStep_initialPosition)
    {
        Game game{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
        Move tested{12, 20, MoveType::QUIET, Piece::PAWN}; // e2 -> e3 (single step)

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White pawn double-step move from initial position (e2 to e4).
    TEST(MoveGen, QuietPawnPush_doubleStep_initialPosition)
    {
        Game game{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
        Move tested{12, 28, MoveType::DOUBLE_PUSH, Piece::PAWN}; // e2 -> e4 (double step)

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White knight moves from initial position (b1 to c3).
    TEST(MoveGen, QuietKnightMove_initialPosition)
    {
        Game game{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
        Move tested{1, 18, MoveType::QUIET, Piece::KNIGHT}; // b1 -> c3

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White bishop moves along a clear diagonal (bishop at c4 moves to a2).
    TEST(MoveGen, QuietBishopMove_diagonal_openPosition)
    {
        Game game{"k7/8/8/8/2B5/8/8/4K3 w - - 0 1"};        // White bishop at c4, kings at a8 and e1
        Move tested{26, 8, MoveType::QUIET, Piece::BISHOP}; // c4 -> a2

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White rook moves along an open rank (rook at a4 moves to h4).
    TEST(MoveGen, QuietRookMove_horizontal_openPosition)
    {
        Game game{"7k/8/8/8/R7/8/8/4K3 w - - 0 1"};        // White rook at a4, black king at h8, white king at e1
        Move tested{24, 31, MoveType::QUIET, Piece::ROOK}; // a4 -> h4

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White queen moves along a clear diagonal (queen at d4 moves to h8).
    TEST(MoveGen, QuietQueenMove_diagonal_openPosition)
    {
        Game game{"k7/8/8/8/3Q4/8/8/4K3 w - - 0 1"};        // White queen at d4, black king at a8, white king at e1
        Move tested{27, 63, MoveType::QUIET, Piece::QUEEN}; // d4 -> h8

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White king moves one square in an open board (king at e1 moves to e2).
    TEST(MoveGen, QuietKingMove_oneStep_noCheck)
    {
        Game game{"4k3/8/8/8/8/8/8/4K3 w - - 0 1"};       // Only kings on board (black king e8, white king e1)
        Move tested{4, 12, MoveType::QUIET, Piece::KING}; // e1 -> e2

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // ---------------------------------------------------------------------
    // CAPTURE MOVES (capturing an enemy piece)
    // ---------------------------------------------------------------------

    // Scenario: White pawn captures diagonally (pawn at e4 captures enemy pawn at d5).
    TEST(MoveGen, CapturePawn_diagonal)
    {
        Game game{"k7/8/8/3p4/4P3/8/8/4K3 w - - 0 1"};       // White pawn at e4, black pawn at d5
        Move tested{28, 35, MoveType::CAPTURE, Piece::PAWN}; // e4 -> d5

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White knight captures in L-shape (knight at d4 captures enemy pawn at e6).
    TEST(MoveGen, CaptureKnight_LShape)
    {
        Game game{"k7/8/4p3/8/3N4/8/8/4K3 w - - 0 1"};         // White knight at d4, black pawn at e6
        Move tested{27, 44, MoveType::CAPTURE, Piece::KNIGHT}; // d4 -> e6

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White bishop captures along a diagonal (bishop at c4 captures enemy knight at f1).
    TEST(MoveGen, CaptureBishop_diagonal)
    {
        Game game{"k7/8/8/8/2B5/8/8/4Kn2 w - - 0 1"};         // White bishop at c4, black knight at f1
        Move tested{26, 5, MoveType::CAPTURE, Piece::BISHOP}; // c4 -> f1

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White rook captures along a rank (rook at a4 captures enemy knight at e4).
    TEST(MoveGen, CaptureRook_horizontal)
    {
        Game game{"7k/8/8/8/R3n3/8/8/4K3 w - - 0 1"};        // White rook at a4, black knight at e4
        Move tested{24, 28, MoveType::CAPTURE, Piece::ROOK}; // a4 -> e4

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White queen captures along a diagonal (queen at d4 captures enemy bishop at h8).
    TEST(MoveGen, CaptureQueen_diagonal)
    {
        Game game{"k6b/8/8/8/3Q4/8/8/4K3 w - - 0 1"};         // White queen at d4, black bishop at h8
        Move tested{27, 63, MoveType::CAPTURE, Piece::QUEEN}; // d4 -> h8

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White king captures an adjacent enemy piece (king at e1 captures enemy knight at d2).
    TEST(MoveGen, CaptureKing_adjacent)
    {
        Game game{"k7/8/8/8/8/8/3n4/4K3 w - - 0 1"};        // White king at e1, black knight at d2
        Move tested{4, 11, MoveType::CAPTURE, Piece::KING}; // e1 -> d2

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // ---------------------------------------------------------------------
    // CASTLING MOVES (special king-rook moves)
    // ---------------------------------------------------------------------

    // Scenario: White can castle king side (no pieces between and not in check).
    TEST(MoveGen, Castling_WhiteKingSide_allowed)
    {
        // White to move, white king at e1, rooks at a1/h1, KQkq available (FEN_2 defines this position)
        Game game{FEN_2};
        Move tested{4, 6, MoveType::CASTLE, Piece::KING, Castling::WHITE_KING_SIDE}; // White O-O: e1 -> g1

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White can castle queen side (no pieces between and not in check).
    TEST(MoveGen, Castling_WhiteQueenSide_allowed)
    {
        Game game{FEN_2}; // Same position as above, white can also castle long
        Move tested{4, 2, MoveType::CASTLE, Piece::KING, Castling::WHITE_QUEEN_SIDE}; // White O-O-O: e1 -> c1

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: Black can castle king side (no pieces between and not in check, black to move).
    TEST(MoveGen, Castling_BlackKingSide_allowed)
    {
        // Use FEN_2 position but set black to move (black king at e8, rooks at a8/h8, rights kq).
        Game game{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1"};
        Move tested{60, 62, MoveType::CASTLE, Piece::KING, Castling::BLACK_KING_SIDE}; // Black O-O: e8 -> g8

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: Black can castle queen side (no pieces between and not in check, black to move).
    TEST(MoveGen, Castling_BlackQueenSide_allowed)
    {
        Game game{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1"};
        Move tested{60, 58, MoveType::CASTLE, Piece::KING, Castling::BLACK_QUEEN_SIDE}; // Black O-O-O: e8 -> c8

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: White king side castling is disallowed when the king is in check.
    TEST(MoveGen, Castling_WhiteKingSide_disallowed_inCheck)
    {
        // White king at e1 is in check from a black rook on e2; castling should not appear.
        // White rook h1, white king e1 (K rights), black rook e2 giving check
        Game game{"4k3/8/8/8/8/8/4r3/4K2R w K - 0 1"};
        Move tested{4, 6, MoveType::CASTLE, Piece::KING, Castling::WHITE_KING_SIDE}; // e1 -> g1 attempt

        EXPECT_FALSE(game.m_moveList.contains(tested));
    }

    // Scenario: White king side castling is disallowed when a square on the path is under attack.
    TEST(MoveGen, Castling_WhiteKingSide_disallowed_pathAttacked)
    {
        // White king at e1, rook at h1, not in check, but black rook on f3 attacks f1 (the path).
        // f1 (path square) is attacked by black rook at f3
        Game game{"4k3/8/8/8/8/5r2/8/4K2R w K - 0 1"};
        Move tested{4, 6, MoveType::CASTLE, Piece::KING, Castling::WHITE_KING_SIDE};

        EXPECT_FALSE(game.m_moveList.contains(tested));
    }

    // Scenario: White king side castling is disallowed when pieces are blocking the path (initial position).
    TEST(MoveGen, Castling_WhiteKingSide_disallowed_blocked)
    {
        // Initial position: pieces between king and rook
        Game game{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
        Move tested{4, 6, MoveType::CASTLE, Piece::KING, Castling::WHITE_KING_SIDE};

        EXPECT_FALSE(game.m_moveList.contains(tested));
    }

    // ---------------------------------------------------------------------
    // EN PASSANT MOVES (special pawn captures)
    // ---------------------------------------------------------------------

    // Scenario: White pawn performs en passant capture to the right (white pawn at g5 captures black pawn en passant on
    // h6).
    TEST(MoveGen, EnPassant_WhitePawn_captureRight)
    {
        // Position after black pawn moved from h7 to h5 (en passant target h6, white pawn at g5 ready).
        // White pawn g5, black pawn h5, ep target h6
        Game game{"k7/8/8/6Pp/8/8/8/K7 w - h6 0 1"};
        Move tested{38, 47, MoveType::EN_PASSANT, Piece::PAWN}; // g5 -> h6 en passant

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // Scenario: Black pawn performs en passant capture to the left (black pawn at c4 captures white pawn en passant on
    // d3).
    TEST(MoveGen, EnPassant_BlackPawn_captureLeft)
    {
        // Position after white pawn moved from d2 to d4 (en passant target d3, black pawn at c4 ready).
        // Black pawn c4, white pawn d4, ep target d3
        Game game{"k7/8/8/8/2pP4/8/8/K7 b - d3 0 1"};
        Move tested{26, 19, MoveType::EN_PASSANT, Piece::PAWN}; // c4 -> d3 en passant

        EXPECT_TRUE(game.m_moveList.contains(tested));
    }

    // ---------------------------------------------------------------------
    // PROMOTION MOVES (pawn reaching last rank)
    // ---------------------------------------------------------------------

    // Scenario: White pawn promotion on reaching the last rank without capture (e7 to e8 = Q/R/B/N).
    TEST(MoveGen, Promotion_WhitePawn_noCapture)
    {
        Game game{"k7/4P3/8/8/8/8/8/7K w - - 0 1"}; // White pawn at e7, e8 empty

        // Test that all promotion pieces are available
        Move promoKnight{52, 60, MoveType::QUIET, Piece::PAWN};
        promoKnight.setPromotionPiece(Piece::KNIGHT);

        Move promoBishop{52, 60, MoveType::QUIET, Piece::PAWN};
        promoBishop.setPromotionPiece(Piece::BISHOP);

        Move promoRook{52, 60, MoveType::QUIET, Piece::PAWN};
        promoRook.setPromotionPiece(Piece::ROOK);

        Move promoQueen{52, 60, MoveType::QUIET, Piece::PAWN};
        promoQueen.setPromotionPiece(Piece::QUEEN);

        EXPECT_TRUE(game.m_moveList.contains(promoKnight));
        EXPECT_TRUE(game.m_moveList.contains(promoBishop));
        EXPECT_TRUE(game.m_moveList.contains(promoRook));
        EXPECT_TRUE(game.m_moveList.contains(promoQueen));
    }

    // Scenario: White pawn promotion with capture (pawn at g7 captures rook at h8 and promotes).
    TEST(MoveGen, Promotion_WhitePawn_capture)
    {
        Game game{"k6r/6P1/8/8/8/8/8/6K1 w - - 0 1"}; // White pawn at g7, black rook at h8

        // Test a couple of promotion outcomes on capture
        Move promoQueen{54, 63, MoveType::CAPTURE, Piece::PAWN};
        promoQueen.setPromotionPiece(Piece::QUEEN); // g7 -> h8=Q

        Move promoKnight{54, 63, MoveType::CAPTURE, Piece::PAWN};
        promoKnight.setPromotionPiece(Piece::KNIGHT); // g7 -> h8=N

        EXPECT_TRUE(game.m_moveList.contains(promoQueen));
        EXPECT_TRUE(game.m_moveList.contains(promoKnight));
    }

    // Scenario: Black pawn promotion on reaching the last rank without capture (e2 to e1 = Q/R/B/N).
    TEST(MoveGen, Promotion_BlackPawn_noCapture)
    {
        Game game{"4k3/8/8/8/8/8/4p3/7K b - - 0 1"}; // Black pawn at e2, e1 empty

        Move promoQueen{12, 4, MoveType::QUIET, Piece::PAWN};
        promoQueen.setPromotionPiece(Piece::QUEEN); // e2 -> e1=Q

        Move promoRook{12, 4, MoveType::QUIET, Piece::PAWN};
        promoRook.setPromotionPiece(Piece::ROOK); // e2 -> e1=R

        Move promoBishop{12, 4, MoveType::QUIET, Piece::PAWN};
        promoBishop.setPromotionPiece(Piece::BISHOP); // e2 -> e1=B

        Move promoKnight{12, 4, MoveType::QUIET, Piece::PAWN};
        promoKnight.setPromotionPiece(Piece::KNIGHT); // e2 -> e1=N

        EXPECT_TRUE(game.m_moveList.contains(promoQueen));
        EXPECT_TRUE(game.m_moveList.contains(promoRook));
        EXPECT_TRUE(game.m_moveList.contains(promoBishop));
        EXPECT_TRUE(game.m_moveList.contains(promoKnight));
    }

    // Scenario: Black pawn promotion with capture (pawn at b2 captures knight at a1 and promotes).
    TEST(MoveGen, Promotion_BlackPawn_capture)
    {
        Game game{"4k3/8/8/8/8/8/1p6/N6K b - - 0 1"}; // Black pawn at b2, white knight at a1

        Move promoQueen{9, 0, MoveType::CAPTURE, Piece::PAWN};
        promoQueen.setPromotionPiece(Piece::QUEEN); // b2 -> a1=Q

        Move promoKnight{9, 0, MoveType::CAPTURE, Piece::PAWN};
        promoKnight.setPromotionPiece(Piece::KNIGHT); // b2 -> a1=N

        EXPECT_TRUE(game.m_moveList.contains(promoQueen));
        EXPECT_TRUE(game.m_moveList.contains(promoKnight));
    }

    // ---------------------------------------------------------------------
    // ILLEGAL MOVES (moves that should NOT appear in the legal move list)
    // ---------------------------------------------------------------------

    // Scenario: A pinned white pawn cannot move as it would expose its king to check (pawn at e2 pinned by rook on e8).
    TEST(MoveGen, IllegalMove_PinnedPawn)
    {
        // White pawn at e2, white king d2, black rook h2 pinning the pawn
        Game game{"k7/8/8/8/8/8/3KP2r/8 w - - 0 1"};

        Move testedPush{12, 20, MoveType::QUIET, Piece::PAWN};         // e2 -> e3 (would expose king)
        Move testedDouble{12, 28, MoveType::DOUBLE_PUSH, Piece::PAWN}; // e2 -> e4 (would expose king)

        EXPECT_FALSE(game.m_moveList.contains(testedPush));
        EXPECT_FALSE(game.m_moveList.contains(testedDouble));
    }

    // Scenario: A knight move that does not block check is illegal when the king is in check (white king in check from
    // a rook, knight moves away).
    TEST(MoveGen, IllegalMove_KnightIgnoresCheck)
    {
        Game game{"k3r3/8/8/8/8/8/8/4K1N1 w - - 0 1"};      // White king e1 in check by black rook e8, white knight g1
        Move tested{6, 23, MoveType::QUIET, Piece::KNIGHT}; // Knight g1 -> h3 (does not block or capture the rook)

        EXPECT_FALSE(game.m_moveList.contains(tested));
    }

} // namespace test
