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

#if !defined(BUILD_RELEASE) && !defined(BENCHMARK)
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
     *
     * We store a move on a 32 bits integers (22 used, 10 useless, I know, this is kinda crap)
     * ───────────────────────────────────────────────────────────────────────────────────────────
     *  0  - 5  : from square      (6 bits)
     *  6  - 11 : to square        (6 bits)
     *  12 - 14 : moving piece     (3 bits)  0 = Pawn, 1 = Knight, 2 = Rook, 3 = Bishop, 5 = King
     *  15 - 17 : move type        (3 bits)  0 = QUIET, 1 = CAPTURE, 2 = CASTLE, 3 = DOUBLE_PUSH, 4 = EN_PASSANT
     *  18 - 19 : castling type    (2 bits)  0 = WK, 1 = WQ, 2 = BK, 3 = BQ
     *  20      : promotion flag   (1 bit)   1 = move is a promotion
     *  21      : prom. piece bit  (1 bit)   0 = Knight, 1 = Queen (unless asked by user, Queen is set by default)
     *  22 - 31 : do whatever you want with those
     */
    class Move
    {
      public:
        /**
         * @brief Default constructor.
         */
        Move() noexcept
            : m_data{0}
        {
        }

        /**
         * @brief Basic move constructor.
         *
         * @param [in] from       : initial square
         * @param [in] to         : destination square
         * @param [in] type       : type of move
         * @param [in] fromPieces : moving piece
         */
        Move(int from, int to, core::MoveType type, core::Piece fromPiece) noexcept
            : m_data{0}
        {
            this->setFromSquare(from);
            this->setToSquare(to);
            this->setMoveType(type);
            this->setFromPiece(fromPiece);

            if (fromPiece == core::Piece::PAWN &&
                (board::State::getRankIndex(to) == 7 || board::State::getRankIndex(to) == 0)) [[unlikely]]
            {
                this->setPromotionPiece(core::Piece::QUEEN);
            }
        }

        /**
         * @brief Castling move constructor.
         *
         * @param [in] from       : initial square
         * @param [in] to         : destination square
         * @param [in] type       : type of move
         * @param [in] fromPieces : moving piece
         * @param [in] castle     : type of castling
         */
        Move(int from, int to, core::MoveType type, core::Piece fromPiece, core::Castling castle) noexcept
            : Move(from, to, type, fromPiece)
        {
            setCastlingType(castle);
        }

        /**
         * @brief Check if a move has value or not.
         *
         * @return True if set, false otherwise
         */
        inline bool isSet() noexcept
        {
            return m_data != 0;
        }

        /**
         * @brief Set a value to m_data attribute.
         *
         * @param [in] newData : new value to set
         */
        inline void setData(uint32_t&& newData) noexcept
        {
            m_data = std::move(newData);
        }

        /*----------------------------------------*
         *                GETTERS                 *
         *----------------------------------------*/

        int getFromSquare() const noexcept
        {
            return int(m_data & 0x3Fu);
        }

        int getToSquare() const noexcept
        {
            return int((m_data >> 6) & 0x3Fu);
        }

        core::Piece getFromPiece() const noexcept
        {
            return static_cast<core::Piece>((m_data >> 12) & 0x7);
        }

        core::MoveType getMoveType() const noexcept
        {
            return static_cast<core::MoveType>((m_data >> 15) & 0x7);
        }

        core::Castling getCastlingType() const noexcept
        {
            return static_cast<core::Castling>((m_data >> 18) & 0x3);
        }

        core::Piece getPromotionPiece() const noexcept
        {
            if (!isPromotion())
            {
                return core::Piece::UNKNOWN_PIECE;
            }

            return (m_data >> 21) & 0x1 ? core::Piece::QUEEN : core::Piece::KNIGHT;
        }

        /*----------------------------------------*
         *                SETTERS                 *
         *----------------------------------------*/

        void setFromSquare(int square) noexcept
        {
            m_data = (m_data & ~0x3Fu) | (uint32_t(square) & 0x3Fu);
        }

        void setToSquare(int square) noexcept
        {
            m_data = (m_data & ~(0x3Fu << 6)) | ((uint32_t(square) & 0x3Fu) << 6);
        }

        void setFromPiece(core::Piece piece) noexcept
        {
            m_data = (m_data & ~(0x7u << 12)) | (static_cast<uint32_t>(piece) << 12);
        }

        void setMoveType(core::MoveType type) noexcept
        {
            m_data = (m_data & ~(0x7u << 15)) | (static_cast<uint32_t>(type) << 15);
        }

        void setCastlingType(core::Castling castling) noexcept
        {
            m_data = (m_data & ~(0x3u << 18)) | (static_cast<uint32_t>(castling) << 18);
        }

        void setPromotionPiece(core::Piece piece) noexcept
        {
            m_data |= (1u << 20);
            if (piece == core::Piece::QUEEN)
            {
                m_data |= (1u << 21); // 1 = Queen
            }
            else
            {
                m_data &= ~(1u << 21); // 0 = Knight
            }
        }

        /*----------------------------------------*
         *              COMPARAISON               *
         *----------------------------------------*/

        bool operator==(const Move& other) const noexcept
        {
            return m_data == other.m_data;
        }

        bool operator!=(const Move& other) const noexcept
        {
            return m_data != other.m_data;
        }

        /**
         * @brief Check if this move is a castling
         * by checking if 1 castling flag is set.
         */
        bool isCastling() const noexcept
        {
            return (m_data & (0xFu << 12)) != 0;
        }

        /**
         * @brief Check if this move is a promotion.
         */
        bool isPromotion() const noexcept
        {
            return (m_data >> 20) & 0x1;
        }

        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BENCHMARK)
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
        uint32_t m_data;
    };
} // namespace engine::game

#endif // MOVE_H
