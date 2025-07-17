/**
 * @file      game.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game state representation
 */

#ifndef GAME_H_
#define GAME_H_

#include <cstdint>

#include "engine/board/state.h"
#include "engine/game/move_list.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    /**
     * @class Game
     * @brief Holds the game state
     */
    class Game
    {
      public:
        /**
         * @brief Constructor.
         */
        Game() noexcept;

        /**
         * @brief Constructor from FEN notation.
         */
        Game(const std::string_view fenNotation) noexcept;

        /**
         * @brief Default destructor.
         */
        ~Game() noexcept = default;

        /**
         * @brief Ask the user the move he wants to play.
         *
         * @return std::string : the user's input
         */
        std::string askInput() const noexcept;

        /**
         * @brief Parses a user input and use it to build a move.
         *
         * @param [in] input : the user input
         * @return Move : the builded move / Empty move if the input is not valid
         */
        game::Move inputToMove(std::string input) noexcept;

        /**
         * @brief Special function make castling move.
         *
         * @param [in] move : the castling move
         */
        void makeCastling(const game::Move& move) noexcept;

        /**
         * @brief Special function to make enPassant move.
         *
         * @param [in] move       : the enPassant move
         * @param [in] enemyColor : color of the captured piece
         */
        void makeEnPassant(const game::Move& move, core::Colors enemyColor) noexcept;

        /**
         * @brief Update game state after a move was played.
         *
         * @param [in] move       : the played move
         * @param [in] enemyColor : next team to play
         */
        void update(const game::Move& move, core::Colors enemyColor) noexcept;

        /**
         * @brief Play a move.
         *
         * @param [in] move  : The move to play
         */
        void playMove(const game::Move& move) noexcept;

        board::State state;      // Game's state
        game::MoveList moveList; // Legal move lists of the current halfMoveClock
    };

} // namespace engine::game

#endif // GAME_H_