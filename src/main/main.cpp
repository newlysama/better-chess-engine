/**
 * @file      main.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Entry point of the program
 */

#include "engine/game/game.h"
#include "logging/logging.h"

#if defined(GENERATE_MAGICS)
#include "engine/board/magics_generator.h"
#endif

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Initialize logger
    logging::init_logger();

    // Init game
    engine::game::Game game;

#if defined(GENERATE_MAGICS)
    engine::board::initMagics();
#elif defined(PLAY_CONSOLE)
    game.playConsole();
#else
    LOG_INFO("DEFAULT BUILD");
    return 0;
#endif

    return 0;
}
