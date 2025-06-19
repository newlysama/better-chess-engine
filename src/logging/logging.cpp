/**
 * @file logging.cpp
 * @author Thibault THOMAS
 *
 * @brief Logging system implementation.
 */

#include "logging/logging.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

#include "conf/paths.h"

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace logging
     */
    namespace logging
    {
        bool isRelease = false;

#if defined(BUILD_RELEASE)
        isRelease = true;
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
            if (isRelease)
            {
                sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(engine::conf::paths::log_file_path, true);
            }

            else
            {
                sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            }

            // Create and configure logger
            logger = std::make_shared<spdlog::logger>("engine", std::initializer_list<spdlog::sink_ptr>{sink});

            logger->set_level(isRelease ? spdlog::level::trace : spdlog::level::info);

            spdlog::register_logger(logger);
        }

        std::shared_ptr<spdlog::logger> get_logger()
        {
            init_logger();
            return logger;
        }
    } // namespace logging
} // namespace engine
