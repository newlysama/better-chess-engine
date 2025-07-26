/**
 * @file      console_runner.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Implementation of the game loop in console mode.
 */

#include "main/console_runner/console_runner.h"

#include <iostream>
#include <print>

#include "engine/core/const.h"
#include "logging/logging.h"
#include "ui/console/console_ui.h"
#include "utils/enums_to_string.h"

#if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
#include "utils/utils.h"
#endif

namespace console_runner
{
    using namespace ui::console;
    using namespace engine::core;
    using namespace engine::game;

    ConsoleRunner::ConsoleRunner() noexcept
        : game(Game{})
    {
    }

    ConsoleRunner::ConsoleRunner(std::string& fenNotation)
        : game(Game{fenNotation})
    {
    }

    std::string ConsoleRunner::askInput() const noexcept
    {
        std::string input;

        if (this->game.state.halfMoveClock >= 100)
        {
            std::print("Select a move to play (format ex: a1a2 OR 'draw'): ");
        }
        else
        {
            std::print("Select a move to play (format ex: a1a2): ");
        }

        std::cin >> input;

        LOG_INFO("Got user console input: {}", input);

        return input;
    }

    std::string ConsoleRunner::askDraw() const noexcept
    {
        std::string input;
        std::print("{} player asked for a draw, accept ? (yes/no): ", utils::toString(this->game.state.sideToMove));

        while (true)
        {
            std::cin >> input;

            if (input == "yes" || input == "no")
            {
                LOG_INFO("Draw response from enemy player: {}", input);
                return input;
            }
            else
            {
                std::print("Invalid response: [{}], please enter yes/no : ", input);
            }
        }
    }

    Pieces ConsoleRunner::askPromotion() const noexcept
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
                LOG_INFO("User entered non existing piece: {}", promotion);
                std::print("Please enter a valid piece (queen, rook, bishop or knight): ");
            }
            else
            {
                break;
            }
        }

        return piece;
    }

    std::expected<Move, std::string> ConsoleRunner::inputToMove(std::string input) noexcept
    {
        if (input.size() != 4)
        {
            return std::unexpected(std::format("Invalid input size: {}", input.size()));
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
            return std::unexpected(std::format("Entered an initial square does not exist: {}", fromStr));
        }

        // Check if destination square input is valid
        if (SQUARE_INDEX.find(toStr) != SQUARE_INDEX.end())
        {
            toSquare = SQUARE_INDEX.at(toStr);
        }
        else
        {
            return std::unexpected(std::format("Entered a destination square does not exist: {}", toStr));
        }

        // If the requested move exists, return it.
        for (std::size_t i = 0; i < this->game.moveList.size(); i++)
        {
            Move current = this->game.moveList[i];

            if (current.getFromSquare() == fromSquare && current.getToSquare() == toSquare)
            {
                return current;
            }
        }

        return std::unexpected(std::format("Entered move is not legal: {} --> {}", fromStr, toStr));
    }

    bool ConsoleRunner::handleDrawRequest() noexcept
    {
        LOG_INFO("Player {} is calling a draw", utils::toString(this->game.state.sideToMove));

        if (this->game.state.halfMoveClock >= 100)
        {
            while (true)
            {
                // Ask the enemy player his response to the draw request
                std::string drawResponse = this->askDraw();

                if (drawResponse == "yes")
                {
                    LOG_INFO("Both players acctepted the draw. Ending the this->");
                    std::println("Game ended on a draw");

                    return true;
                }
                else
                {
                    LOG_INFO("Enemy player refused the draw request, continuing");
                    std::println("Enemy player refused the draw request, continuing");

                    return false;
                }
            }
        }
        else
        {
            LOG_INFO("Draw request is not valid, current Half Move Clock is {}", this->game.state.halfMoveClock);
            std::println("Cannot draw until Half Move Clock reaches 100.");

            return false;
        }
    }

    void ConsoleRunner::runGame() noexcept
    {
        this->game.moveList.generateAllMoves(this->game.state);

        // Load UI header
        ConsoleUI::loadHeader();

        // Print initial state
        ConsoleUI::RenderState(this->game.state);

        while (true)
        {
            std::string userInput = this->askInput();

            if (userInput == "draw")
            {
                bool isDraw = this->handleDrawRequest();

                if (isDraw)
                {
                    return;
                }
                else
                {
                    continue;
                }
            }

            auto move = this->inputToMove(userInput);

            if (!move)
            {
                LOG_DEBUG(move.error());
                std::println("{}", move.error());
                continue;
            }

            if (move.value().isPromotion() == true) [[unlikely]]
            {
                move.value().setPromotionPiece(this->askPromotion());
            }

            this->game.playMove(move.value());

            // Print new state
            ConsoleUI::RenderState(this->game.state);

            if (this->game.state.isCheckMate)
            {
                Colors winner = Colors::WHITE ? Colors::BLACK : Colors::WHITE;
                LOG_INFO("{} team won the game.", utils::toString(winner));
                std::println("{} team won the game !", utils::toString(winner));

                return;
            }
        }
    }
} // namespace console_runner