/**
 * @file      logging.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Logging system management.
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <memory>
#include <spdlog/spdlog.h>

/**
 * @namespace logging
 * @brief Declares loggers initialization and access.
 */
namespace logging
{

    /**
     * @brief Initialize logger.
     *
     * @details
     * Initialize its sinks, level, output.
     * If we build in release mode, log in log files defined in src/conf/const.h.
     * If we are in any other mode, log in stdout.
     */
    void init_logger();

    /**
     * @brief Allows logger access
     */
    std::shared_ptr<spdlog::logger> get_logger();

} // namespace logging

#define LOG_DEBUG(...)    SPDLOG_LOGGER_DEBUG(logging::get_logger(), __VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_LOGGER_INFO(logging::get_logger(), __VA_ARGS__)
#define LOG_WARN(...)     SPDLOG_LOGGER_WARN(logging::get_logger(), __VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_LOGGER_ERROR(logging::get_logger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(logging::get_logger(), __VA_ARGS__)

#endif // LOGGING_H_
