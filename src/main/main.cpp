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

#if defined(PLAY_CONSOLE)
void playConsole()
{
    engine::game::Game game;
    game.moveList.generateAllMoves(game.state);
    engine::printer::printState(game.state);

    while (true)
    {
        std::string userInput = game.askInput();
        bool draw = false;

        if (userInput.compare("draw") == 0)
        {
            LOG_DEBUG("Player {} is calling a draw", utils::toString(game.state.sideToMove));
            if (game.state.halfMoveClock >= 100)
            {
                std::string drawResponse = "";
                while (true)
                {
                    drawResponse = game.askDraw();

                    if (drawResponse != "yes" && drawResponse != "no")
                    {
                        std::cout << "Please enter a valid response to the draw\n";
                        continue;
                    }
                    else
                    {
                        if (drawResponse == "yes")
                        {
                            LOG_DEBUG("Bot players acctepted the draw. Ending the game.");
                            std::cout << "Game ended on a draw.\n";
                            return;
                        }
                        else
                        {
                            LOG_DEBUG("Enemy player refused the draw request, continuing.");
                            std::cout << "Enemy player refused the draw request, continuing.\n";
                            draw = true;
                            break;
                        }
                    }
                }
            }
        }

        if (!draw)
        {
            engine::game::Move move;
            move = game.inputToMove(userInput);

            if (move.isSet() == false) [[unlikely]]
            {
                std::cout << "Please enter a valid input\n";

                continue;
            }

            game.playMove(move);
            engine::printer::printState(game.state);
        }
    }
}
#endif // PLAY_CONSOLE

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Initialize loggers
    logging::init_logger();

#if defined(GENERATE_MAGICS)
    engine::board::initMagics();
#elif defined(PLAY_CONSOLE)
    playConsole();
#else
    return;
#endif

    return 0;
}
