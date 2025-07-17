/**
 * @file      main.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Entry point of the program
 */

#include "engine/game/game.h"
#include "engine/printer/printer.h"
#include "logging/logging.h"

#if defined(GENERATE_MAGICS)
#include "engine/board/magics_generator.h"
#endif

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Initialize loggers
    logging::init_logger();

#if defined(GENERATE_MAGICS)
    engine::board::initMagics();
#else

    engine::game::Game game;
    game.moveList.generateAllMoves(game.state, engine::core::Colors::WHITE);
    engine::printer::printState(game.state);

    while (true)
    {
        std::string userInput = game.askInput();

        engine::game::Move move;
        move = game.inputToMove(userInput);

        if (move.isSet() == false)
        {
            std::cout << "Please enter a valid input";

            continue;
        }

        game.playMove(move);
        engine::printer::printState(game.state);
    }

#endif // GENERATE_MAGICS

    return 0;
}
