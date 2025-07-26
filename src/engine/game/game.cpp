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

#include "logging/logging.h"
#include "utils/enums_to_string.h"

#if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
#include "utils/utils.h"
#endif

namespace engine::game
{

    using namespace engine::core;
    using namespace engine::board;

    Game::Game() noexcept
        : m_state(State{})
        , m_moveList(MoveList{})
    {
    }

    Game::Game(const std::string& fenNotation)
        : m_moveList(MoveList{})
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
    }

    void Game::makeCapture(const game::Move& move, const Color enemyColor) noexcept
    {
        // Since state.unsetPiece() will delete target piece from general occupancy,
        // state.checkCastlingRemoval() in state.movePiece() won't detect
        // if we take an enemy rook or not and therefore will not be able
        // to remove the enemy castling right correctly
        m_state.checkCastlingRemoval(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        Piece toRemove = m_state.getPiece(enemyColor, move.getToSquare());
        m_state.unsetPiece(enemyColor, toRemove, move.getToSquare());
        m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        LOG_INFO("{} {} captured by {} {}", utils::toString(enemyColor), utils::toString(toRemove),
                 utils::toString(m_state.m_sideToMove), utils::toString(move.getFromPiece()));
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
            LOG_INFO("Performed {} castling", utils::toString(move.getCastlingType()));
            break;
        case Castling::WHITE_QUEEN_SIDE:
            m_state.movePiece(Piece::ROOK, 0, 3);
            LOG_INFO("Performed {} castling", utils::toString(move.getCastlingType()));
            break;
        case Castling::BLACK_KING_SIDE:
            m_state.movePiece(Piece::ROOK, 63, 61);
            LOG_INFO("Performed {} castling", utils::toString(move.getCastlingType()));
            break;
        case Castling::BLACK_QUEEN_SIDE:
            m_state.movePiece(Piece::ROOK, 56, 59);
            LOG_INFO("Performed {} castling", utils::toString(move.getCastlingType()));
            break;
        default:
            LOG_ERROR("Move castling type is invalid: {}", utils::toString(move.getCastlingType()));
            break;
        }
    }

    void Game::makeEnPassant(const Move& move, const Color enemyColor) noexcept
    {
        // Move the pawn performing enPassant
        m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        // Determine enPassant captured piece's square
        int captureSquare;
        if (m_state.m_sideToMove == Color::WHITE)
        {
            captureSquare = move.getToSquare() - 8;
        }
        else
        {
            captureSquare = move.getToSquare() + 8;
        }

        m_state.unsetPiece(enemyColor, Piece::PAWN, captureSquare);

        LOG_INFO("Performed En Passant");
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

    void Game::update(const Move& move, const Color enemyColor) noexcept
    {
        if (move.getMoveType() == MoveType::CAPTURE || move.getMoveType() == MoveType::EN_PASSANT ||
            move.getMoveType() == MoveType::PROMOTION || move.getFromPiece() == Piece::PAWN)
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
        if (move.getMoveType() == MoveType::DOUBLE_PUSH)
        {
            m_state.m_epSquare = (move.getFromSquare() + move.getToSquare()) >> 1;
        }
        // If enPassant square was set and we didn't double push, set it back to -1
        else if (m_state.m_epSquare != -1)
        {
            m_state.m_epSquare = -1;
        }

        m_state.m_sideToMove = enemyColor;

        LOG_DEBUG("===== BOARD STATE UPDATED =====");
        LOG_DEBUG("New side to move: {}", utils::toString(m_state.m_sideToMove));
        LOG_DEBUG("Half Move Clock: {} - Full Move Clock: {}", m_state.m_halfMoveClock, m_state.m_fullMoveClock);
        LOG_DEBUG("White King Side Castling Right: {} - White Queen Side Castling Right: {}",
                  m_state.hasCastlingRight<WHITE_KING_SIDE>(), m_state.hasCastlingRight<WHITE_QUEEN_SIDE>());
        LOG_DEBUG("Black King Side Castling Right: {} - Black Queen Side Castling Right: {}",
                  m_state.hasCastlingRight<BLACK_KING_SIDE>(), m_state.hasCastlingRight<BLACK_QUEEN_SIDE>());
        LOG_DEBUG("En Passant square: {}", utils::squareIndexToString(m_state.m_epSquare));
        LOG_DEBUG("===============================");

        m_moveList.generateAllMoves(m_state);

        // GAME OVER BABY
        if (m_moveList.size() == 0)
        {
            m_state.m_isCheckMate = true;
        }
    }

    void Game::playMove(const Move& move) noexcept
    {
        LOG_DEBUG("Move request: [From square: {}] - [To square: {}] - [Move type: {}] - [From piece: {}]",
                  utils::squareIndexToString(move.getFromSquare()), utils::squareIndexToString(move.getToSquare()),
                  utils::toString(move.getMoveType()), utils::toString(move.getFromPiece()));

        Color enemyColor = m_state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        // If move is a capture, move the from piece and remove the target piece
        if (move.getMoveType() == MoveType::CAPTURE)
        {
            this->makeCapture(move, enemyColor);
        }
        // If move is a castling, move to according rook
        else if (move.getMoveType() == MoveType::CASTLE) [[unlikely]]
        {
            this->makeCastling(move);
        }
        // If move is an enPassant, remove the target piece
        else if (move.getMoveType() == MoveType::EN_PASSANT) [[unlikely]]
        {
            this->makeEnPassant(move, enemyColor);
        }
        // If no special move, just move the piece
        else
        {
            m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());
        }

        // If promotion piece is set, make promotion
        if (move.isPromotion()) [[unlikely]]
        {
            this->makePromotion(move);
        }

        this->update(move, enemyColor);
    }
} // namespace engine::game