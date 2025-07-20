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

/**
 * @namespace conf::paths
 */
namespace conf::paths
{
    // Path to the directory from where the binary is executed
    inline const std::filesystem::path rootPath = std::filesystem::path(PROJECT_ROOT);

    // Path to log files
    inline const std::string logFilePath = (rootPath / "logs" / "engine.log").string();

    inline const std::string consoleUIHeaderPath = (rootPath / "src" / "ui" / "console" / "header.txt").string();
} // namespace conf::paths

#endif // PATHS_H_
