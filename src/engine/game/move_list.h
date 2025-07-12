/**
 * @file      move_list.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Holds the move list and handles the move generation.
 */

#ifndef MOVE_GENERATOR_H_
#define MOVE_GENERATOR_H_

#include <cstdint>

#include "conf/types.h"
#include "engine/board/board.h"
#include "engine/game/move.h"

/**
 * @namespace engine::game::move
 */
namespace engine::game::move
{
    using namespace conf::enums;
    using namespace board;

    /**
     * @class MoveList
     */
    class MoveList
    {
      public:
        /**
         * @brief Default constructor.
         */
        MoveList() noexcept = default;

        /**
         * @brief Add a new move to the moves list, throw exception if it is full.
         *
         * @param [in] move : move to add
         */
        void add(Move& move) noexcept;

        /**
         * @brief Set write index back to 0.
         */
        void clear() noexcept;

        /**
         * @brief Get the size of the list
         */
        std::size_t size() noexcept;

        /**
         * @brief Allow [index] access though the class
         *
         * @param [in] index : index to access
         */
        Move& operator[](std::size_t index) noexcept;

        /**
         * @brief Call each generation method to generate all legal moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generateAllMoves(const Board& board, Colors color) noexcept;

      private:
        /**
         * @brief Generates moves from a given square with a specific moveType and targets bitboard.
         *
         * @param [in] targets    : Bitboard of possible destination
         * @param [in] squareFrom : Starting square index
         * @param [in] moveType   : Type of move (Capture, Quiet, etc...)
         */
        inline void processTargets(Bitboard& targets, int squareFrom, MoveTypes moveType) noexcept;

        /**
         * @brief Generates pseudo-legal Pawns moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generatePawnsMoves(const Board& board, Colors color) noexcept;

        /**
         * @brief Generates pseudo-legal Knights moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generateKnightsMoves(const Board& board, Colors color) noexcept;

        /**
         * @brief Generates pseudo-legal Bishops moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generateBishopsMoves(const Board& board, Colors color) noexcept;

        /**
         * @brief Generates pseudo-legal Rooks moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generateRooksMoves(const Board& board, Colors color) noexcept;

        /**
         * @brief Generates pseudo-legal Queen moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generateQueenMoves(const Board& board, Colors color) noexcept;

        /**
         * @brief Generates pseudo-legal King moves.
         *
         * @param [in] board : The board to work with
         * @param [in] color : Color to play
         */
        void generateKingMoves(const Board& board, Colors color) noexcept;

        Move _moves[256]; // Actual list of moves
        int _size;        // Current size of the list
    };
} // namespace engine::game::move

#endif // MOVE_GENERATOR_H_