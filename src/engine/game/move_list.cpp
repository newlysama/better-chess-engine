/**
 * @file      move_list.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Implements MoveList class.
 */

#include "move_list.h"

#include <assert.h>

/**
 * @namespace engine::game::move
 */
namespace engine::game::move
{
    using namespace conf::enums;
    using namespace engine::board;
    using namespace engine::board::mask;

    void MoveList::add(Move& move) noexcept
    {
        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            assert(this->_size < 256);
        #endif
        // clang-format on

        this->_moves[this->_size++] = move;
    }

    void MoveList::clear() noexcept
    {
        this->_size = 0;
    }

    std::size_t MoveList::size() noexcept
    {
        return this->_size;
    }

    Move& MoveList::operator[](std::size_t index) noexcept
    {
        return this->_moves[index];
    }

    inline void MoveList::processTargets(board::Bitboard& targets, int squareFrom, MoveTypes moveType) noexcept
    {
        while (targets.getData())
        {
            int squareTo = targets.lsbIndex();
            Move move{squareFrom, squareTo, moveType};
            this->add(move);

            // Move to the next target
            targets &= ~(1ULL << squareTo);
        }
    }

    /**
     * @todo Generate EnPassant and Promotions
     */
    void MoveList::generatePawnsMoves(const board::Board& board, Colors color) noexcept
    {
        Bitboard pawns = board.allPieces[color][PAWN];
        Colors ennemyColor = color == WHITE ? BLACK : WHITE;

        // While there is bits set to 1 in pawns' bitboard
        while (pawns.getData())
        {
            // Extract bit index to the next '1'
            int squareFrom = pawns.lsbIndex();

            // Move to the next pawn
            pawns &= ~(1ULL << squareFrom);

            // Rechable empty squares
            Bitboard pushTargets = PAWN_PUSHES_MASKS[color][squareFrom] & ~board.generalOccupancy;

            // Rechable capture square
            Bitboard captureTargets = PAWN_CAPTURES_MASKS[color][squareFrom] & ~board.coloredOccupancies[ennemyColor];

            this->processTargets(captureTargets, squareFrom, CAPTURE);
            this->processTargets(pushTargets, squareFrom, QUIET);
        }
    }

    void MoveList::generateKnightsMoves(const board::Board& board, Colors color) noexcept
    {
        Bitboard knights = board.allPieces[color][KNIGHT];
        Colors ennemyColor = color == WHITE ? BLACK : WHITE;

        // While there is bits set to 1 in knights' bitboard
        while (knights.getData())
        {
            // Extract bit index to the next '1'
            int squareFrom = knights.lsbIndex();

            // Move to the next knight
            knights &= ~(1ULL << squareFrom);

            Bitboard targets = KNIGHT_ATTACKS_MASKS[squareFrom] & ~board.coloredOccupancies[color];

            // Rechable empty squares
            Bitboard quietTargets = KNIGHT_ATTACKS_MASKS[squareFrom] & ~board.coloredOccupancies[color];

            // Rechable squares occupied by an ennemy piece
            Bitboard captureTargets = quietTargets & board.coloredOccupancies[ennemyColor];

            this->processTargets(captureTargets, squareFrom, CAPTURE);
            this->processTargets(quietTargets, squareFrom, QUIET);
        }
    }
} // namespace engine::game::move
