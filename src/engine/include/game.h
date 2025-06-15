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
#include "engine/include/board.h"

/**
 * @namespace engine
 */
namespace engine
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
            uint16_t turnCount;           //< Number of played tunred
            conf::enums::Team teamToPlay; //< Whose turn is it ? :)

            engine::Board board; //< Game's board
    };
} // namespace engine

#endif // GAME_H_