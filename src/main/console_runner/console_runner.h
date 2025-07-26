/**
 * @file      console_runner.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Handles the game loop in console mode.
 */

#ifndef CONSOLE_RUNNER_H
#define CONSOLE_RUNNER_H

#include <expected>

#include "engine/core/enums.h"
#include "engine/game/game.h"

namespace console_runner
{
    /**
     * @class ConsoleRunner
     * @brief Handles the game loop in console mode.
     */
    class ConsoleRunner
    {
      public:
        /**
         * @brief Constructor : instantiate the game
         */
        ConsoleRunner() noexcept;

        /**
         * @brief Constructor using FEN notation string
         *
         * @param [in] fenNotation : the FEN string
         */
        ConsoleRunner(std::string& fenNotation);

        /**
         * @brief Default destructor.
         */
        ~ConsoleRunner() noexcept = default;

        /**
         * @brief Main loop function to run the game.
         *
         * @return The winner team
         */
        void runGame() noexcept;

      private:
        /**
         * @brief Ask the user the move he wants to play.
         *
         * @return std::string : the user's input
         */
        std::string askInput() const noexcept;

        /**
         * @brief If previous player called a draw, ask it to the other player.
         *
         * @return std::string : response from other player (yes/no)
         */
        std::string askDraw() const noexcept;

        /**
         * @brief Asks user for promotion.
         * Restart until user enters a valid promotion.
         *
         * @return Piece : the requested piece type.
         */
        engine::core::Piece askPromotion() const noexcept;

        /**
         * @brief Parses a user input and use it to build a move.
         *
         * @param [in] input : the user input
         * @return std::expected : built move if input is valid or error message if not valid
         */
        std::expected<engine::game::Move, std::string> inputToMove(std::string input) noexcept;

        /**
         * @brief Handles a draw request from a user.
         *
         * @return bool : wether the draw is valid and accepted by both users
         */
        bool handleDrawRequest() noexcept;

        // Game engine instance
        engine::game::Game m_game;
    };

} // namespace console_runner

#endif // CONSOLE_RUNNER_H