/**
 * @file main.cpp
 * @author Thibault THOMAS
 *
 * @brief Entry point of the program
 */

/*----- Project Headers -----*/
#include "logging/include/logging.h"
#include "board/include/board.h"

int main(int argc, char* argv[])
{
    // Initialize loggers
    engine::logging::init_logger();

    LOG_DEBUG("Engine debug");
    LOG_ERROR("Engine error");

    return 0;
}
