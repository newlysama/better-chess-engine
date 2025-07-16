/**
 * @file      state.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief State class implementation.
 */

#include "engine/board/state.h"

#include "engine/core/const.h"
#include "logging/logging.h"
#include "utils/enums_to_string.h"

#if !defined(BUILD_RELEASE)
#include "utils/utils.h"
#endif

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace engine::core;

    State::State() noexcept
        : halfMoveClock(0)
        , fullMoveClock(0)
        , sideToMove(Colors::WHITE)
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

    Pieces State::getPiece(int squareIndex) const noexcept
    {
        for (Colors color : {Colors::WHITE, Colors::BLACK})
        {
            for (Pieces piece = Pieces::PAWN; piece < Pieces::PIECES; piece = static_cast<Pieces>(piece + 1))
            {
                if (this->allPieces[color][piece].isSet(squareIndex))
                {
                    return piece;
                }
            }
        }

        LOG_ERROR("[State::getPiece()] Trying to access piece at square {}, but no piece was found.", squareIndex);
        return Pieces::UNKNOWN_PIECE;
    }

    Pieces State::getPiece(const Colors color, const int squareIndex) const noexcept
    {
        for (Pieces piece = Pieces::PAWN; piece < Pieces::PIECES; piece = static_cast<Pieces>(piece + 1))
        {
            if (this->allPieces[color][piece].isSet(squareIndex))
            {
                return piece;
            }
        }

        LOG_ERROR("[State::getPiece(Color)] Trying to access piece of team {} at square {}, but no piece was found.",
                  utils::toString(color), squareIndex);

        return Pieces::UNKNOWN_PIECE;
    }

    void State::setPiece(const Colors color, const Pieces piece, const int squareIndex) noexcept
    {
        this->allPieces[color][piece].set(squareIndex);
        this->generalOccupancy.set(squareIndex);
        this->coloredOccupancies[color].set(squareIndex);

        LOG_DEBUG("[State::setPiece()] Added {} {} to {}", utils::toString(color), utils::toString(piece),
                  utils::squareIndexToString(squareIndex));
    }

    void State::unsetPiece(const Colors color, const Pieces piece, const int squareIndex) noexcept
    {
        this->allPieces[color][piece].unset(squareIndex);
        this->generalOccupancy.unset(squareIndex);
        this->coloredOccupancies[color].unset(squareIndex);

        LOG_DEBUG("[State::unsetPiece()] Removed {} {} from {}", utils::toString(color), utils::toString(piece),
                  utils::squareIndexToString(squareIndex));
    }

    void State::movePiece(const Colors color, const Pieces piece, const int fromSquare, const int toSquare) noexcept
    {
        this->unsetPiece(color, piece, fromSquare);
        this->setPiece(color, piece, toSquare);

        LOG_INFO("[State::movePiece()] Moved {} {} from {} to {}", utils::toString(color), utils::toString(piece),
                 utils::squareIndexToString(fromSquare), utils::squareIndexToString(toSquare));
    }

} // namespace engine::board