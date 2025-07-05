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

int main(int argc, char* argv[])
{
    // Initialize loggers
    logging::init_logger();

    engine::board::Board board;

    engine::printer::printBoard(board);

    return 0;
}
