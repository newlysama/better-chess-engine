/**
 * @file      printer.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Allows to print the game state in stdout.
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#include <print>

#include "engine/board/state.h"
#include "utils/enums_to_string.h"

/**
 * @namespace engine::printer
 */
namespace engine::printer
{
    // Print colors
    inline constexpr std::string_view RESET = "\033[0m";
    inline constexpr std::string_view RED = "\033[31m";
    inline constexpr std::string_view GREEN = "\033[32m";

    // Array [color][piece] for colored pieces printing
    //  color 0 = black, 1 = white
    inline constexpr std::array<std::array<std::string_view, 6>, 2> PIECE_STR = {
        {
         {// white:   P N B R Q K
             "\033[32m\033[0m", "\033[32m\033[0m", "\033[32m󰡜\033[0m", "\033[32m󰡛\033[0m",
             "\033[32m󰡚\033[0m", "\033[32m󰡗\033[0m"},
         {// black:   p n b r q k
             "\033[31m\033[0m", "\033[31m\033[0m", "\033[31m󰡜\033[0m", "\033[31m󰡛\033[0m",
             "\033[31m󰡚\033[0m", "\033[31m󰡗\033[0m"},
         }
    };

    // Tabs to center a bit the state and the header on the terminal
    const std::string HEADER_TAB = "                        ";
    const std::string BOARD_TAB = "              ";

    /**
     * @brief Prints the state's print function header.
     */
    inline void printHeader() noexcept
    {
        std::println("{}   ________  __________________", HEADER_TAB);
        std::println("{}  / ____/ / / / ____/ ___/ ___/", HEADER_TAB);
        std::println("{} / /   / /_/ / __/  \\__ \\__ \\", HEADER_TAB);
        std::println("{}/ /___/ __  / /___ ___/ /__/ /", HEADER_TAB);
        std::println("{}\\____/_/ /_/_____//____/____/", HEADER_TAB);

        std::println();
    }

    /**
     * @brief Prints the tab before the state, the rank's number and the opening border.
     */
    inline void printRankStart(const int rankIndex) noexcept
    {
        std::print("{}{} |", BOARD_TAB, rankIndex);
    }

    /**
     * @brief Prints the files letters.
     */
    inline void printFiles() noexcept
    {
        std::print("{}  ", BOARD_TAB);

        for (int file = 0; file < 8; file++)
        {
            std::print("   {}  ", static_cast<char>('A' + file));
        }
    }

    /**
     * @brief Get a colored string representing a piece on a specific square.
     */
    inline constexpr std::string_view getPiece(const board::State& state, int squareIndex)
    {
        for (std::size_t color = 0; color < core::Colors::COLORS; color++)
        {
            for (std::size_t piece = 0; piece < core::Pieces::PIECES; piece++)
            {
                // If allPieces[color][piece] is set to 1 (piece is present)
                if ((state.allPieces[color][piece].getData() >> squareIndex) & 1ULL)
                {
                    return PIECE_STR[color][piece];
                }
            }
        }

        // Empty square
        return " ";
    }

    /**
     * @brief Prints the state.
     */
    inline void printBoard(const board::State& state) noexcept
    {
        for (int rankIndex = 7; rankIndex >= 0; rankIndex--)
        {
            std::println("{}  -------------------------------------------------", BOARD_TAB);
            std::println("{}  |     |     |     |     |     |     |     |     |", BOARD_TAB);

            printRankStart(rankIndex + 1);

            for (int fileIndex = 0; fileIndex < 8; fileIndex++)
            {
                // Get the piece to print
                int squareIndex = board::State::getSquareIndex(rankIndex, fileIndex);
                std::string_view piece = getPiece(state, squareIndex);

                std::print("  {}  |", piece);
            }
            std::println();

            std::println("{}  |     |     |     |     |     |     |     |     |", BOARD_TAB);
        }

        std::println("{}  -------------------------------------------------", BOARD_TAB);

        printFiles();

        std::println("\n\n");
    }

    inline void printState(const board::State& state) noexcept
    {
        printHeader();
        printBoard(state);

        std::println("Side to move:    {}", utils::toString(state.sideToMove));
        std::println("Half Move Clock: {}", state.halfMoveClock);
        std::println("Full Move Clock: {}", state.fullMoveClock);
    }
} // namespace engine::printer

#endif // PRINTER_H_