/**
 * @file      main.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Entry point of the program
 */

/*----- Project Headers -----*/
#include "board/board.h"
#include "logging/logging.h"
#include "printer/printer.h"

int main(int argc, char *argv[])
{
    // Initialize loggers
    engine::logging::init_logger();

    engine::board::Board board;

    engine::printer::printBoard(board);

    return 0;
}
