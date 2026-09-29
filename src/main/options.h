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

#include <charconv>
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
        bool benchmark{false};
        int benchmark_depth{0};
    };

    /**
     * @brief Parse the --bench depth argument.
     *
     * @param [in] depthStr : the depth as entered by the user
     * @return int : the depth, or an error message if not an integer in [1, 10]
     */
    inline std::expected<int, std::string> parseBenchDepth(const std::string& depthStr)
    {
        int depth = 0;
        auto [end, ec] = std::from_chars(depthStr.data(), depthStr.data() + depthStr.size(), depth);

        if (ec != std::errc{} || end != depthStr.data() + depthStr.size() || depth < 1 || depth > 10)
        {
            return std::unexpected("Usage : ./chess --bench <depth> (1 <= depth <= 10)");
        }

        return depth;
    }

    /**
     * @brief Validate argv and return Options or an error message.
     *
     * - generate_magics builds take no option.
     * - Every other build accepts --bench <depth>.
     * - Console builds also accept --fen <fen>, and start a game when no option is given.
     */
    inline std::expected<Options, std::string> parse(int argc, char* argv[])
    {
        std::vector<std::string> args{argv + 1, argv + argc};
        Options opt;

        #if defined(GENERATE_MAGICS)
            if (!args.empty())
            {
                return std::unexpected("This build takes no command-line options");
            }
            return opt;
        #else
            // --bench <depth>
            if (args.size() == 2 && args[0] == "--bench")
            {
                auto depth = parseBenchDepth(args[1]);
                if (!depth)
                {
                    return std::unexpected(depth.error());
                }

                opt.benchmark = true;
                opt.benchmark_depth = depth.value();
                return opt;
            }

            #if defined(PLAY_CONSOLE)
                if (args.empty())
                    return opt;

                // --fen <FEN>
                if (args.size() == 2 && args[0] == "--fen")
                {
                    opt.fen = std::string(args[1]);
                    return opt;
                }

                return std::unexpected("Usage : ./chess [--fen <fen> | --bench <depth>]");
            #else
                return std::unexpected("Usage : ./chess --bench <depth> (build with -Dconsole=true to play)");
            #endif
        #endif
    }

} // namespace options

#endif // OPTIONS_H_
