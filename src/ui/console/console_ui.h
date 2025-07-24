/**
 * @file      console_ui.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Console rendering of the game.
 */

#include <array>
#include <cstring>
#include <string_view>

#include "engine/board/state.h"
#include "engine/core/enums.h"

#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

namespace ui::console
{
    /**
     * @class ConsoleUI
     */
    class ConsoleUI
    {
      public:
        /**
         * @brief Load the game's header from text file.
         */
        static void loadHeader() noexcept;

        /**
         * @brief Render the board and the current game state infos.
         *
         * @param [in] state : current state
         */
        static void RenderState(engine::board::State& state) noexcept;

      private:
        /**
         * @brief Get a colored symbol for a given piece from a given team.
         *
         * @param [in] color : team's color
         * @param [in] piece : piece to get
         * @return std::string : the colored piece string
         */
        static std::string getColoredPiece(engine::core::Colors color, engine::core::Pieces piece) noexcept;

        /**
         * @brief Look through allPieces bitboard to get the piece on a specific square.
         * If no piece found, return empty string for an empty square.
         *
         * @param [in] state  : current state
         * @param [in] square : square to look on
         * @return std::string : the colored piece symbol or empty string
         */
        static std::string getSquare(engine::board::State& state, int square) noexcept;

        /**
         * @brief Prints the header and blank lines after it.
         */
        static void printHeader() noexcept;

        /**
         * @brief Prints a full row with rank number specifier and square values.
         *
         * @param [in] state : current state
         * @param [in] rank  : rank to print
         */
        static void printRow(engine::board::State& state, int square) noexcept;

        /**
         * @brief Prints the current state infos (side to move, clock, etc...)
         *
         * @param [in] state : current state
         */
        static void printInfos(engine::board::State& state) noexcept;

        // ANSI escapes
        static constexpr std::string_view RESET = "\033[0m";
        static constexpr std::string_view RED = "\033[31m";
        static constexpr std::string_view GREEN = "\033[32m";

        static constexpr std::string_view FILES = "A     B     C     D     E     F     G     H";

        static constexpr std::string_view TOP_LINE = "┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐";

        static constexpr std::string_view IN_LINE = "├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤";

        static constexpr std::string_view BOT_LINE = "└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘";

        static constexpr std::string_view BORDER = "│";

        static constexpr std::array<std::string_view, engine::core::Pieces::PIECES> pieceSymbol = {
            "", "", "󰡜", "󰡛", "󰡚", "󰡗"};

        static constexpr std::string_view RANK_INDENT = "            ";
        static constexpr std::string_view BOARD_INDENT = "              ";
        static constexpr std::string_view FILE_IDENT = "                 ";

        static inline std::string HEADER = "";
    };
} // namespace ui::console

#endif // CONSOLE_UI_H