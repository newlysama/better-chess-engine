/**
 * @file      move.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Move definition.
 */

#ifndef MOVE_H_
#define MOVE_H_

#include <bit>
#include <cstdint>

#if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
#include <print>

#include "logging/logging.h"
#include "utils/enums_to_string.h"
#endif

#include "engine/core/enums.h"

namespace engine::game
{
    /**
     * @class Move
     *
     * @details
     * We store a move on a 32 bits unigned int
     *
     * ==> Those bits are combined to store a single 5 bits integer.
     *      Bits 0 -> 5   : Initial square
     *      Bits 6 -> 11  : Destination square
     *
     * ==> Theese ones are juste flags, never combined (1 bit = 1 piece or 1 type)
     *      Bits 12 -> 15 : Types of castling (if any)
     *      Bits 16 -> 19 : Promotion piece (if any) (0: Knhight - 1: Bishop - 2: Rook - 3: Queen)
     *      Bits 20 -> 25 : Type of move
     *      Bits 26 -> 31 : Intial square piece
     */
    class Move
    {
      public:
        /**
         * @brief Default constructor.
         */
        Move() noexcept = default;

        /**
         * @brief Basic move constructor.
         */
        Move(int from, int to, core::MoveTypes type, core::Pieces fromPiece) noexcept
            : _data{0}
        {
            setFromSquare(from);
            setToSquare(to);
            setMoveType(type);
            setFromPiece(fromPiece);
        }

        /**
         * @brief Castling move constructor.
         */
        Move(int from, int to, core::MoveTypes type, core::Pieces fromPiece, core::Castlings castle) noexcept
            : Move(from, to, type, fromPiece)
        {
            setCastlingType(castle);
        }

        /*----------------------------------------*
         *                GETTERS                 *
         *----------------------------------------*/

        int getFromSquare() const noexcept
        {
            return int(_data & 0x3Fu);
        }

        int getToSquare() const noexcept
        {
            return int((_data >> 6) & 0x3Fu);
        }

        core::Castlings getCastlingType() const noexcept
        {
            // 4 bits of castlings, starting at bit 12
            unsigned index = extractFlag(12, 4);
            return index < static_cast<unsigned>(core::Castlings::CASTLINGS) ? static_cast<core::Castlings>(index)
                                                                             : core::Castlings::UNKNOWN_CASTLING;
        }

        core::Pieces getPromotionPiece() const noexcept
        {
            // 4 bits of promotion piece, starting at bit 16
            unsigned index = extractFlag(16, 4);

            // Since possible promotion pieces have values that does not match
            // the reserved bit indices in _data, we need to map them.
            switch (index)
            {
            case 0:
                return core::Pieces::KNIGHT;
            case 1:
                return core::Pieces::BISHOP;
            case 2:
                return core::Pieces::ROOK;
            case 3:
                return core::Pieces::QUEEN;
            default:
                return core::Pieces::UNKNOWN_PIECE;
            }
        }

        core::MoveTypes getMoveType() const noexcept
        {
            // 6 bits of move type, starting at bit 20
            unsigned index = extractFlag(20, 6);
            return index < static_cast<unsigned>(core::MoveTypes::MOVE_TYPES) ? static_cast<core::MoveTypes>(index)
                                                                              : core::MoveTypes::UNKNOWN_MOVE_TYPE;
        }

        core::Pieces getFromPiece() const noexcept
        {
            // 6 bits of from piece, starting at bit 26
            unsigned index = extractFlag(26, 6);
            return index < static_cast<unsigned>(core::Pieces::PIECES) ? static_cast<core::Pieces>(index)
                                                                       : core::Pieces::UNKNOWN_PIECE;
        }

        /*----------------------------------------*
         *                SETTERS                 *
         *----------------------------------------*/

        void setFromSquare(int square) noexcept
        {
            _data = (_data & ~0x3Fu) | (uint32_t(square) & 0x3Fu);
        }

        void setToSquare(int square) noexcept
        {
            _data = (_data & ~(0x3Fu << 6)) | ((uint32_t(square) & 0x3Fu) << 6);
        }

