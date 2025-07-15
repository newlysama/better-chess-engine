/**
 * @file      state.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief State representation.
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <cstdint>
#include <memory>

#include "engine/board/bitboard.h"
#include "engine/board/mask.h"
#include "engine/core/types.h"
#include "engine/game/move.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace engine::core;
    using namespace engine::game;

    /**
     * @typedef UnmakeInfo
     * @brief Contains all the necessary infos to unmake a move.
     */
    typedef struct StateInfo
    {
        Move move;
        Pieces captured;
        StateInfo* previous;

        StateInfo() = default;
    } StateInfo;

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
        template <Castlings Type>
        inline constexpr bool hasCastlingRight() const noexcept
        {
            return ((this->castlingRights >> static_cast<unsigned>(Type)) & 1U) != 0;
        }

        /**
         * @brief Enable a type of castling.
         */
        template <Castlings Type>
        inline constexpr void setCastlingRight() noexcept
        {
            this->castlingRights |= (uint8_t{1} << static_cast<unsigned>(Type));
        }

        /**
         * @brief Disable a type of castling.
         */
        template <Castlings Type>
        inline constexpr void clearCastlingRight() noexcept
        {
            this->castlingRights &= static_cast<uint8_t>(~(uint8_t{1} << static_cast<unsigned>(Type)));
        }

        uint16_t halfMoveClock; // Half move counter
        uint16_t fullMoveClock; // Full move counter
        Colors sideToMove;      // Whose turn is it ? :)

        CastlingRights castlingRights; // Informations about enabled castlings.

        PiecesBitboards allPieces;             // Occupancy for each team and each piece
        Bitboard generalOccupancy;             // Occupancy for all pieces
        ColoredOccupancies coloredOccupancies; // Team specific occupancies

        StateInfo stateInfo; // Structure holding relevant infos to unmake a move
    };

} // namespace engine::board

#endif // BOARD_H_