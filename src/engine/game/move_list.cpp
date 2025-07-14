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

#include "engine/board/magic_const.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    using namespace engine::core;
    using namespace engine::board;

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

    inline void MoveList::processTargets(Bitboard& targets, int squareFrom, MoveTypes moveType) noexcept
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
    void MoveList::generatePawnsMoves(const State& state, Colors color) noexcept
    {
        Bitboard pawns = state.allPieces[color][Pieces::PAWN];
        Colors ennemyColor = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // While there is bits set to 1
        while (pawns.getData())
        {
            // Extract bit index to the next '1'
            int squareFrom = pawns.lsbIndex();

            // Move to the next pawn
            pawns &= ~(1ULL << squareFrom);

            // Rechable empty squares
            Bitboard pushTargets = PAWN_PUSHES_MASKS[color][squareFrom] & ~state.generalOccupancy;

            // Rechable capture square
            Bitboard captureTargets = PAWN_CAPTURES_MASKS[color][squareFrom] & ~state.coloredOccupancies[ennemyColor];

            this->processTargets(captureTargets, squareFrom, MoveTypes::CAPTURE);
            this->processTargets(pushTargets, squareFrom, MoveTypes::QUIET);
        }
    }

    void MoveList::generateKnightsMoves(const State& state, Colors color) noexcept
    {
        Bitboard knights = state.allPieces[color][Pieces::KNIGHT];
        Colors ennemyColor = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // While there is bits set to 1
        while (knights.getData())
        {
            // Extract bit index to the next '1'
            int squareFrom = knights.lsbIndex();

            // Move to the next knight
            knights &= ~(1ULL << squareFrom);

            // Get the targets
            Bitboard targets = KNIGHT_ATTACKS_MASKS[squareFrom] & ~state.coloredOccupancies[color];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.coloredOccupancies[ennemyColor];

            // Rechable squares occupied by an ennemy piece
            Bitboard captureTargets = targets & state.coloredOccupancies[ennemyColor];

            this->processTargets(captureTargets, squareFrom, MoveTypes::CAPTURE);
            this->processTargets(quietTargets, squareFrom, MoveTypes::QUIET);
        }
    }

    void MoveList::generateRooksMoves(const State& state, Colors color) noexcept
    {
        Bitboard rooks = state.allPieces[color][Pieces::ROOK];
        Colors ennemyColor = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // While there is bits set to 1
        while (rooks.getData())
        {
            // Extract bit index to the next '1'
            int squareFrom = rooks.lsbIndex();

            // Move to the next rook
            rooks &= ~(1ULL << squareFrom);

            Bitboard relevantOcc = state.generalOccupancy & ROOK_RELEVANT_MASKS[squareFrom];

            // Index the attack table using magic bitboards
            size_t magicIndex =
                (relevantOcc.getData() * rookMagics[squareFrom].getData()) >> (64 - rookShifts[squareFrom]);

            // Get the targets
            Bitboard targets = ROOK_ATTACKS_TABLE[squareFrom][magicIndex] & ~state.coloredOccupancies[color];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.coloredOccupancies[ennemyColor];

            // Rechable squares occupied by an ennemy piece
            Bitboard captureTargets = targets & state.coloredOccupancies[ennemyColor];

            this->processTargets(captureTargets, squareFrom, MoveTypes::CAPTURE);
            this->processTargets(quietTargets, squareFrom, MoveTypes::QUIET);
        }
    }

    void MoveList::generateBishopsMoves(const State& state, Colors color) noexcept
    {
        Bitboard bishops = state.allPieces[color][Pieces::BISHOP];
        Colors ennemyColor = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // While there is bits set to 1
        while (bishops.getData())
        {
            // Extract bit index to the next '1'
            int squareFrom = bishops.lsbIndex();

            // Move to the next bishop
            bishops &= ~(1ULL << squareFrom);

            Bitboard relevantOcc = state.generalOccupancy & BISHOP_RELEVANT_MASKS[squareFrom];

            // Index the attack table using magic bitboards
            size_t magicIndex =
                (relevantOcc.getData() * bishopMagics[squareFrom].getData()) >> (64 - bishopShifts[squareFrom]);

            // Get the targets
            Bitboard targets = BISHOP_ATTACKS_TABLE[squareFrom][magicIndex] & ~state.coloredOccupancies[color];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.coloredOccupancies[ennemyColor];

            // Rechable squares occupied by an ennemy piece
            Bitboard captureTargets = targets & state.coloredOccupancies[ennemyColor];

            this->processTargets(captureTargets, squareFrom, MoveTypes::CAPTURE);
            this->processTargets(quietTargets, squareFrom, MoveTypes::QUIET);
        }
    }

    void MoveList::generateQueenMoves(const State& state, Colors color) noexcept
    {
        Bitboard queen = state.allPieces[color][Pieces::QUEEN];
        Colors ennemyColor = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // While there is bits set to 1
        // Remember that thank's to promotions, we can have several queens alive
        while (queen.getData())
        {
            // Extract queen's index
            int squareFrom = queen.lsbIndex();

            // Remove queen's bit
            queen &= ~(1ULL << squareFrom);

            // Magic rook
            Bitboard rookRelevantOcc = state.generalOccupancy & ROOK_RELEVANT_MASKS[squareFrom];
            size_t rookMagicIndex =
                (rookRelevantOcc.getData() * rookMagics[squareFrom].getData()) >> (64 - rookShifts[squareFrom]);

            // Magic bishop
            Bitboard bishopRelevantOcc = state.generalOccupancy & BISHOP_RELEVANT_MASKS[squareFrom];
            size_t bishopMagicIndex =
                (bishopRelevantOcc.getData() * bishopMagics[squareFrom].getData()) >> (64 - bishopShifts[squareFrom]);

            // Combine rooks and bishops attack tables
            Bitboard attacks =
                ROOK_ATTACKS_TABLE[squareFrom][rookMagicIndex] | BISHOP_ATTACKS_TABLE[squareFrom][bishopMagicIndex];

            Bitboard targets = attacks & ~state.coloredOccupancies[color];
            Bitboard quietTargets = targets & ~state.coloredOccupancies[ennemyColor];
            Bitboard catpureTargets = targets & state.coloredOccupancies[ennemyColor];

            this->processTargets(catpureTargets, squareFrom, MoveTypes::CAPTURE);
            this->processTargets(quietTargets, squareFrom, MoveTypes::QUIET);
        }
    }

    void MoveList::generateKingMoves(const State& state, Colors color) noexcept
    {
        Bitboard king = state.allPieces[color][Pieces::KING];
        Colors ennemyColor = color == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // Extract king's index
        int squareFrom = king.lsbIndex();

        Bitboard targets = KING_ATTACKS_MASKS[squareFrom] & ~state.coloredOccupancies[color];
        Bitboard quietTargets = targets & ~state.coloredOccupancies[ennemyColor];
        Bitboard captureTargets = targets & state.coloredOccupancies[ennemyColor];

        this->processTargets(captureTargets, squareFrom, MoveTypes::CAPTURE);
        this->processTargets(quietTargets, squareFrom, MoveTypes::QUIET);
    }
} // namespace engine::game
