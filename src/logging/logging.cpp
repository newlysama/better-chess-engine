/**
 * @file      logging.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Logging system implementation.
 */

#include "logging/logging.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

#include "conf/paths.h"

namespace logging
{
#if defined(BUILD_RELEASE) || defined(BUILD_TEST) || defined(BUILD_BENCHMARK)
    bool logInFile = true;
#else
    bool logInFile = false;
#endif

    // Loggers
    static std::shared_ptr<spdlog::logger> logger;

    void init_logger()
    {
        if (logger)
            return;

        // Create empty sink_ptr
        spdlog::sink_ptr sink;

        // Fill sink depending on the build mode
        if (logInFile)
        {
            sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(conf::paths::logFilePath, true);
        }

        else
        {
            sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }

        // Create and configure logger
        logger = std::make_shared<spdlog::logger>("engine", std::initializer_list<spdlog::sink_ptr>{sink});
        logger->set_level(logInFile ? spdlog::level::info : spdlog::level::trace);
        logger->flush_on(spdlog::level::info);
        spdlog::register_logger(logger);
    }

    std::shared_ptr<spdlog::logger> get_logger()
    {
        init_logger();
        return logger;
    }
} // namespace logging
