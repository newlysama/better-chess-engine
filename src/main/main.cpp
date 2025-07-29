/**
 * @file      main.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Entry point of the program
 */

#include "logging/logging.h"
#include "main/options.h"

#if defined(PLAY_CONSOLE)
#include "main/console_runner/console_runner.h"

#elif defined(PLAY_SERVER)
// TODO: include your server header

#elif defined(GENERATE_MAGICS)

#include "engine/magics_generator/magics_generator.h"

#endif

/**
 * @brief Run the program according to the defined options
 *
 * @param [in] options : processed user entered options
 */

static int run(const options::Options& opt)
{
#if defined(PLAY_CONSOLE)

    auto fen = opt.perftFen;

    console_runner::ConsoleRunner runner =
        fen.has_value() ? console_runner::ConsoleRunner{fen.value()} : console_runner::ConsoleRunner{};

    runner.runGame();

    return 0;

/*───────────────────────────────────────────────────────────────────────────*/
#elif defined(PLAY_SERVER)

    LOG_INFO("Starting server (not implemented yet)");
    // TODO: server loop here
    return 0;

/*───────────────────────────────────────────────────────────────────────────*/
#elif defined(GENERATE_MAGICS)

    engine::magics_generator::initMagics();
    return 0;
#endif
}

int main(int argc, char* argv[])
{
    // Logger
    logging::init_logger();

    // Argument parsing
    auto parsed = options::parse(argc, argv);

    if (!parsed)
    {
        LOG_ERROR("Argument error: {}", parsed.error());
        return EXIT_FAILURE;
    }

    // Dispatch according to mode
    const auto& options = parsed.value();
    run(options);
}
