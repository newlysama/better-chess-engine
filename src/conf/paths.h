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
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace conf::paths
     */
    namespace conf::paths
    {
        #ifdef PROJECT_ROOT

        // Path to the directory from where the binary is executed
        inline const std::filesystem::path root_path = std::filesystem::path(PROJECT_ROOT);

        // Path to log files
        inline const std::string log_file_path = (root_path / "logs" / "engine.log").string();

        #endif // PROJECT_ROOT
    } // namespace conf::paths
} // namespace engine


#endif // PATHS_H_
