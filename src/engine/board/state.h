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
#include <ctype.h>
#include <expected>
#include <sstream>

#include "engine/board/bitboard.h"
#include "engine/core/const.h"
#include "engine/core/types.h"
#include "utils/utils.h"

namespace engine::board
{
    /**
     * @class State
     * @brief Holds the current game state.
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
        State(const std::array<std::string, 6> fenNotation);

        /**
         * @brief Destructor.
         */
        ~State() = default;

        /**
         * @brief Get a square's index, based on the rank's and file's indexes.
         *
         * @param [in] rankInex  : rank's index
         * @param [in] fileIndex : file's index
         *
         * @return the square's index
         */
        static inline constexpr int getSquareIndex(const int rankIndex, const int fileIndex) noexcept
        {
            return (8 * rankIndex) + fileIndex;
        };

        /**
         * @brief Get a rank's index, bases on a square's index
         *
         * @param [in] square : square's index
         *
         * @return the rank's index
         */
        static inline constexpr int getRankIndex(const int square) noexcept
        {
            return square >> 3;
        };

        /**
         * @brief Get a files's index, bases on a square's index.
         *
         * @param [in] square : square's index
         *
         * @return the file's index
         */
        static inline constexpr int getFileIndex(const int square) noexcept
        {
            return square & 7;
        };

        core::Color getEnemyColor() const noexcept
        {
            return m_sideToMove == core::Color::WHITE ? core::Color::BLACK : core::Color::WHITE;
        }

        /**
         * @brief Check if a type of castling is enabled by checking its corresponding bit.
         *
         * @return True if this type of castling is enabled, fasle otherwise
         */
        template <core::Castling Type>
        inline constexpr bool hasCastlingRight() const noexcept
        {
            return ((m_castlingRights >> static_cast<unsigned>(Type)) & 1U) != 0;
        }

        /**
         * @brief Enable a type of castling.
         */
        inline constexpr void setCastlingRight(core::Castling castle) noexcept
        {
            m_castlingRights |= (uint8_t{1} << static_cast<unsigned>(castle));
        }

        /**
         * @brief Disable a type of castling.
         */
        inline constexpr void clearCastlingRight(core::Castling castle) noexcept
        {
            m_castlingRights &= static_cast<uint8_t>(~(uint8_t{1} << static_cast<unsigned>(castle)));
        }

        /**
         * @brief Get a piece on a given square.
         *
         * @param [in] square : the square to check
         *
         * @return the found Piece
         */
        core::Piece getPiece(const int square) const noexcept;

        /**
         * @brief Add a piece to a given team on a given square.
         *
         * @param [in] color  : Team to add the piece to
         * @param [in] piece  : Piece to add
         * @param [in] square : Square to add the piece on
         */
        void setPiece(const core::Color color, const core::Piece piece, const int square) noexcept;

        /**
         * @brief Remove a piece from a given team and square.
         *
         * @param [in] color  : Team to remove the piece from
         * @param [in] piece  : Piece to remove
         * @param [in] square : Square to remove the piece from
         */
        void unsetPiece(const core::Color color, const core::Piece piece, const int square) noexcept;

        /**
         * @brief Move a given piece.
         *
         * @param [in] color      : Team to move the piece from
         * @param [in] piece      : Piece to move
         * @param [in] fromSquare : Square to move the piece from
         * @param [in] toSquare   : Square to move the piece to
         */
        void movePiece(const core::Piece piece, const int fromSquare, const int toSquare) noexcept;

        /**
         * @brief Computes all current side to move bitboards of
         * pinned pieces allowed destinations.
         */
        void computePinnedPieces() noexcept;

        /**
         * @brief Computes the squares targeted by enemy moves, and
         * updates the infos relative to king's check and double check states.
         */
        void computeEnemyTargetedSquares() noexcept;

        /*****************************************
         *               ATTRIBUTES              *
         *****************************************/

        uint16_t m_halfMoveClock = 0;                  // Half move counter
        uint16_t m_fullMoveClock = 1;                  // Full move counter
        core::Color m_sideToMove = core::Color::WHITE; // Whose turn is it ? :)

        // Informations about enabled castlings
        core::CastlingRights m_castlingRights = (1 << core::Castling::N_CASTLINGS) - 1;

