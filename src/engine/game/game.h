/**
 * @file      game.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game API
 */

#ifndef GAME_H_
#define GAME_H_

#include <expected>

#include "engine/board/state.h"
#include "engine/game/move_list.h"

namespace engine::game
{
    /**
     * @class Game
     * @brief Game API.
     * Holds the current state, the current legal moves list, and
     * handles the different move requests.
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
        Game(const std::string& fenNotation);

        /**
         * @brief Default destructor.
         */
        ~Game() noexcept = default;

        /**
         * @brief Play a move.
         *
         * @param [in] move  : The move to play
         */
        void playMove(const game::Move& move) noexcept;

        board::State state;      // Game's state
        game::MoveList moveList; // Legal move lists of the current halfMoveClock

      private:
        /**
         * @brief Make a catpure.
         *
         * @param [in] move       : capture move
         * @param [in] enemyColor : enemy color
         */
        void makeCapture(const game::Move& move, const core::Colors enemyColor) noexcept;

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
        void makeEnPassant(const game::Move& move, const core::Colors enemyColor) noexcept;

        /**
         * @brief Asks user for promotion option and perform it.
         *
         * @param [in] move : move leading to promotion
         */
        void makePromotion(const game::Move& move) noexcept;

        /**
         * @brief Update game state after a move was played.
         *
         * @param [in] move       : the played move
         * @param [in] enemyColor : next team to play
         */
        void update(const game::Move& move, const core::Colors enemyColor) noexcept;
    };

} // namespace engine::game

#endif // GAME_H_