/**
 * @file      game.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Game API
 */

#ifndef GAME_H_
#define GAME_H_

#include <deque>
#include <expected>

#include "engine/board/state.h"
#include "engine/game/move_list.h"
#include "logging/logging.h"

namespace engine::game
{
    struct UnmakeInfo
    {
        // State infos
        core::CastlingRights prevCastlingRights;
        int prevEpSquare;
        uint16_t prevHalfMoveClock;
        uint16_t prevFullMoveClock;

        // Capture / promotion infos
        core::Piece capPiece = core::Piece::UNKNOWN_PIECE;
        core::Color capColor = core::Color::UNKNOWN_COLOR;
        int capSquare = -1;

        // State bitboards
        core::PinnedPieces prevPinnedBB{};
        board::Bitboard prevTargetsBB = board::Bitboard{0ULL};
        board::Bitboard prevCheckersBB = board::Bitboard{0ULL};
        board::Bitboard prevBlockersBB = board::Bitboard{0ULL};
    };

    /**
     * @class Game
     * @brief Game API.
     * Holds the current state, the current legal moves list, and
     * handles the different move requests.
     */
    class Game
    {
      public:
        /**
         * @brief Constructor.
         */
        Game() noexcept;

        /**
         * @brief Constructor from FEN notation.
         *
         * @param [in] fenNotation : FEN string
         */
        Game(const std::string& fenNotation);

        /**
         * @brief Default destructor.
         */
        ~Game() noexcept = default;

        /**
         * @brief Make a move, and save to previous state if specified.
         *
         * @param [in] move  : The move to make
         */
        template <bool SAVE_STATE>
        void makeMove(const game::Move& move)
        {
            using namespace engine::core;

            LOG_DEBUG("Move request: [From square: {}] - [To square: {}] - [Move type: {}] - [From piece: {}]",
                      utils::squareIndexToString(move.getFromSquare()), utils::squareIndexToString(move.getToSquare()),
                      utils::toString(move.getMoveType()), utils::toString(move.getFromPiece()));

            // Save current state infos
            UnmakeInfo unmakeInfo;
            if constexpr (SAVE_STATE)
            {
                unmakeInfo.prevCastlingRights = m_state.m_castlingRights;
                unmakeInfo.prevEpSquare = m_state.m_epSquare;
                unmakeInfo.prevHalfMoveClock = m_state.m_halfMoveClock;
                unmakeInfo.prevFullMoveClock = m_state.m_fullMoveClock;

                unmakeInfo.prevPinnedBB = m_state.m_pinnedBB;
                unmakeInfo.prevTargetsBB = m_state.m_targetsBB;
                unmakeInfo.prevBlockersBB = m_state.m_blockersBB;
                unmakeInfo.prevCheckersBB = m_state.m_checkersBB;
            }

            Color enemyColor = m_state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;
            MoveType moveType = move.getMoveType();

            // If move is a capture, move the from piece and remove the target piece
            if (moveType == MoveType::CAPTURE)
            {
                int toSquare = move.getToSquare();
                Piece toRemove = m_state.getPiece(enemyColor, toSquare);

                if constexpr (SAVE_STATE)
                {
                    unmakeInfo.capColor = enemyColor;
                    unmakeInfo.capSquare = toSquare;
                    unmakeInfo.capPiece = toRemove;
                }

                this->makeCapture(move, enemyColor, toRemove);
            }
            // If move is a castling, move to according rook
            else if (moveType == MoveType::CASTLE) [[unlikely]]
            {
                this->makeCastling(move);
            }
            // If move is an En Passant, remove the target piece
            else if (moveType == MoveType::EN_PASSANT) [[unlikely]]
            {
                int capSquare = m_state.m_sideToMove == Color::WHITE ? move.getToSquare() - 8 : move.getToSquare() + 8;

                if constexpr (SAVE_STATE)
                {
                    unmakeInfo.capColor = enemyColor;
                    unmakeInfo.capSquare = capSquare;
                    unmakeInfo.capPiece = Piece::PAWN;
                }

                this->makeEnPassant(move, enemyColor, capSquare);
            }
            // If no special move, just move the piece
            else
            {
                m_state.movePiece(move.getFromPiece(), move.getFromSquare(), move.getToSquare());
            }

            // If promotion piece is set, make promotion
            if (move.isPromotion()) [[unlikely]]
            {
                this->makePromotion(move);
            }

            if constexpr (SAVE_STATE)
            {
                m_unmakeStack[m_stackSize++] = std::move(unmakeInfo);
            }

            this->update(move, enemyColor);
        }

        /**
         * @brief Unmake a move(used in move simulation).
         *
         * @param [in] move  : The move to unmake
         */
        void unmakeMove(const game::Move& move) noexcept;

        board::State m_state;                      // Game's state
        game::MoveList m_moveList;                 // Legal move lists of the current m_halfMoveClock
        std::array<UnmakeInfo, 256> m_unmakeStack; // History of previous states to undo move efficiently
        std::array<MoveList, 256> m_moveListStack; // History of the previous move lists
        std::size_t m_stackSize = 0;               // Current size of m_unmakeStack

      private:
        /**
         * @brief Make a capture.
         *
         * @param [in] move       : capture move
         * @param [in] enemyColor : enemy color
         * @param [in] enemyPiece : enemy piece
         */
        void makeCapture(const game::Move& move, const core::Color& enemyColor, const core::Piece& enemyPiece) noexcept;

        /**
         * @brief Unmake a capture.
         *
         * @param [in] move       : capture move
         * @param [in] unmakeInfo : state infos to restore
         */
        void unmakeCapture(const game::Move& move, const UnmakeInfo& unmakeInfo) noexcept;

        /**
         * @brief Special function make castling move.
         *
         * @param [in] move : the castling move
         */
        void makeCastling(const game::Move& move) noexcept;

        /**
         * @brief Unmake a castling.
         *
         * @param [in] move       : castling move
         * @param [in] unmakeInfo : state infos to restore
         */
        void unmakeCastling(const game::Move& move) noexcept;

        /**
         * @brief Special function to make En Passant move.
         *
         * @param [in] move       : En Passant move
         * @param [in] enemyColor : color of the captured piece
         * @param [in] capSquare  : square of the captured piece
         */
        void makeEnPassant(const game::Move& move, const core::Color& enemyColor, const int& capSquare) noexcept;

        /**
         * @brief Unmake an En Passant.
         *
         * @param [in] move       : En Passant move
         * @param [in] unmakeInfo : state infos to restore
         */
        void unmakeEnPassant(const game::Move& move, const UnmakeInfo& unmakeInfo) noexcept;

        /**
         * @brief Asks user for promotion option and perform it.
         *
         * @param [in] move : move leading to promotion
         */
        void makePromotion(const game::Move& move) noexcept;

        /**
         * @brief Unmake a promotion.
         *
         * @param [in] move       : promotion move
         * @param [in] unmakeInfo : state infos to restore
         */
        void unmakePromotion(const game::Move& move) noexcept;

        /**
         * @brief Update game state after a move was played.
         *
         * @param [in] move       : the played move
         * @param [in] enemyColor : next team to play
         */
        void update(const game::Move& move, const core::Color& enemyColor) noexcept;
    };

} // namespace engine::game

#endif // GAME_H_