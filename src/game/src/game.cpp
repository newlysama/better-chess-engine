/**
 * @file game.cpp
 * @author Thibault THOMAS
 *
 * @brief Game class implementation.
 */

/*----- Project Headers -----*/
#include "game/include/game.h"

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
        Game::Game() noexcept
            : turnCount(0)
            , colorToPlay(conf::enums::Colors::WHITE)
            , board(board::Board())
            , legalMoves{}
            {
                using namespace conf::enums;

                // Reserve predertermined space for our vectors of legal moves
                legalMoves[MoveTypes::QUIET]     .reserve(RESERVE_QUIET);
                legalMoves[MoveTypes::CAPTURE]   .reserve(RESERVE_CAPTURES);
                legalMoves[MoveTypes::PROMOTION] .reserve(RESERVE_PROMOTIONS);
                legalMoves[MoveTypes::CASTLE]    .reserve(RESERVE_CASTLES);
                legalMoves[MoveTypes::ENPASSANT] .reserve(RESERVE_ENPASSANT);
            }
    } // namespace game
} // namespace engine