/**
 * @file main.cpp
 * @author Thibault THOMAS
 *
 * @brief Entry point of the program
 */

/*----- Project Headers -----*/
#include "logging/include/logging.h"
#include "engine/include/board.h"

int main(int argc, char* argv[])
{
    // Initialize loggers
    logging::init_engine_logger();
    logging::init_application_logger();

    return 0;
}
