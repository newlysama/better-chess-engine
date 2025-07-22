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

/**
 * @namespace engine::game
 */
namespace engine::game
{

    using namespace engine::core;
    using namespace engine::board;

    Game::Game() noexcept
        : state(State{})
        , moveList(MoveList{})
    {
    }

    Game::Game(const std::string& fenNotation)
        : moveList(MoveList{})
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

        state = State{parts};
    }

    void Game::makeCapture(const game::Move& move, const Colors enemyColor) noexcept
    {
        // Since state.unsetPiece() will delete target piece from general occupancy,
        // state.checkCastlingRemoval() in state.movePiece() won't detect
        // if we take an enemy rook or not and therefore will not be able
        // to remove the enemy castling right correctly
        this->state.checkCastlingRemoval(move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        Pieces toRemove = this->state.getPiece(enemyColor, move.getToSquare());
        this->state.unsetPiece(enemyColor, toRemove, move.getToSquare());
        this->state.movePiece(this->state.sideToMove, move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        LOG_INFO("{} {} captured by {} {}", utils::toString(enemyColor), utils::toString(toRemove),
                 utils::toString(state.sideToMove), utils::toString(move.getFromPiece()));
    }

    void Game::makeCastling(const Move& move) noexcept
    {
        // Move the king
        this->state.movePiece(this->state.sideToMove, move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        // Move the rook
        switch (move.getCastlingType())
        {
        case Castlings::WHITE_KING_SIDE:
            this->state.movePiece(Colors::WHITE, Pieces::ROOK, 7, 5);
            LOG_INFO("Performed {}", utils::toString(move.getCastlingType()));
            break;
        case Castlings::WHITE_QUEEN_SIDE:
            this->state.movePiece(Colors::WHITE, Pieces::ROOK, 0, 3);
            LOG_INFO("Performed {}", utils::toString(move.getCastlingType()));
            break;
        case Castlings::BLACK_KING_SIDE:
            this->state.movePiece(Colors::BLACK, Pieces::ROOK, 63, 61);
            LOG_INFO("Performed {}", utils::toString(move.getCastlingType()));
            break;
        case Castlings::BLACK_QUEEN_SIDE:
            this->state.movePiece(Colors::BLACK, Pieces::ROOK, 56, 59);
            LOG_INFO("Performed {}", utils::toString(move.getCastlingType()));
            break;
        default:
            LOG_ERROR("Move castling type is invalid: {}", utils::toString(move.getCastlingType()));
            break;
        }
    }

    void Game::makeEnPassant(const Move& move, const Colors enemyColor) noexcept
    {
        // Move the pawn performing enPassant
        this->state.movePiece(this->state.sideToMove, move.getFromPiece(), move.getFromSquare(), move.getToSquare());

        // Determine enPassant captured piece's square
        int captureSquare;
        if (this->state.sideToMove == Colors::WHITE)
        {
            captureSquare = move.getToSquare() - 8;
        }
        else
        {
            captureSquare = move.getToSquare() + 8;
        }

        this->state.unsetPiece(enemyColor, Pieces::PAWN, captureSquare);

        LOG_INFO("Performed En Passant");
    }

    void Game::makePromotion(const game::Move& move) noexcept
    {
        if (move.getToSquare() >= 56) // Rank 8 = White promotion
        {
            this->state.unsetPiece(Colors::WHITE, Pieces::PAWN, move.getToSquare());
            this->state.setPiece(Colors::WHITE, move.getPromotionPiece(), move.getToSquare());

            LOG_INFO("Performed White Promotion");
        }
        else // Black promotion
        {
            this->state.unsetPiece(Colors::BLACK, Pieces::PAWN, move.getToSquare());
            this->state.setPiece(Colors::BLACK, move.getPromotionPiece(), move.getToSquare());

            LOG_INFO("Performed Black Promotion");
        }
    }

    void Game::update(const Move& move, const Colors enemyColor) noexcept
    {
        if (move.getMoveType() == MoveTypes::CAPTURE || move.getMoveType() == MoveTypes::EN_PASSANT ||
            move.getMoveType() == MoveTypes::PROMOTION || move.getFromPiece() == Pieces::PAWN)
        {
            this->state.halfMoveClock = 0;
        }
        else
        {
            this->state.halfMoveClock++;
        }

        // If black just played, increase full move clock
        if (this->state.sideToMove == Colors::BLACK)
        {
            this->state.fullMoveClock++;
        }

        // If pawn double push was played, set enPassant square
        if (move.getMoveType() == MoveTypes::DOUBLE_PUSH)
        {
            this->state.enPassantSquare = (move.getFromSquare() + move.getToSquare()) >> 1;
        }
        // If enPassant square was set and we didn't double push, set it back to -1
        else if (this->state.enPassantSquare != -1)
        {
            this->state.enPassantSquare = -1;
        }

        this->state.sideToMove = enemyColor;

        LOG_DEBUG("Board state updated: [Half move clock: {}] - [Full move clock: {}] - [Side to move: {}] - [White "
                  "king side castling right: {}] - [White queen side castling right: {}] - [Black king side castling "
                  "right: {}] - [Black queen side castling right: {}] - [En passant square: {}]",
                  this->state.halfMoveClock, this->state.fullMoveClock, utils::toString(this->state.sideToMove),
                  this->state.hasCastlingRight<WHITE_KING_SIDE>(), this->state.hasCastlingRight<WHITE_QUEEN_SIDE>(),
                  this->state.hasCastlingRight<BLACK_KING_SIDE>(), this->state.hasCastlingRight<BLACK_QUEEN_SIDE>(),
                  this->state.enPassantSquare);

        this->moveList.generateAllMoves(this->state);
    }

    void Game::playMove(const Move& move) noexcept
    {
        LOG_DEBUG("Playing move: [From square: {}] - [To square: {}] - [Move type: {}] - [From piece: {}]",
                  utils::squareIndexToString(move.getFromSquare()), utils::squareIndexToString(move.getToSquare()),
                  utils::toString(move.getMoveType()), utils::toString(move.getFromPiece()));

        Colors enemyColor = this->state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // If move is a capture, move the from piece and remove the target piece
        if (move.getMoveType() == MoveTypes::CAPTURE)
        {
            this->makeCapture(move, enemyColor);
        }
        // If move is a castling, move to according rook
        else if (move.getMoveType() == MoveTypes::CASTLE) [[unlikely]]
        {
            this->makeCastling(move);
        }
        // If move is an enPassant, remove the target piece
        else if (move.getMoveType() == MoveTypes::EN_PASSANT) [[unlikely]]
        {
            this->makeEnPassant(move, enemyColor);
        }
        // If no special move, just move the piece
        else
        {
            this->state.movePiece(this->state.sideToMove, move.getFromPiece(), move.getFromSquare(),
                                  move.getToSquare());
        }

        // If promotion piece is set, make promotion
        if (move.isPromotion()) [[unlikely]]
        {
            this->makePromotion(move);
        }

        this->update(move, enemyColor);
    }
} // namespace engine::game