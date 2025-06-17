/**
 * @file logging.cpp
 * @author Thibault THOMAS
 *
 * @brief Logging system implementation.
 */

/*----- System -----*/
#include <vector>

/*----- External Libs -----*/
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

/*----- Project Headers -----*/
#include "logging/include/logging.h"
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

        // Loggers
        static std::shared_ptr<spdlog::logger> logger;

    /*-------------------------------------------------------------------*
    *                        INIT ENGINE LOGGER                         *
    *-------------------------------------------------------------------*/

        void init_logger()
        {
            if (logger) return;

            std::vector<spdlog::sink_ptr> sinks;

            #if defined(BUILD_RELEASE)
                auto path = engine::conf::paths::log_file_path;

                sinks.push_back(
                    std::make_shared<spdlog::sinks::basic_file_sink_mt>(path, true)
                );

            #else
                sinks.push_back(
                    std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
                );

            #endif

            logger = std::make_shared<spdlog::logger>("engine", sinks.begin(), sinks.end());

            #if defined(BUILD_DEBUG) || defined(BUILD_PROFILE) || defined(BUILD_TEST) || defined(BUILD_BENCHMARK)
                logger->set_level(spdlog::level::debug);

            #else
                logger->set_level(spdlog::level::warn);
            #endif

            spdlog::register_logger(logger);
        }

        std::shared_ptr<spdlog::logger> get_logger()
        {
            init_logger();
            return logger;
        }
    } // namespace logging
} // namespace engine

