/**
 * @file const.h
 * @author Thibault THOMAS
 *
 * @brief Defines various paths.
 */

#ifndef PATHS_H_
#define PATHS_H_

/*----- System -----*/
#include <filesystem>
#include <string>
#include <iostream>

/**
 * @namespace conf::paths
 */
namespace conf::paths
{
    #ifdef PROJECT_ROOT

    // Path to the directory from where the binary is executed
    inline constexpr std::filesystem::path root = std::filesystem::path(PROJECT_ROOT);

    // Paths to log files
    inline constexpr std::string engine_log_file = (root / "logs" / "engine.log").string();
    inline constexpr std::string app_log_file    = (root / "logs" / "app.log").string();

    #endif // PROJECT_ROOT
} // namespace conf::paths

#endif // PATHS_H_
