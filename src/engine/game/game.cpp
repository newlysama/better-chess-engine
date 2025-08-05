/**
 * @file      game.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game class implementation.
 */

#include "engine/game/game.h"

#include <exception>

#include "utils/enums_to_string.h"
#include "utils/utils.h"

namespace engine::game
{

    using namespace engine::core;
    using namespace engine::board;

    Game::Game() noexcept
        : m_state(State{})
        , m_moveList(MoveList{})
        , m_unmakeStack{}
    {
        m_moveList.generateAllMoves(m_state);
    }

    Game::Game(const std::string& fenNotation)
        : m_moveList(MoveList{})
        , m_unmakeStack{}
    {
        std::array<std::string, 6> parts;
        std::string token;
        std::istringstream iss(fenNotation);

        for (std::size_t i = 0; i < 6; i++)
        {
            if (!std::getline(iss, token, ' '))
            {
                throw std::invalid_argument("FEN's occupancy part error: less than 8 fields.");
            }

            parts[i] = token;
        }

        m_state = State{parts};

        m_moveList.generateAllMoves(m_state);
    }

    void Game::makeCapture(const Move& move, const Color& enemyColor, const Piece& enemyPiece) noexcept
    {
        // Check if we capture a rook, which would remove a castling right
        if (enemyPiece == Piece::ROOK)
        {
            if (enemyColor == Color::WHITE)
            {
                if (move.getToSquare() == 56) // Initial queen side black rook pos
                    m_state.clearCastlingRight<BLACK_QUEEN_SIDE>();
                else if (move.getToSquare() == 63) // Initial king side black rook pos
                    m_state.clearCastlingRight<BLACK_KING_SIDE>();
            }
            else
            {
                if (move.getToSquare() == 0) // Initial queen side white rook pos
                    m_state.clearCastlingRight<WHITE_QUEEN_SIDE>();
                else if (move.getToSquare() == 7) // Initial king side white rook pos
                    m_state.clearCastlingRight<WHITE_KING_SIDE>();
            }
        }

        m_state.unsetPiece(enemyColor, enemyPiece, move.getToSquare());
        m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        LOG_INFO("{} {} captured by {} {}", utils::toString(enemyColor), utils::toString(enemyPiece),
                 utils::toString(m_state.m_sideToMove), utils::toString(move.getFromPiece()));
    }

    void Game::unmakeCapture(const Move& move, const UnmakeInfo& unmakeInfo) noexcept
    {
        m_state.movePiece(move.getFromPiece(), move.getToSquare(), move.getFromSquare());
        m_state.setPiece(unmakeInfo.capColor, unmakeInfo.capPiece, unmakeInfo.capSquare);

        LOG_INFO("Unmade {} {} ({}) capture on {} {} ({})", utils::toString(m_state.m_sideToMove),
                 utils::toString(move.getFromPiece()), utils::squareIndexToString(move.getFromSquare()),
                 utils::toString(unmakeInfo.capColor), utils::toString(unmakeInfo.capPiece),
                 utils::squareIndexToString(unmakeInfo.capSquare));
    }

    void Game::makeCastling(const Move& move) noexcept
    {
        // Move the king
        m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        // Move the rook
        switch (move.getCastlingType())
        {
        case Castling::WHITE_KING_SIDE:
            m_state.movePiece(Piece::ROOK, 7, 5);
            break;
        case Castling::WHITE_QUEEN_SIDE:
            m_state.movePiece(Piece::ROOK, 0, 3);
            break;
        case Castling::BLACK_KING_SIDE:
            m_state.movePiece(Piece::ROOK, 63, 61);
            break;
        case Castling::BLACK_QUEEN_SIDE:
            m_state.movePiece(Piece::ROOK, 56, 59);
            break;
        default:
            LOG_ERROR("Move castling type is invalid: {}", utils::toString(move.getCastlingType()));
            break;
        }

        LOG_INFO("Performed {} castling", utils::toString(move.getCastlingType()));
    }

    void Game::unmakeCastling(const Move& move) noexcept
    {
        // Move the king
        m_state.movePiece(move.getFromPiece(), move.getToSquare(), move.getFromSquare());

        // Move the rook
        switch (move.getCastlingType())
        {
        case Castling::WHITE_KING_SIDE:
            m_state.movePiece(Piece::ROOK, 5, 7);
            break;
        case Castling::WHITE_QUEEN_SIDE:
            m_state.movePiece(Piece::ROOK, 3, 0);
            break;
        case Castling::BLACK_KING_SIDE:
            m_state.movePiece(Piece::ROOK, 61, 63);
            break;
        case Castling::BLACK_QUEEN_SIDE:
            m_state.movePiece(Piece::ROOK, 59, 56);
            break;
        default:
            LOG_ERROR("Move castling type is invalid: {}", utils::toString(move.getCastlingType()));
            break;
        }

        LOG_INFO("Unmade {} castling", utils::toString(move.getCastlingType()));
    }

