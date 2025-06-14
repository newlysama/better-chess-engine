/*----- Project Headers -----*/
#include "logging/include/logging.h"
//#include "engine/include/Board.h"
#include "conf/const.h"

int main(int argc, char* argv[])
{
    // Initialize loggers
    logging::init_engine_logger();
    logging::init_application_logger();

    return 0;
}
