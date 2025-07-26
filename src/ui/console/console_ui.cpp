/**
 * @file      console_ui.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Console rendering implem.
 */

#include "ui/console/console_ui.h"

#include <fstream>
#include <print>

#include "conf/paths.h"
#include "logging/logging.h"
#include "utils/enums_to_string.h"

namespace ui::console
{
    using namespace conf::paths;
    using namespace engine::core;
    using namespace engine::board;

    void ConsoleUI::loadHeader() noexcept
    {
        // Load header file
        std::ifstream headerFile(consoleUIHeaderPath.data());

        if (!headerFile)
        {
            ConsoleUI::HEADER = "";
            LOG_WARN("Could not open header file at: {}", consoleUIHeaderPath);

            return;
        }

        std::string line;
        std::string header;

        while (std::getline(headerFile, line))
        {
            header.append(line + "\n");
        }

        ConsoleUI::HEADER = header;

        headerFile.close();
    }

    std::string ConsoleUI::getColoredPiece(Color color, Piece piece) noexcept
    {
        std::string coloredPiece;

        if (color == Color::WHITE)
        {
            coloredPiece = std::format("{}{}{}", ConsoleUI::GREEN, ConsoleUI::pieceSymbol[piece], ConsoleUI::RESET);

            return coloredPiece;
        }
        else if (color == Color::BLACK)
        {
            coloredPiece = std::format("{}{}{}", ConsoleUI::RED, ConsoleUI::pieceSymbol[piece], ConsoleUI::RESET);

            return coloredPiece;
        }

        LOG_WARN("Unknown color: {}", utils::toString(color));
        return "";
    }

    std::string ConsoleUI::getSquare(State& state, int square) noexcept
    {
        for (int color = 0; color < Color::N_COLORS; color++)
        {
            for (int piece = 0; piece < Piece::N_PIECES; piece++)
            {
                Bitboard bb = state.m_piecesBB[color][piece];
                if (bb.isSet(square))
                {
                    return ConsoleUI::getColoredPiece(static_cast<Color>(color), static_cast<Piece>(piece));
                }
            }
        }

        // No piece, empty square
        return " ";
    }

    void ConsoleUI::printHeader() noexcept
    {
        std::println("{}", ConsoleUI::HEADER);
        std::println();
    }

    void ConsoleUI::printRow(State& state, int rank) noexcept
    {
        // Indent the printing
        std::print(ConsoleUI::RANK_INDENT);

        // Print rank specifier
        std::print("{} ", rank + 1);

        for (int file = 0; file < 8; file++)
        {
            int square = State::getSquareIndex(rank, file);
            std::string squareVal = ConsoleUI::getSquare(state, square);

            std::print("{}  {}  ", ConsoleUI::BORDER, squareVal);
        }

        // Print rank specifier
        std::println("{} {}", ConsoleUI::BORDER, rank + 1);

        // Print in-board line speparator
        if (rank > 0)
        {
            std::println("{}{}", ConsoleUI::BOARD_INDENT, ConsoleUI::IN_LINE);
        }
    }

    void ConsoleUI::printInfos(engine::board::State& state) noexcept
    {
        std::println("{}Half move clock : {}", ConsoleUI::RANK_INDENT, state.m_halfMoveClock);
        std::println("{}Full move clock : {}", ConsoleUI::RANK_INDENT, state.m_fullMoveClock);
        std::println("{}Side to move    : {}", ConsoleUI::RANK_INDENT, utils::toString(state.m_sideToMove));
    }

    void ConsoleUI::RenderState(engine::board::State& state) noexcept
    {
        ConsoleUI::printHeader();

        // Print files specifiers
        std::println("{}{}", ConsoleUI::FILE_IDENT, ConsoleUI::N_FILES);

        // Print top line
        std::println("{}{}", ConsoleUI::BOARD_INDENT, ConsoleUI::TOP_LINE);

        // Print each row
        for (int rank = 7; rank >= 0; rank--)
        {
            ConsoleUI::printRow(state, rank);
        }

        // Print the bottom line and files specifiers
        std::println("{}{}", ConsoleUI::BOARD_INDENT, ConsoleUI::BOT_LINE);
        std::println("{}{}", ConsoleUI::FILE_IDENT, ConsoleUI::N_FILES);

        std::println();

        ConsoleUI::printInfos(state);
    }

} // namespace ui::console