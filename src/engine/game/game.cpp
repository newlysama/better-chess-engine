/**
 * @file      game.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game class implementation.
 */

#include "engine/game/game.h"

#include <iostream>
#include <string>

#include "engine/core/const.h"
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

    std::string Game::askInput() const noexcept
    {
        std::string input;

        std::cout << "Select a move to play (format ex: a1a2): ";
        std::cin >> input;

        LOG_DEBUG("Got user console input: {}", input);

        return input;
    }

    Pieces Game::askPromotion() const noexcept
    {
        std::string promotion;
        Pieces piece;

        std::cout << "Select a promotion type (lowercase piece name): ";

        while (true)
        {
            std::cin >> promotion;
            piece = utils::fromString(promotion);

            if (piece == Pieces::UNKNOWN_PIECE || piece == Pieces::KING || piece == Pieces::PAWN)
            {
                LOG_DEBUG("User entered non existing piece: {}", promotion);
                std::cout << "Please enter a valid piece (queen, rook, bishop or knight): ";
            }
            else
            {
                break;
            }
        }

        return piece;
    }

    Move Game::inputToMove(std::string input) noexcept
    {
        if (input.size() != 4)
        {
            LOG_DEBUG("User's input had invalid size: {}", input.size());
            return Move{};
        }

        std::string fromStr = input.substr(0, 2);
        std::string toStr = input.substr(2, 2);

        int fromSquare;
        int toSquare;

        // Check if from square input is valid
        if (SQUARE_INDEX.find(fromStr) != SQUARE_INDEX.end())
        {
            fromSquare = SQUARE_INDEX.at(fromStr);

            // Check if from square input contains a piece from sideToMove team
            if (this->state.coloredOccupancies[this->state.sideToMove].isSet(fromSquare) == false)
            {
                LOG_DEBUG("User entered a fromSquare that does not contains any ally piece");
                return Move{};
            }
        }
        else
        {
            LOG_DEBUG("User entered a fromSquare that does not exist");
            return Move{};
        }

        // Check if to square input is valid
        if (SQUARE_INDEX.find(toStr) != SQUARE_INDEX.end())
        {
            toSquare = SQUARE_INDEX.at(toStr);

            // Check if to square input contains a piece from sideToMove team
            if (this->state.coloredOccupancies[this->state.sideToMove].isSet(toSquare))
            {
                LOG_DEBUG("User entered a toSquare that contains an ally piece");
                return Move{};
            }
        }
        else
        {
            LOG_DEBUG("User entered a toSquare that does not exist");
            return Move{};
        }

        // If the requested move exists, assign it
        Move move{};
        for (std::size_t i = 0; i < this->moveList.size(); i++)
        {
            Move current = this->moveList[i];

            // Check if the requested move is in the legal moves list
            if (current.squareFrom == fromSquare && current.squareTo == toSquare)
            {
                move = current;
                break;
            }
        }

        // If the requested move doesn't exists, throw
        if (!move.isSet())
        {
            LOG_DEBUG("User entered an move that is not legal");
        }

        return move;
    }

    void Game::makeCapture(const game::Move& move, const Colors enemyColor) noexcept
    {
        Pieces toRemove = this->state.getPiece(enemyColor, move.squareTo);
        this->state.unsetPiece(enemyColor, toRemove, move.squareTo);
        this->state.movePiece(this->state.sideToMove, move.fromPiece, move.squareFrom, move.squareTo);
    }

    void Game::makeCastling(const Move& move) noexcept
    {
        // Move the king
        this->state.movePiece(this->state.sideToMove, move.fromPiece, move.squareFrom, move.squareTo);

        // Move the rook
        switch (move.castling)
        {
        case Castlings::WHITE_KING_SIDE:
            this->state.movePiece(Colors::WHITE, Pieces::ROOK, 7, 5);
            break;
        case Castlings::WHITE_QUEEN_SIDE:
            this->state.movePiece(Colors::WHITE, Pieces::ROOK, 0, 3);
            break;
        case Castlings::BLACK_KING_SIDE:
            this->state.movePiece(Colors::BLACK, Pieces::ROOK, 63, 61);
            break;
        case Castlings::BLACK_QUEEN_SIDE:
            this->state.movePiece(Colors::BLACK, Pieces::ROOK, 56, 59);
            break;
        default:
            LOG_ERROR("Move castling type is invalid: {}", utils::toString(move.castling));
            break;
        }
    }

    void Game::makeEnPassant(const Move& move, const Colors enemyColor) noexcept
    {
        // Move the pawn performing enPassant
        this->state.movePiece(this->state.sideToMove, move.fromPiece, move.squareFrom, move.squareTo);

        // Determine enPassant captured piece's square
        int captureSquare;
        if (this->state.sideToMove == Colors::WHITE)
        {
            captureSquare = move.squareTo - 8;
        }
        else
        {
            captureSquare = move.squareTo + 8;
        }

        this->state.unsetPiece(enemyColor, Pieces::PAWN, captureSquare);
    }

    void Game::makePromotion(const game::Move& move) noexcept
    {
        Pieces promotionPiece = this->askPromotion();

        if (move.squareTo >= 56) // Rank 8 = White promotion
        {
            this->state.unsetPiece(Colors::WHITE, Pieces::PAWN, move.squareTo);
            this->state.setPiece(Colors::WHITE, promotionPiece, move.squareTo);
        }
        else // Black promotion
        {
            this->state.unsetPiece(Colors::WHITE, Pieces::PAWN, move.squareTo);
            this->state.setPiece(Colors::BLACK, promotionPiece, move.squareTo);
        }
    }

    void Game::update(const Move& move, const Colors enemyColor) noexcept
    {
        this->state.halfMoveClock++;

        // If white just played, increase full move clock
        if (this->state.sideToMove == Colors::WHITE)
        {
            this->state.fullMoveClock++;
        }

        // If pawn double push was played, set enPassant square
        if (move.moveType == MoveTypes::DOUBLE_PUSH)
        {
            this->state.enPassantSquare = (move.squareFrom + move.squareTo) >> 1;
        }
        // If enPassant square was set and we didn't double push, set it back to -1
        else if (this->state.enPassantSquare != -1)
        {
            this->state.enPassantSquare = -1;
        }

        this->state.sideToMove = enemyColor;

        LOG_DEBUG("Board state updated: [Half move clock: {}] - [Full move clock: {}] - [Side to move: {}] - [White "
                  "king side castling right: {}] - [White queen side castling right: {}] - [Black king side castling "
                  "right: {}] - [Black queen side castling right: {}] - En passant square: {}",
                  this->state.halfMoveClock, this->state.fullMoveClock, utils::toString(this->state.sideToMove),
                  this->state.hasCastlingRight<WHITE_KING_SIDE>(), this->state.hasCastlingRight<WHITE_QUEEN_SIDE>(),
                  this->state.hasCastlingRight<BLACK_KING_SIDE>(), this->state.hasCastlingRight<BLACK_QUEEN_SIDE>(),
                  this->state.enPassantSquare);

        this->moveList.generateAllMoves(this->state, enemyColor);
    }

    void Game::playMove(const Move& move) noexcept
    {
        LOG_DEBUG("Playing move: [From square: {}] - [To square: {}] - [Move type: {}] - [From piece: {}]",
                  utils::squareIndexToString(move.squareFrom), utils::squareIndexToString(move.squareTo),
                  utils::toString(move.moveType), utils::toString(move.fromPiece));

        Colors enemyColor = this->state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // If move is a capture, move the from piece and remove the target piece
        if (move.moveType == MoveTypes::CAPTURE)
        {
            this->makeCapture(move, enemyColor);
        }
        // If move is a castling, move to according rook
        else if (move.moveType == MoveTypes::CASTLE)
        {
            this->makeCastling(move);
        }
        // If move is an enPassant, remove the target piece
        else if (move.moveType == MoveTypes::ENPASSANT) [[unlikely]]
        {
            this->makeEnPassant(move, enemyColor);
        }
        // If no special move, just move the piece
        else
        {
            this->state.movePiece(this->state.sideToMove, move.fromPiece, move.squareFrom, move.squareTo);
        }

        // If promotion flag is set, ask and make promotion
        if (move.promotion == true) [[unlikely]]
        {
            this->makePromotion(move);
        }

        this->update(move, enemyColor);
    }
} // namespace engine::game