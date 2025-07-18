/**
 * @file      state.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief State representation.
 */

#ifndef STATE_H_
#define STATE_H_

#include <cstdint>
#include <memory>

#include "engine/board/bitboard.h"
#include "engine/core/types.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    /**
     * @class State
     */
    class State
    {
      public:
        /**
         * @brief Constructor.
         */
        State() noexcept;

        /**
         * @brief Constructor from FEN notation.
         */
        State(const std::string_view fenNotation) noexcept;

        /**
         * @brief Destructor.
         */
        ~State() = default;

        /**
         * @brief Get a square's index, based on the rank's and file's indexes.
         *
         * @param [in] rankInex  : rank's index
         * @param [in] fileIndex : file's index
         * @return int : the square's index
         */
        static inline constexpr int getSquareIndex(const int rankIndex, const int fileIndex) noexcept
        {
            return (8 * rankIndex) + fileIndex;
        };

        /**
         * @brief Get a rank's index, bases on a square's index
         *
         * @param [in] squareIndex : square's index
         * @return int : the rank's index
         */
        static inline constexpr int getRankIndex(const int squareIndex) noexcept
        {
            return squareIndex >> 3;
        };

        /**
         * @brief Get a files's index, bases on a square's index.
         *
         * @param [in] squareIndex : square's index
         * @return int : the file's index
         */
        static inline constexpr int getFileIndex(const int squareIndex) noexcept
        {
            return squareIndex & 7;
        };

        /**
         * @brief Check if a type of castling is enabled by checking its corresponding bit.
         *
         * @return bool : Wether this type of castling is enabled.
         */
        template <core::Castlings Type>
        inline constexpr bool hasCastlingRight() const noexcept
        {
            return ((this->castlingRights >> static_cast<unsigned>(Type)) & 1U) != 0;
        }

        /**
         * @brief Enable a type of castling.
         */
        template <core::Castlings Type>
        inline constexpr void setCastlingRight() noexcept
        {
            this->castlingRights |= (uint8_t{1} << static_cast<unsigned>(Type));
        }

        /**
         * @brief Disable a type of castling.
         */
        template <core::Castlings Type>
        inline constexpr void clearCastlingRight() noexcept
        {
            this->castlingRights &= static_cast<uint8_t>(~(uint8_t{1} << static_cast<unsigned>(Type)));
        }

        /**
         * @brief Check if we are losing any castling right by :
         * - Moving a rook / king from it's initial position
         * - Capturing an enemy rook (making him lose 1 castling right)
         *
         * @param [in] color      : Performing the move
         * @param [in] piece      : Piece being moved
         * @param [in] fromSquare : Initial square
         * @param [in] toSquare   : Destination square
         */
        void checkCastlingRemoval(const core::Pieces piece, const int fromSquare, const int toSquare) noexcept;

        /**
         * @brief Get a piece on a given square.
         *
         * @param [in] squareIndex : the square to check
         * @return Pieces : the found Piece
         */
        core::Pieces getPiece(const int squareIndex) const noexcept;

        /**
         * @brief Get a piece from a given team and square.
         *
         * @param [in] color       : the team to check
         * @param [in] squareIndex : the square to check
         * @return Pieces : the found Piece
         */
        core::Pieces getPiece(const core::Colors color, const int squareIndex) const noexcept;

        /**
         * @brief Add a piece to a given team on a given square.
         *
         * @param [in] color       : Team to add the piece to
         * @param [in] piece       : Piece to add
         * @param [in] squareIndex : Square to add the piece on
         */
        void setPiece(const core::Colors color, const core::Pieces piece, const int squareIndex) noexcept;

        /**
         * @brief Remove a piece from a given team and square.
         *
         * @param [in] color       : Team to remove the piece from
         * @param [in] piece       : Piece to remove
         * @param [in] squareIndex : Square to remove the piece from
         */
        void unsetPiece(const core::Colors color, const core::Pieces piece, const int squareIndex) noexcept;

        /**
         * @brief Move a given piece.
         *
         * @param [in] color      : Team to move the piece from
         * @param [in] piece      : Piece to move
         * @param [in] fromSquare : Square to move the piece from
         * @param [in] toSquare   : Square to move the piece to
         */
        void movePiece(const core::Colors color, const core::Pieces piece, const int fromSquare,
                       const int toSquare) noexcept;

        uint16_t halfMoveClock = 0;                    // Half move counter
        uint16_t fullMoveClock = 1;                    // Full move counter
        core::Colors sideToMove = core::Colors::WHITE; // Whose turn is it ? :)
        core::CastlingRights castlingRights;           // Informations about enabled castlings.

        int enPassantSquare = -1; // When En Passant is enabled, this var is set

        core::PiecesBitboards allPieces;             // Occupancy for each team and each piece
        Bitboard generalOccupancy;                   // Occupancy for all pieces
        core::ColoredOccupancies coloredOccupancies; // Team specific occupancies
    };

} // namespace engine::board

#endif // STATE_H_