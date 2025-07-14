/**
 * @file      game.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game class implementation.
 */

#include "engine/game/game.h"

/**
 * @namespace engine::game
 */
namespace engine::game
{

    using namespace conf::enums;
    using namespace engine::board;
    using namespace engine::game;

    Game::Game() noexcept
        : state(State{})
    {
    }
} // namespace engine::game