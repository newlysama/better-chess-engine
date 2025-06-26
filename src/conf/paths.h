/**
 * @file      const.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Defines various paths.
 */

#ifndef PATHS_H_
#define PATHS_H_

#include <filesystem>
#include <string>

/**
 * @namespace conf::paths
 */
namespace conf::paths
{
    // Path to the directory from where the binary is executed
    inline const std::filesystem::path root_path = std::filesystem::path(PROJECT_ROOT);

    // Path to log files
    inline const std::string log_file_path = (root_path / "logs" / "engine.log").string();
} // namespace conf::paths

#endif // PATHS_H_
