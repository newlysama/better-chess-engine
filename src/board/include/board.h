/**
 * @file board.h
 * @author Thibault THOMAS
 *
 * @brief Board representation.
 */

#ifndef BOARD_H_
#define BOARD_H_

/*----- System--*/
#include <cstdint>
#include <memory>

/*----- Project Headers--*/
#include "conf/types.h"

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
        /**
         * @class Board
         * @brief Board represented by multiple Bitboards.
         */
        class Board
        {
            public:

                /**
                 * @brief Constructor.
                 */
                Board() noexcept;

                /**
                 * @brief Constructor from FEN notation.
                 */
                Board(const std::string fenNotation) noexcept;

                /**
                 * @brief Destructor.
                 */
                ~Board() = default;

                /**
                 * @brief Get a square's index, based on the rank's and file's indexes.
                 *
                 * @param [in] rankInex  : rank's index
                 * @param [in] fileIndex : file's index
                 * @return uint8_t : the square's index
                 */
                static inline constexpr uint8_t getSquareIndex(const uint8_t rankIndex, const uint8_t fileIndex) noexcept
                {
                    return (8 * rankIndex) + fileIndex;
                };

                /**
                 * @brief Get a rank's index, bases on a square's index
                 *
                 * @param [in] squareIndex : square's index
                 * @return uint8_t : the rank's index
                 */
                static inline constexpr uint8_t getRankIndex(const uint8_t squareIndex) noexcept
                {
                     return squareIndex >> 3;
                };

                /**
                 * @brief Get a files's index, bases on a square's index.
                 *
                 * @param [in] squareIndex : square's index
                 * @return uint8_t : the file's index
                 */
                static inline constexpr uint8_t getFileIndex(const uint8_t squareIndex) noexcept
                {
                    return squareIndex & 7;
                };


                // Bitboards for each piece from each team
                conf::types::PiecesBitboards allPieces;

                // Occupancy of the board (1 general + 1 for each team)
                board::Bitboard generalOccupancy;
                board::Bitboard whiteOccupancy;
                board::Bitboard blackOccupancy;
        };

        /**
         * @brief Files masks.
         */
        inline constexpr conf::types::FilesMasks filesMasks =
        {
            Bitboard{0x0101'0101'0101'0101ULL},  // file A
            Bitboard{0x0202'0202'0202'0202ULL},  // file B
            Bitboard{0x0404'0404'0404'0404ULL},  // file C
            Bitboard{0x0808'0808'0808'0808ULL},  // file D
            Bitboard{0x1010'1010'1010'1010ULL},  // file E
            Bitboard{0x2020'2020'2020'2020ULL},  // file F
            Bitboard{0x4040'4040'4040'4040ULL},  // file G
            Bitboard{0x8080'8080'8080'8080ULL}   // file H
        };

        /**
         * @brief Ranks masks.
         */
        inline constexpr conf::types::RanksMasks ranksMasks =
        {
            Bitboard{0x0000'0000'0000'00FFULL},  // rank 1
            Bitboard{0x0000'0000'0000'FF00ULL},  // rank 2
            Bitboard{0x0000'0000'00FF'0000ULL},  // rank 3
            Bitboard{0x0000'0000'FF00'0000ULL},  // rank 4
            Bitboard{0x0000'00FF'0000'0000ULL},  // rank 5
            Bitboard{0x0000'FF00'0000'0000ULL},  // rank 6
            Bitboard{0x00FF'0000'0000'0000ULL},  // rank 7
            Bitboard{0xFF00'0000'0000'0000ULL}   // rank 8
        };

        /**
         * @brief Build diagonals masks.
         */
        static constexpr conf::types::DiagonalMasks makeDiagonalMasks() noexcept
        {
            conf::types::DiagonalMasks mask{};  // initialized to 0ULL everywhere

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                uint8_t rankIndex = squareIndex / 8;
                uint8_t fileIndex = squareIndex % 8;
                uint8_t maskIndex = fileIndex - rankIndex + 7;

                mask[maskIndex] |= Bitboard(1ULL << squareIndex);
            }

            return mask;
        }

        /**
         * @brief Diagonals masks.
         */
        inline constexpr conf::types::DiagonalMasks diagonalMasks = makeDiagonalMasks();

        /**
         * @brief Builds anti-diagonals masks.
         */
        static constexpr conf::types::DiagonalMasks makeAntiDiagonalMasks() noexcept
        {
            conf::types::DiagonalMasks mask{};  // initialized to 0ULL everywhere

            for (uint8_t squareIndex = 0; squareIndex < 64; squareIndex++)
            {
                uint8_t rankIndex = squareIndex / 8;
                uint8_t fileIndex = squareIndex % 8;
                uint8_t maskIndex = fileIndex + rankIndex;

                mask[maskIndex] |= Bitboard(1ULL << squareIndex);
            }

            return mask;
        }

        /**
         * @brief Anti-diagonals masks.
         */
        inline constexpr conf::types::DiagonalMasks antiDiagonalMasks = makeAntiDiagonalMasks();

    } // namespace board
} // namespace engine

#endif // BOARD_H_