        void setCastlingType(core::Castlings castle) noexcept
        {
            _data &= ~(0xFu << 12); // clear existing bits if any
            _data |= (1u << (12 + static_cast<unsigned>(castle)));
        }

        void setPromotionPiece(core::Pieces piece) noexcept
        {
            // Since possible promotion pieces have values that does not match
            // the reserved bit indices in _data, we need to map them.
            unsigned p;
            switch (piece)
            {
            case core::Pieces::KNIGHT:
                p = 0;
                break;
            case core::Pieces::BISHOP:
                p = 1;
                break;
            case core::Pieces::ROOK:
                p = 2;
                break;
            case core::Pieces::QUEEN:
                p = 3;
                break;
            default:
                p = 4;
            }

            _data &= ~(0xFu << 16); // clear existing bits if any
            _data |= (1u << (16 + static_cast<unsigned>(p)));
        }

        void setMoveType(core::MoveTypes type) noexcept
        {
            _data &= ~(0x3Fu << 20); // clear existing bits if any
            _data |= (1u << (20 + static_cast<unsigned>(type)));
        }

        void setFromPiece(core::Pieces piece) noexcept
        {
            _data &= ~(0x3Fu << 26); // clear existing bits if any
            _data |= (1u << (26 + static_cast<unsigned>(piece)));
        }

        /*----------------------------------------*
         *              COMPARAISON               *
         *----------------------------------------*/

        bool operator==(const Move& other) const noexcept
        {
            return _data == other._data;
        }

        bool operator!=(const Move& other) const noexcept
        {
            return _data != other._data;
        }

        /**
         * @brief Check if this move is a castling
         * by checking if 1 castling flag is set.
         */
        bool isCastling() const noexcept
        {
            return (_data & (0xFu << 12)) != 0;
        }

        /**
         * @brief Check if this move is a promotion
         * by checking if 1 promotion piece flag is set.
         */
        bool isPromotion() const noexcept
        {
            int toSquare = this->getToSquare();
            return (this->getFromPiece() == core::Pieces::PAWN &&
                    ((toSquare <= 63 && toSquare >= 56) || (toSquare >= 0 && toSquare <= 7)));
        }

        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
        inline void print() const noexcept
        {
            std::println("From    : {}", getFromSquare());
            std::println("To      : {}", getToSquare());
            std::println("Type    : {}", utils::toString(getMoveType()));
            std::println("Piece   : {}", utils::toString(getFromPiece()));
            std::println("Promo   : {}", utils::toString(getPromotionPiece()));
            std::println("Castle  : {}", utils::toString(getCastlingType()));
        }

        inline void log() const noexcept
        {
            LOG_DEBUG("From    : {}", getFromSquare());
            LOG_DEBUG("To      : {}", getToSquare());
            LOG_DEBUG("Type    : {}", utils::toString(getMoveType()));
            LOG_DEBUG("Piece   : {}", utils::toString(getFromPiece()));
            LOG_DEBUG("Promo   : {}", utils::toString(getPromotionPiece()));
            LOG_DEBUG("Castle  : {}", utils::toString(getCastlingType()));
        }
        #endif
        // clang-format on

      private:
        /**
         * @brief Extract the index of the bit set to 1 in a specific range.
         *
         * @param [in] shift : start index of the relevant bits
         * @param [in] width : number of bits we want to look at
         * @return unsigned : the bit index
         */
        inline unsigned extractFlag(int shift, int width) const noexcept
        {
            // Create mask where each bit within [shift; shfit + width] is set to 1
            uint32_t mask = ((1u << width) - 1u) << shift;

            // Apply the mask on _data to reveal the one bit set to 1
            uint32_t revealed = _data & mask;

            if (revealed == 0) // out-of-range
            {
                return width;
            }

            return unsigned(std::countr_zero(revealed)) - unsigned(shift);
        }

        uint32_t _data;
    };
} // namespace engine::game

#endif // MOVE_H
