/**
 * @file      state.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief State class implementation.
 */

#include "engine/board/state.h"

#include <exception>

#include "logging/logging.h"
#include "utils/enums_to_string.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace engine::core;

    State::State() noexcept
        : castlingRights((1 << core::Castlings::CASTLINGS) - 1)
        , whitePinned{}
        , blackPinned{}
        , allPieces{
            {// -- White pieces (index 0) --
             {
                 Bitboard{0x0000'0000'0000'FF00ULL}, // pawns
                 Bitboard{0x0000'0000'0000'0042ULL}, // knights
                 Bitboard{0x0000'0000'0000'0024ULL}, // bishops
                 Bitboard{0x0000'0000'0000'0081ULL}, // rooks
                 Bitboard{0x0000'0000'0000'0008ULL}, // queen
                 Bitboard{0x0000'0000'0000'0010ULL}  // king
             }, // -- Black pieces (index 1) --
             {
                 Bitboard{0x00FF'0000'0000'0000ULL}, // pawns
                 Bitboard{0x4200'0000'0000'0000ULL}, // knights
                 Bitboard{0x2400'0000'0000'0000ULL}, // bishops
                 Bitboard{0x8100'0000'0000'0000ULL}, // rooks
                 Bitboard{0x0800'0000'0000'0000ULL}, // queen
                 Bitboard{0x1000'0000'0000'0000ULL}  // king
             }}
            },
        generalOccupancy{}, coloredOccupancies{}
    {
        // Builds occupancy bitboards
        for (std::size_t piece = 0; piece < Pieces::PIECES; piece++)
        {
            this->coloredOccupancies[Colors::WHITE] |= this->allPieces[Colors::WHITE][piece];
            this->coloredOccupancies[Colors::BLACK] |= this->allPieces[Colors::BLACK][piece];
        }

        this->generalOccupancy = this->coloredOccupancies[Colors::WHITE] | this->coloredOccupancies[Colors::BLACK];
    }

    State::State(const std::array<std::string, 6> fenNotation)
    {
        std::expected<void, std::string> res{};

        res = this->setOccupanciesFromFen(fenNotation[0]);
        if (!res.has_value())
        {
            LOG_ERROR(res.error());
            throw std::invalid_argument(res.error());
        }

        res = this->setSideToMoveFromFen(fenNotation[1]);
        if (!res.has_value())
        {
            LOG_ERROR(res.error());
            throw std::invalid_argument(res.error());
        }

        res = this->setCastlingRightsFromFen(fenNotation[2]);
        if (!res.has_value())
        {
            LOG_ERROR(res.error());
            throw std::invalid_argument(res.error());
        }

        res = this->setEnPassantSquareFromFen(fenNotation[3]);
        if (!res.has_value())
        {
            LOG_ERROR(res.error());
            throw std::invalid_argument(res.error());
        }

        res = this->setHalfMoveClockFromFen(fenNotation[4]);
        if (!res.has_value())
        {
            LOG_ERROR(res.error());
            throw std::invalid_argument(res.error());
        }

        res = this->setFullMoveClockFromFen(fenNotation[5]);
        if (!res.has_value())
        {
            LOG_ERROR(res.error());
            throw std::invalid_argument(res.error());
        }
    }

    void State::checkCastlingRemoval(const Pieces piece, const int fromSquare, const int toSquare) noexcept
    {
        if (this->sideToMove == Colors::WHITE &&
            (this->hasCastlingRight<Castlings::WHITE_KING_SIDE>() || this->hasCastlingRight<WHITE_QUEEN_SIDE>()))
        {
            if (piece == Pieces::KING)
            {
                this->clearCastlingRight<Castlings::WHITE_KING_SIDE>();
                this->clearCastlingRight<Castlings::WHITE_QUEEN_SIDE>();
                LOG_DEBUG("Removed all white team castling rights");
            }
            else if (piece == Pieces::ROOK && fromSquare == 0) // Initial white queen side rook position
            {
                this->clearCastlingRight<Castlings::WHITE_QUEEN_SIDE>();
                LOG_DEBUG("Removed white queen side castling right");
            }
            else if (piece == Pieces::ROOK && fromSquare == 7) // Initial white king side rook position
            {
                this->clearCastlingRight<Castlings::WHITE_KING_SIDE>();
                LOG_DEBUG("Removed white king side castling right");
            }
        }

        if (this->sideToMove == Colors::BLACK &&
            (this->hasCastlingRight<Castlings::BLACK_KING_SIDE>() || this->hasCastlingRight<BLACK_QUEEN_SIDE>()))
        {
            if (piece == Pieces::KING)
            {
                this->clearCastlingRight<Castlings::BLACK_KING_SIDE>();
                this->clearCastlingRight<Castlings::BLACK_QUEEN_SIDE>();
                LOG_DEBUG("Removed all black team castling rights");
            }
            else if (piece == Pieces::ROOK && fromSquare == 56) // Initial black queen side rook position
            {
                this->clearCastlingRight<Castlings::BLACK_QUEEN_SIDE>();
                LOG_DEBUG("Removed black queen side castling right");
            }
            else if (piece == Pieces::ROOK && fromSquare == 63) // Initial black king side rook position
            {
                this->clearCastlingRight<Castlings::BLACK_KING_SIDE>();
                LOG_DEBUG("Removed black king side castling right");
            }
        }

        // Check if we are removing enemy's castling right by capturing a rook
        Colors enemyColor = (this->sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE);

        // If target piece is a rook
        if (this->getPiece(toSquare))
        {
            if (enemyColor == Colors::WHITE)
            {
                if (toSquare == 0) // Initial white queen side rook position
                {
                    this->clearCastlingRight<Castlings::WHITE_QUEEN_SIDE>();
                    LOG_DEBUG("Removed white queen side castling right by capture");
                }
                else if (toSquare == 7) // Initial white king side rook position
                {
                    this->clearCastlingRight<Castlings::WHITE_KING_SIDE>();
                    LOG_DEBUG("Removed white king side castling right by capture");
                }
            }
            else
            {
                if (toSquare == 56) // Initial black queen side rook position
                {
                    this->clearCastlingRight<Castlings::BLACK_QUEEN_SIDE>();
                    LOG_DEBUG("Removed black queen side castling right by capture");
                }
                else if (toSquare == 63) // Initial black king side rook position
                {
                    this->clearCastlingRight<Castlings::BLACK_KING_SIDE>();
                    LOG_DEBUG("Removed black king side castling right by capture");
                }
            }
        }
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

        LOG_ERROR("Trying to access piece at square {}, but no piece was found", squareIndex);
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

        LOG_ERROR("Trying to access piece of team {} at square {}, but no piece was found", utils::toString(color),
                  squareIndex);

        return Pieces::UNKNOWN_PIECE;
    }

    void State::setPiece(const Colors color, const Pieces piece, const int squareIndex) noexcept
    {
        this->allPieces[color][piece].set(squareIndex);
        this->generalOccupancy.set(squareIndex);
        this->coloredOccupancies[color].set(squareIndex);

        LOG_DEBUG("Added {} {} to {}", utils::toString(color), utils::toString(piece),
                  utils::squareIndexToString(squareIndex));
    }

    void State::unsetPiece(const Colors color, const Pieces piece, const int squareIndex) noexcept
    {
        this->allPieces[color][piece].unset(squareIndex);
        this->generalOccupancy.unset(squareIndex);
        this->coloredOccupancies[color].unset(squareIndex);

        LOG_DEBUG("Removed {} {} from {}", utils::toString(color), utils::toString(piece),
                  utils::squareIndexToString(squareIndex));
    }

    void State::getPinnedPieces(const core::Colors color) noexcept
    {
    }

    void State::movePiece(const Colors color, const Pieces piece, const int fromSquare, const int toSquare) noexcept
    {
        // Keep track of the kings indices
        if (fromSquare == this->whiteKingIndex)
        {
            this->whiteKingIndex = toSquare;
        }
        else if (fromSquare == blackKingIndex)
        {
            this->blackKingIndex = toSquare;
        }

        this->unsetPiece(color, piece, fromSquare);
        this->setPiece(color, piece, toSquare);

        // If we move a rook or the king, check for castling rights removal
        this->checkCastlingRemoval(piece, fromSquare, toSquare);

        LOG_INFO("Moved {} {} from {} to {}", utils::toString(color), utils::toString(piece),
                 utils::squareIndexToString(fromSquare), utils::squareIndexToString(toSquare));
    }

} // namespace engine::board