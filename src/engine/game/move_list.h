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
#include <stack>

#include "conf/types.h"
#include "engine/board/state.h"
#include "engine/game/move.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    using namespace conf::enums;
    using namespace engine::board;

    /**
     * @typedef UnmakeInfo
     * @brief Contains all the necessary info to unmake a move.
     */
    typedef struct UnmakeInfo
    {
        Move move;
        Pieces captured;
        State* previous;
    } UnmakeInfo;

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
         * @brief Make a move.
         *
         * @param [in] state : The state to make the move on
         * @param [in] move  : The move to make
         */
        void makeMove(State& state, const Move& move) noexcept;

        /**
         * @brief Unmake a move.
         *
         * @param [in] state : The state to unmake the move on
         * @param [in] move  : The move to unmake
         */
        void unmakeMove(State& state, const Move& move) noexcept;

        /**
         * @brief Call each generation method to generate all legal moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateAllMoves(const State& state, Colors color) noexcept;

      private:
        /**
         * @brief Generates legal moves from a given square with a specific moveType and targets bitstate.
         *
         * @param [in] targets    : Bitboard of possible destinations
         * @param [in] squareFrom : Starting square index
         * @param [in] moveType   : Type of move (Capture, Quiet, etc...)
         */
        inline void processTargets(Bitboard& targets, int squareFrom, MoveTypes moveType) noexcept;

        /**
         * @brief Generates legal Pawns moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generatePawnsMoves(const State& state, Colors color) noexcept;

        /**
         * @brief Generates legal Knights moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateKnightsMoves(const State& state, Colors color) noexcept;

        /**
         * @brief Generates legal Bishops moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateBishopsMoves(const State& state, Colors color) noexcept;

        /**
         * @brief Generates legal Rooks moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateRooksMoves(const State& state, Colors color) noexcept;

        /**
         * @brief Generates legal Queen moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateQueenMoves(const State& state, Colors color) noexcept;

        /**
         * @brief Generates legal King moves.
         *
         * @param [in] state : The state to work with
         * @param [in] color : Color to play
         */
        void generateKingMoves(const State& state, Colors color) noexcept;

        Move _moves[256];                // Actual list of moves
        int _size;                       // Current size of the list
        std::stack<UnmakeInfo> _history; // History of made moves
    };
} // namespace engine::game

#endif // MOVE_GENERATOR_H_