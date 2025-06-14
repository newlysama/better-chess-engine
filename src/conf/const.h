#ifndef CONST_H_
#define CONST_H_

/*----- System -----*/
#include <filesystem>

// Get project's root directory path
inline const std::filesystem::path root =
    std::filesystem::current_path();

// Log files
inline const std::filesystem::path engine_log_file = root / "logs" / "engine.log";
inline const std::filesystem::path app_log_file    = root / "logs" / "app.log";

#endif // CONST_H_