        // clang-format off
        // Mapping of rooks starting square with their according castling right
        core::RookCastlingMap m_rookCastlingMap = {
            {0,  core::Castling::WHITE_QUEEN_SIDE}, // a1
            {7,  core::Castling::WHITE_KING_SIDE }, // h1
            {56, core::Castling::BLACK_QUEEN_SIDE}, // a8
            {63, core::Castling::BLACK_KING_SIDE }  // h8
        };
        // clang-format on

        int m_epSquare = -1; // When En Passant is enabled, this var is set

        core::KingSquares m_kgSquares = {3, 60}; // Square for each king
        bool m_isChecked = false;                // Check state for the current king
        bool m_isDoubleChecked = false;          // Double check state for the current king
        bool m_isCheckMate = false;              // GAME OVER BABY

        core::PinnedPieces m_pinnedBB;          // Bitboards of available destinations for each pinned piece
        Bitboard m_targetsBB = Bitboard{0ULL};  // Squares targeted by enemy moves
        Bitboard m_checkersBB = Bitboard{0ULL}; // Bitboard of squares that put the current king in check
        Bitboard m_blockersBB = Bitboard{0ULL}; // Bitboard of squares that block a king's check from sliding pieces

        Bitboard m_allOccBB;                   // General occupancy
        core::ColoredOccupancies m_teamsOccBB; // Team specific occupancies
        core::PiecesBitboards m_piecesBB;      // Occupancy for each team and each piece
        core::PieceAt m_pieceAt;               // Piece type on each square

      private:
        /*****************************************
         *          FEN PARSING METHODS          *
         *****************************************/

        /**
         * @brief Internal function that splits the occupancy
         * part of the fen notation into an array of 8 strings.
         *
         * @param [in] fen : occupancy part of the fen notation
         *
         * @return the built array or error message if fen not valid
         */
        inline std::expected<std::array<std::string, 8>, std::string> getFenOccupancies(const std::string& fen) noexcept
        {
            std::array<std::string, 8> parts;
            std::string token;
            std::istringstream iss(fen);

            for (int i = 7; i >= 0; i--)
            {
                if (!std::getline(iss, token, '/'))
                {
                    return std::unexpected("FEN's occupancy part error: less than 8 fields.");
                }

                parts[i] = token;
            }

            return parts;
        }

        /**
         * @brief Fills the occupancy bitboards from pre-computed substring of the FEN notation.
         *
         * @param [in] fen : occupancy part of the FEN notation
         * @return nothing / error message if FEN invalid
         */
        inline std::expected<void, std::string> setOccupanciesFromFen(const std::string& fen) noexcept
        {
            std::expected<std::array<std::string, 8>, std::string> parts = getFenOccupancies(fen);

            // Check that the substring is valid
            if (!parts.has_value())
            {
                return std::unexpected(parts.error());
            }

            for (std::size_t rank = 0; rank < 8; rank++)
            {
                std::size_t file = 0;
                for (std::size_t index = 0; index < parts.value()[rank].size(); index++)
                {
                    const unsigned char c = parts.value()[rank][index];
                    if (isdigit(c))
                    {
                        // If c is a digit, check that it is valid
                        if (c - '0' > 8)
                        {
                            return std::unexpected(
                                std::format("FEN's occupancy part error: digit is > 8 : {}", c - '0'));
                        }

                        // -1 because the for loop is gonna make +1
                        file += (c - '0');
                    }
                    else
                    {
                        // Check that we get a valid square
                        std::size_t square = getSquareIndex(rank, file);
                        if (square < 0 || square > 63)
                        {
                            return std::unexpected(std::format("FEN's occupancy part error: out of range "
                                                               "square: {} from file {} and rank {}",
                                                               square, file, rank));
                        }

                        std::pair<core::Color, core::Piece> pair = utils::fenCharToPiece(c);
                        if (pair.first == core::Color::UNKNOWN_COLOR && pair.second == core::Piece::UNKNOWN_PIECE)
                        {
                            return std::unexpected(std::format("FEN's occupancy part error: invalid piece {}",
                                                               parts.value()[rank][index]));
                        }

                        m_piecesBB[pair.first][pair.second].set(square);
                        m_teamsOccBB[pair.first].set(square);
                        m_allOccBB.set(square);
                        m_pieceAt[pair.first][square] = pair.second;

                        // Set the king's square
                        if (pair.second == core::Piece::KING)
                        {
                            m_kgSquares[pair.first] = square;
                        }

                        file++;
                    }
                }

                if (file != 8)
                {
                    return std::unexpected(std::format("FEN's occupancy part error: invalid number of files {}", file));
                }
            }

            return std::expected<void, std::string>{};
        }

