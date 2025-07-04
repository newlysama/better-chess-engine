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
#include "engine/game/move_generator.h"

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
        Game(const std::string fenNotation) noexcept;

        /**
         * @brief Default destructor.
         */
        ~Game() noexcept = default;

        /**
         * @brief Clears the vectors of legal moves.
         */
        inline void clearLegalMoves() noexcept;

      private:
        uint16_t turnCount;              // Number of played tunred
        conf::enums::Colors colorToPlay; // Whose turn is it ? :)

        engine::board::Board board; // Game's board

        conf::types::LegalMoves legalMoves; // Vectors of legal moves
    };

    // Constant variables defining the sizes to reserve for our vectors of legal moves
    constexpr size_t RESERVE_QUIET = 64;
    constexpr size_t RESERVE_CAPTURES = 32;
    constexpr size_t RESERVE_PROMOTIONS = 32;
    constexpr size_t RESERVE_CASTLES = 2;
    constexpr size_t RESERVE_ENPASSANT = 1;

} // namespace engine::game

#endif // GAME_H_