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
#include "engine/board/state.h"
#include "engine/game/move_list.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{
    using namespace conf::enums;
    using namespace engine::board;
    using namespace engine::game;

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

        // /**
        //  * @brief Make a move.
        //  *
        //  * @param [in] move  : The move to make
        //  */
        // void makeMove(const Move move) noexcept;

        // /**
        //  * @brief Unmake a move.
        //  *
        //  * @param [in] move  : The move to unmake
        //  */
        // void unmakeMove(const Move move) noexcept;

        State state; // Game's board
    };

} // namespace engine::game

#endif // GAME_H_