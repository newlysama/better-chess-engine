/**
 * @file board.cpp
 * @author Thibault THOMAS
 *
 * @brief Board class implementation.
 */

#include "board/board.h"

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace board
     */
    namespace board
    {
        Board::Board() noexcept
            : allPieces{{// -- White pieces (index 0) --
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
                         }}},
              generalOccupancy{}, whiteOccupancy{}, blackOccupancy{}
        {
            // construit les bitboards d'occupancy
            for (size_t piece = 0; piece < conf::enums::Pieces::PIECES; piece++)
            {
                whiteOccupancy |= allPieces[conf::enums::Colors::WHITE][piece];
                blackOccupancy |= allPieces[conf::enums::Colors::BLACK][piece];
            }

            generalOccupancy = whiteOccupancy | blackOccupancy;
        }

    } // namespace board
} // namespace engine