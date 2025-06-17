/**
 * @file game.h
 * @author Thibault THOMAS
 *
 * @brief Game state representation
 */

#ifndef GAME_H_
#define GAME_H_

/*----- System -----*/
#include <cstdint>

/*----- Project Headers -----*/
#include "conf/enums.h"
#include "board/include/board.h"

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace game
     */
    namespace game
    {
        /**
         * @class Game
         * @brief Holds the game state
         */
        class Game
        {
            public:
                constexpr Game() noexcept;
                ~Game() noexcept = default;

            private:
                uint16_t turnCount;               //< Number of played tunred
                conf::enums::Colors colorsToPlay; //< Whose turn is it ? :)

                engine::board::Board board; //< Game's board
        };
    } // namespace game
} // namespace engine

#endif // GAME_H_