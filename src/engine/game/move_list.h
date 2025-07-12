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
 * @namespace engine::move
 */
namespace engine::move
{
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
        void add(move::Move& move) noexcept;

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
        move::Move& operator[](std::size_t index) noexcept;

        /**
         * @brief Call each generation method to generate all legal moves.
         *
         * @param [in] board : The board to work with
         */
        void generateAllMoves(const board::Board& board) noexcept;

      private:
        /**
         * @brief Generates pseudo-legal Pawns moves.
         *
         * @param [in] board : The board to work with
         */
        inline void generatePawnsMoves(const board::Board& board) noexcept;

        /**
         * @brief Generates pseudo-legal Knights moves.
         *
         * @param [in] board : The board to work with
         */
        inline void generateKnightsMoves(const board::Board& board) noexcept;

        /**
         * @brief Generates pseudo-legal Bishops moves.
         *
         * @param [in] board : The board to work with
         */
        inline void generateBishopsMoves(const board::Board& board) noexcept;

        /**
         * @brief Generates pseudo-legal Rooks moves.
         *
         * @param [in] board : The board to work with
         */
        inline void generateRooksMoves(const board::Board& board) noexcept;

        /**
         * @brief Generates pseudo-legal Queen moves.
         *
         * @param [in] board : The board to work with
         */
        inline void generateQueenMoves(const board::Board& board) noexcept;

        /**
         * @brief Generates pseudo-legal King moves.
         *
         * @param [in] board : The board to work with
         */
        inline void generateKingMoves(const board::Board& board) noexcept;

        move::Move _moves[256]; // Actual list of moves
        int _size;              // Current size of the list
    };
} // namespace engine::move

#endif // MOVE_GENERATOR_H_