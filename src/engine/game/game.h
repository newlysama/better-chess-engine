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

#include "conf/enums.h"
#include "conf/types.h"
#include "engine/board/board.h"
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

      private:
        uint16_t turnCount;              // Number of played tunred
        conf::enums::Colors colorToPlay; // Whose turn is it ? :)

        engine::board::Board board;      // Game's board
        engine::move::MoveList moveList; // List of moves
    };

} // namespace engine::game

#endif // GAME_H_