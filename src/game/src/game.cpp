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

        using namespace conf::enums;

        Game::Game() noexcept
            : turnCount(0),
              colorToPlay(conf::enums::Colors::WHITE),
              board(board::Board()),
              legalMoves{}
            {
                // Reserve predertermined space for our vectors of legal moves
                this->legalMoves[MoveTypes::QUIET]     .reserve(RESERVE_QUIET);
                this->legalMoves[MoveTypes::CAPTURE]   .reserve(RESERVE_CAPTURES);
                this->legalMoves[MoveTypes::PROMOTION] .reserve(RESERVE_PROMOTIONS);
                this->legalMoves[MoveTypes::CASTLE]    .reserve(RESERVE_CASTLES);
                this->legalMoves[MoveTypes::ENPASSANT] .reserve(RESERVE_ENPASSANT);
            }

        inline void Game::clearLegalMoves() noexcept
        {
            this->legalMoves[MoveTypes::QUIET]     .clear();
            this->legalMoves[MoveTypes::CAPTURE]   .clear();
            this->legalMoves[MoveTypes::PROMOTION] .clear();
            this->legalMoves[MoveTypes::CASTLE]    .clear();
            this->legalMoves[MoveTypes::ENPASSANT] .clear();
        }
    } // namespace game
} // namespace engine