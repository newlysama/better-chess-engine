/**
 * @file      game.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game class console specific extensions implementations
 */

#if defined(PLAY_CONSOLE)

#include <iostream>
#include <print>

#include "engine/game/game.h"
#include "engine/printer/printer.h"
#include "logging/logging.h"

#if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
#include "utils/enums_to_string.h"
#include "utils/utils.h"
#endif

/**
 * @namespace engine::game
 */
namespace engine::game
{
    using namespace engine::core;
    using namespace engine::board;
    using namespace engine::printer;

    std::string Game::askInput() const noexcept
    {
        std::string input;

        if (this->state.halfMoveClock >= 100)
        {
            std::print("Select a move to play (format ex: a1a2 OR 'draw'): ");
        }
        else
        {
            std::print("Select a move to play (format ex: a1a2): ");
        }

        std::cin >> input;

        LOG_DEBUG("Got user console input: {}", input);

        return input;
    }

    std::string Game::askDraw() const noexcept
    {
        std::string input;
        std::print("{} player asked for a draw, accept ? (yes/no): ", utils::toString(this->state.sideToMove));

        while (true)
        {
            std::cin >> input;

            if (input == "yes" || input == "no")
            {
                LOG_DEBUG("Draw response from enemy player: {}", input);
                return input;
            }
            else
            {
                std::print("Invalid response: [{}], please enter yes/no : ", input);
            }
        }
    }

    Pieces Game::askPromotion() const noexcept
    {
        std::string promotion;
        Pieces piece;

        std::print("Select a promotion type (lowercase piece name): ");

        while (true)
        {
            std::cin >> promotion;
            piece = utils::fromString(promotion);

            if (piece == Pieces::UNKNOWN_PIECE || piece == Pieces::KING || piece == Pieces::PAWN) [[unlikely]]
            {
                LOG_DEBUG("User entered non existing piece: {}", promotion);
                std::print("Please enter a valid piece (queen, rook, bishop or knight): ");
            }
            else
            {
                break;
            }
        }

        return piece;
    }

    std::expected<Move, std::string> Game::inputToMove(std::string input) noexcept
    {
        if (input.size() != 4)
        {
            return std::unexpected("Invalid input size: " + std::to_string(input.size()));
        }
        else if (input == "draw" && this->state.halfMoveClock < 100) [[unlikely]]
        {
            return std::unexpected("Cannot call a while half move clock is < 100");
        }

        std::string fromStr = input.substr(0, 2);
        std::string toStr = input.substr(2, 2);

        int fromSquare;
        int toSquare;

        // Check if from square input is valid
        if (SQUARE_INDEX.find(fromStr) != SQUARE_INDEX.end())
        {
            fromSquare = SQUARE_INDEX.at(fromStr);
        }
        else
        {
            return std::unexpected("Entered an initial square does not exist: " + fromStr);
        }

        // Check if to square input is valid
        if (SQUARE_INDEX.find(toStr) != SQUARE_INDEX.end())
        {
            toSquare = SQUARE_INDEX.at(toStr);
        }
        else
        {
            return std::unexpected("Entered a destination square does not exist: " + toStr);
        }

        // If the requested move exists, assign it
        Move move{};
        for (std::size_t i = 0; i < this->moveList.size(); i++)
        {
            Move current = this->moveList[i];

            // Check if the requested move is in the legal moves list
            if (current.fromSquare == fromSquare && current.toSquare == toSquare)
            {
                move = current;
                break;
            }
        }

        // If the requested move doesn't exists, throw
        if (!move.isSet())
        {
            return std::unexpected("Entered move is not legal: " + fromStr + " --> " + toStr);
        }

        return move;
    }

    void Game::playConsole() noexcept
    {
        this->moveList.generateAllMoves(this->state);
        engine::printer::printState(this->state);

        while (true)
        {
            std::string userInput = this->askInput();
            bool draw = false;

            if (userInput == "draw")
            {
                LOG_DEBUG("Player {} is calling a draw", utils::toString(this->state.sideToMove));
                if (this->state.halfMoveClock >= 100)
                {
                    std::string drawResponse = "";
                    while (true)
                    {
                        drawResponse = this->askDraw();

                        if (drawResponse == "yes")
                        {
                            LOG_DEBUG("Both players acctepted the draw. Ending the this->");
                            std::println("Game ended on a draw");

                            return;
                        }
                        else
                        {
                            LOG_DEBUG("Enemy player refused the draw request, continuing");
                            std::println("Enemy player refused the draw request, continuing");

                            draw = true;
                            break;
                        }
                    }
                }
            }

            if (!draw) [[likely]]
            {
                auto move = this->inputToMove(userInput).or_else([](std::string error) {
                    LOG_DEBUG(error);
                    std::println("{}", error);

                    return std::expected<Move, std::string>{};
                });

                //
                if (!move)
                    continue;

                if (move.value().promotion == true) [[unlikely]]
                {
                    move.value().promotionPiece = this->askPromotion();
                }

                this->playMove(move.value());
                printState(this->state);
            }
        }
    }
} // namespace engine::game

#endif // PLAY_CONSOLE