/**
 * @file logging.h
 * @author Thibault THOMAS
 *
 * @brief Logging system management.
 */

#ifndef LOGGING_H_
#define LOGGING_H_

/*----- System -----*/
#include <memory>

/*----- External Libs -----*/
#include <spdlog/spdlog.h>

/**
 * @namespace logging
 * @brief Declares loggers initialization and access.
 */
namespace logging
{

    /**
     * @brief Initialize engine's logger.
     *
     * @details
     * Initialize its sinks, level, output.
     * If we build in release mode, log in log files defined in src/conf/const.h.
     * If we are in any other mode, log in stdout.
     */
    void init_engine_logger();

    /**
     * @brief Initialize app's logger
     *
     * @details
     * Initialize its sinks, level, output.
     * If we build in release mode, log in log files defined in src/conf/const.h.
     * If we are in any other mode, log in stdout.
     */
    void init_application_logger();

    /**
     * @brief Allows engine's logger access
     */
    std::shared_ptr<spdlog::logger> engine_logger();

    /**
     * @brief Allows app's logger access
     */
    std::shared_ptr<spdlog::logger> application_logger();

} // namespace logging


#define LOG_ENGINE_DEBUG(...)    SPDLOG_LOGGER_DEBUG    (logging::engine_logger(), __VA_ARGS__)
#define LOG_ENGINE_INFO(...)     SPDLOG_LOGGER_INFO     (logging::engine_logger(), __VA_ARGS__)
#define LOG_ENGINE_WARN(...)     SPDLOG_LOGGER_WARN     (logging::engine_logger(), __VA_ARGS__)
#define LOG_ENGINE_ERROR(...)    SPDLOG_LOGGER_ERROR    (logging::engine_logger(), __VA_ARGS__)
#define LOG_ENGINE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL (logging::engine_logger(), __VA_ARGS__)

#define LOG_APP_DEBUG(...)       SPDLOG_LOGGER_DEBUG    (logging::application_logger(), __VA_ARGS__)
#define LOG_APP_INFO(...)        SPDLOG_LOGGER_INFO     (logging::application_logger(), __VA_ARGS__)
#define LOG_APP_WARN(...)        SPDLOG_LOGGER_WARN     (logging::application_logger(), __VA_ARGS__)
#define LOG_APP_ERROR(...)       SPDLOG_LOGGER_ERROR    (logging::application_logger(), __VA_ARGS__)
#define LOG_APP_CRITICAL(...)    SPDLOG_LOGGER_CRITICAL (logging::application_logger(), __VA_ARGS__)

#endif // LOGGING_H_