        /**
         * @brief Set the m_sideToMove field.
         *
         * @param [in] fen : side to move part of the FEN notation
         *
         * @return nothing / error message if fen is invalid
         */
        inline std::expected<void, std::string> setSideToMoveFromFen(const std::string_view fen) noexcept
        {
            if (fen != "b" && fen != "w")
            {
                return std::unexpected(std::format("FEN's side to move part error: {}", fen));
            }

            m_sideToMove = fen == "b" ? core::Color::BLACK : core::Color::WHITE;
            return std::expected<void, std::string>{};
        }

        /**
         * @brief Fills the castlingRights field.
         *
         * @param [in] fen : castling rights part of the FEN notation
         *
         * @return nothing / error message if fen is invalid
         */
        inline std::expected<void, std::string> setCastlingRightsFromFen(const std::string_view fen) noexcept
        {
            // Start by clearing all castling rights
            this->clearCastlingRight(core::Castling::WHITE_KING_SIDE);
            this->clearCastlingRight(core::Castling::WHITE_QUEEN_SIDE);
            this->clearCastlingRight(core::Castling::BLACK_KING_SIDE);
            this->clearCastlingRight(core::Castling::BLACK_QUEEN_SIDE);

            if (fen == "-")
            {
                return std::expected<void, std::string>{};
            }

            // clang-format off
            for (const unsigned char c : fen)
            {
                std::pair<core::Color, core::Piece> pair = utils::fenCharToPiece(c);
                core::Castling castle = pair.first == core::Color::WHITE && pair.second == core::Piece::KING ? core::Castling::WHITE_KING_SIDE
                                       : pair.first == core::Color::WHITE && pair.second == core::Piece::QUEEN ? core::Castling::WHITE_QUEEN_SIDE
                                       : pair.first == core::Color::BLACK && pair.second == core::Piece::KING ? core::Castling::BLACK_KING_SIDE
                                       : pair.first == core::Color::BLACK && pair.second == core::Piece::QUEEN ? core::Castling::BLACK_QUEEN_SIDE
                                       : core::Castling::UNKNOWN_CASTLING;

                if (castle == core::Castling::UNKNOWN_CASTLING)
                {
                    return std::unexpected(std::format("FEN's castling rights error: invalid piece {}", fen));
                }

                this->setCastlingRight(castle);
            }
            // clang-format on

            return std::expected<void, std::string>{};
        }

        /**
         * @brief Fills the m_epSquare field.
         *
         * @param [in] fen : en passant square part of the FEN notation
         *
         * @return nothing / error message if fen is invalid
         */
        inline std::expected<void, std::string> setEnPassantSquareFromFen(const std::string_view fen) noexcept
        {
            if (fen == "-")
            {
                return std::expected<void, std::string>{};
            }

            if (core::SQUARE_INDEX.find(fen) == core::SQUARE_INDEX.end())
            {
                return std::unexpected(std::format("FEN's en passant square error: invalid square {}", fen));
            }

            m_epSquare = core::SQUARE_INDEX.at(fen);
            return std::expected<void, std::string>{};
        }

        /**
         * @brief Fills the m_halfMoveClock field.
         *
         * @param [in] fen : half move clock part of the FEN notation
         *
         * @return nothing / error message if fen is invalid
         */
        inline std::expected<void, std::string> setHalfMoveClockFromFen(const std::string_view fen) noexcept
        {
            int clock{};
            auto [endPtr, ec] = std::from_chars(fen.data(), fen.data() + fen.size(), clock);

            if (ec != std::errc() || *endPtr != '\0')
            {
                return std::unexpected(std::format("FEN's half move clock error: {}", fen));
            }

            m_halfMoveClock = clock;
            return std::expected<void, std::string>{};
        }

        /**
         * @brief Fills the m_fullMoveClock field.
         *
         * @param [in] fen : full move clock part of the FEN notation
         *
         * @return nothing / error message if fen is invalid
         */
        inline std::expected<void, std::string> setFullMoveClockFromFen(const std::string_view fen) noexcept
        {
            int clock{};
            auto [endPtr, ec] = std::from_chars(fen.data(), fen.data() + fen.size(), clock);

            if (ec != std::errc() || *endPtr != '\0')
            {
                return std::unexpected(std::format("FEN's half move clock error: {}", fen));
            }

            m_fullMoveClock = clock;
            return std::expected<void, std::string>{};
        }
    };

} // namespace engine::board

#endif // STATE_H_