    void Game::makeEnPassant(const Move& move, const Color& enemyColor, const int& capturedSquare) noexcept
    {
        // Move the pawn performing enPassant
        m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        // Determine enPassant captured piece's square
        m_state.unsetPiece(enemyColor, Piece::PAWN, capturedSquare);

        LOG_INFO("Performed En Passant");
    }

    void Game::unmakeEnPassant(const Move& move, const UnmakeInfo& unmakeInfo) noexcept
    {
        m_state.movePiece(move.getFromPiece(), move.getToSquare(), move.getFromSquare());
        m_state.setPiece(unmakeInfo.capColor, Piece::PAWN, unmakeInfo.capSquare);

        LOG_INFO("Unmade En Passant");
    }

    void Game::makePromotion(const game::Move& move) noexcept
    {
        if (move.getToSquare() >= 56) // Rank 8 = White promotion
        {
            m_state.unsetPiece(Color::WHITE, Piece::PAWN, move.getToSquare());
            m_state.setPiece(Color::WHITE, move.getPromotionPiece(), move.getToSquare());

            LOG_INFO("Performed White Promotion");
        }
        else // Black promotion
        {
            m_state.unsetPiece(Color::BLACK, Piece::PAWN, move.getToSquare());
            m_state.setPiece(Color::BLACK, move.getPromotionPiece(), move.getToSquare());

            LOG_INFO("Performed Black Promotion");
        }
    }

    void Game::unmakePromotion(const game::Move& move) noexcept
    {
        m_state.unsetPiece(m_state.m_sideToMove, move.getPromotionPiece(), move.getToSquare());
        m_state.setPiece(m_state.m_sideToMove, Piece::PAWN, move.getFromSquare());

        LOG_INFO("Unmade Promotion");
    }

    void Game::update(const Move& move, const Color& enemyColor) noexcept
    {
        MoveType moveType = move.getMoveType();

        if (moveType == MoveType::CAPTURE || move.getFromPiece() == Piece::PAWN)
        {
            m_state.m_halfMoveClock = 0;
        }
        else
        {
            m_state.m_halfMoveClock++;
        }

        // If black just played, increase full move clock
        if (m_state.m_sideToMove == Color::BLACK)
        {
            m_state.m_fullMoveClock++;
        }

        // If pawn double push was played, set enPassant square
        if (moveType == MoveType::DOUBLE_PUSH)
        {
            m_state.m_epSquare = (move.getFromSquare() + move.getToSquare()) >> 1;
        }
        // If enPassant square was set and we didn't double push, set it back to -1
        else if (m_state.m_epSquare != -1)
        {
            m_state.m_epSquare = -1;
        }

        m_state.m_sideToMove = enemyColor;
    }

    void Game::unmakeMove(const game::Move& move) noexcept
    {
        LOG_INFO("Unmake move request: [From square: {}] - [To square: {}] - [Move type: {}] - [From piece: {}]",
                 utils::squareIndexToString(move.getFromSquare()), utils::squareIndexToString(move.getToSquare()),
                 utils::toString(move.getMoveType()), utils::toString(move.getFromPiece()));

        m_state.m_sideToMove = m_state.getEnemyColor();

        m_stackSize--;

        UnmakeInfo unmakeInfo = m_unmakeStack[m_stackSize];
        MoveType moveType = move.getMoveType();

        // Check promotion first because the original
        // fromPiece of the move has been replaced
        if (move.isPromotion()) [[unlikely]]
        {
            this->unmakePromotion(move);
        }
        if (moveType == MoveType::CAPTURE)
        {
            this->unmakeCapture(move, unmakeInfo);
        }
        else if (moveType == MoveType::CASTLE) [[unlikely]]
        {
            this->unmakeCastling(move);
        }
        else if (moveType == MoveType::EN_PASSANT) [[unlikely]]
        {
            this->unmakeEnPassant(move, unmakeInfo);
        }
        else
        {
            m_state.movePiece(move.getFromPiece(), move.getToSquare(), move.getFromSquare());
        }

        m_state.m_castlingRights = unmakeInfo.prevCastlingRights;
        m_state.m_halfMoveClock = unmakeInfo.prevHalfMoveClock;
        m_state.m_fullMoveClock = unmakeInfo.prevFullMoveClock;
        m_state.m_epSquare = unmakeInfo.prevEpSquare;

        m_state.m_isChecked = unmakeInfo.prevIsChecked;
        m_state.m_isDoubleChecked = unmakeInfo.prevIsDoubleCheked;
        m_state.m_isCheckMate = unmakeInfo.prevIsCheckmate;

        m_state.m_pinnedBB = unmakeInfo.prevPinnedBB;
        m_state.m_targetsBB = unmakeInfo.prevTargetsBB;
        m_state.m_checkersBB = unmakeInfo.prevCheckersBB;
        m_state.m_blockersBB = unmakeInfo.prevBlockersBB;

        m_moveList = m_moveListStack[m_stackSize];
    }
} // namespace engine::game