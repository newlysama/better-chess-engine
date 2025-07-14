/**
 * @file      state.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief State class implementation.
 */

#include "engine/board/state.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace engine::core;

    State::State() noexcept
        : halfMoveClock(0)
        , fullMoveClock(0)
        , colorToPlay(Colors::WHITE)
        , castlingRights(uint8_t(0))
        ,allPieces{
                    {
                        // -- White pieces (index 0) --
                        {
                            Bitboard{0x0000'0000'0000'FF00ULL}, // pawns
                            Bitboard{0x0000'0000'0000'0042ULL}, // knights
                            Bitboard{0x0000'0000'0000'0024ULL}, // bishops
                            Bitboard{0x0000'0000'0000'0081ULL}, // rooks
                            Bitboard{0x0000'0000'0000'0008ULL}, // queen
                            Bitboard{0x0000'0000'0000'0010ULL}  // king
                        },
                        // -- Black pieces (index 1) --
                        {
                            Bitboard{0x00FF'0000'0000'0000ULL}, // pawns
                            Bitboard{0x4200'0000'0000'0000ULL}, // knights
                            Bitboard{0x2400'0000'0000'0000ULL}, // bishops
                            Bitboard{0x8100'0000'0000'0000ULL}, // rooks
                            Bitboard{0x0800'0000'0000'0000ULL}, // queen
                            Bitboard{0x1000'0000'0000'0000ULL}  // king
                        }
                    }
                }
        , generalOccupancy{}
        , coloredOccupancies{}
    {
        // Builds occupancy bitboards
        for (std::size_t piece = 0; piece < Pieces::PIECES; piece++)
        {
            this->coloredOccupancies[Colors::WHITE] |= this->allPieces[Colors::WHITE][piece];
            this->coloredOccupancies[Colors::BLACK] |= this->allPieces[Colors::BLACK][piece];
        }

        this->generalOccupancy = this->coloredOccupancies[Colors::WHITE] | this->coloredOccupancies[Colors::BLACK];
    }

} // namespace engine::board