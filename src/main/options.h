/**
 * @file      options.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Options handling logic (single-mode build).
 */
#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// clang-format off
namespace options
{
    struct Options
    {
        std::optional<std::string> fen{};
        bool benchmark;
    };

    /**
     * @brief Validate argv and return Options or an error message.
     */
    inline std::expected<Options, std::string> parse(int argc, char* argv[])
    {
        std::vector<std::string_view> args{argv + 1, argv + argc};
        Options opt;

        #if defined(PLAY_CONSOLE)
            if (args.empty())
                return opt;

            // --bench
            if (args.size() == 1 && args[0] == "--bench")
            {
                opt.benchmark = true;
                return opt;
            }
            else
            {
                opt.benchmark = false;
            }

            // --fen <FEN>
            if (args.size() == 2 && args[0] == "--fen")
            {
                opt.fen = std::string(args[1]);
                return opt;
            }

            return std::unexpected("Usage : ./chess [--fen <fen> | --bench]");
        #else
            if (!args.empty())
            {
                return std::unexpected("This build takes no command-line options");
            }
            return opt;
        #endif
    }

} // namespace options

#endif // OPTIONS_H_
