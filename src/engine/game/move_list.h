/**
 * @file      move_list.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Holds the move list and handles the move generation.
 */

#ifndef MOVE_LIST_H_
#define MOVE_LIST_H_

#include "engine/board/state.h"
#include "engine/game/move.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    /**
     * @class MoveList
     */
    class MoveList
    {
      public:
        /**
         * @brief Constructor.
         */
        MoveList() noexcept;

        /**
         * @brief Add a new move to the moves list, throw exception if it is full.
         *
         * @param [in] move : move to add
         */
        void add(const Move& move) noexcept;

        /**
         * @brief Set write index back to 0.
         */
        void clear() noexcept;

        /**
         * @brief Get the size of the list
         */
        std::size_t size() const noexcept;

        /**
         * @brief Allow [index] access though the class
         *
         * @param [in] index : index to access
         */
        Move& operator[](std::size_t index) noexcept;

        /**
         * @brief Check if a given move is present in the list.
         *
         * @param [in] move : the move to look for
         * @return bool : weather the move is present or not
         */
        bool contains(const game::Move move) noexcept;

        /**
         * @brief Make a move.
         *
         * @param [in] state : The state to make the move on
         * @param [in] move  : The move to make
         */
        void makeMove(board::State& state, const game::Move& move) noexcept;

        /**
         * @brief Unmake a move.
         *
         * @param [in] state : The state to unmake the move on
         * @param [in] move  : The move to unmake
         */
        void unmakeMove(board::State& state, const game::Move& move) noexcept;

        /**
         * @brief Call each generation method to generate all legal moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateAllMoves(const board::State& state) noexcept;

      private:
        /**
         * @brief Generates legal moves from a given square with a specific moveType and targets bitstate.
         *
         * @param [in] targets    : Bitboard of possible destinations
         * @param [in] squareFrom : Starting square index
         * @param [in] moveType   : Type of move (Capture, Quiet, etc...)
         */
        void processTargets(board::Bitboard& targets, int squareFrom, core::MoveTypes moveType,
                            core::Pieces fromPiece) noexcept;

        /**
         * @brief If castling enabled, add the corresponding move to the movelist.
         *
         * @param [in] state      : Current state of the board
         * @param [in] fromSquare : Current king square
         */
        template <core::Castlings C>
        void getCastlingMoves(const board::State& state, int fromSquare) noexcept;

        /**
         * @brief If enPassant enabled, add the corresponding move the the movelist;
         *
         * @param [in] state      : Current state of the board
         * @param [in] fromSquare : Current king square
         */
        void getEnPassantMoves(const board::State& state, int fromSquare) noexcept;

        /**
         * @brief Generates legal Pawns moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generatePawnsMoves(const board::State& state) noexcept;

        /**
         * @brief Generates legal Knights moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateKnightsMoves(const board::State& state) noexcept;

        /**
         * @brief Generates legal Bishops moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateBishopsMoves(const board::State& state) noexcept;

        /**
         * @brief Generates legal Rooks moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateRooksMoves(const board::State& state) noexcept;

        /**
         * @brief Generates legal Queen moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateQueenMoves(const board::State& state) noexcept;

        /**
         * @brief Generates legal King moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateKingMoves(const board::State& state) noexcept;

        Move _moves[256]; // Actual list of moves
        int _size;        // Current size of the list
    };
} // namespace engine::game

#endif // MOVE_LIST_H_