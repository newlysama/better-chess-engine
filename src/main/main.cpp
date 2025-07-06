/**
 * @file      main.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Entry point of the program
 */

#include "engine/board/board.h"
#include "engine/printer/printer.h"
#include "logging/logging.h"

#if defined(BUILD_GENERATE_MAGICS)
#    include "engine/board/magics_generator.h"
#endif

int main(int argc, char* argv[])
{
    // Initialize loggers
    logging::init_logger();

#if defined(BUILD_GENERATE_MAGICS)
    engine::board::magics_generator::initMagics();
#else

    engine::board::Board board;

    engine::printer::printBoard(board);

#endif // BUILD_GENERATE_MAGICS

    return 0;
}
