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

#include "engine/board/mask.h"
#include "logging/logging.h"
#include "utils/enums_to_string.h"

namespace engine::board
{
    using namespace engine::core;

    State::State() noexcept
        : m_kgSquares{3, 60}
        , m_pinnedBB{}
        , m_piecesBB{
            {// -- White pieces --
             {
                 Bitboard{0x0000'0000'0000'FF00ULL}, // pawns
                 Bitboard{0x0000'0000'0000'0042ULL}, // knights
                 Bitboard{0x0000'0000'0000'0024ULL}, // bishops
                 Bitboard{0x0000'0000'0000'0081ULL}, // rooks
                 Bitboard{0x0000'0000'0000'0008ULL}, // queen
                 Bitboard{0x0000'0000'0000'0010ULL}  // king
             }, // -- Black pieces --
             {
                 Bitboard{0x00FF'0000'0000'0000ULL}, // pawns
                 Bitboard{0x4200'0000'0000'0000ULL}, // knights
                 Bitboard{0x2400'0000'0000'0000ULL}, // bishops
                 Bitboard{0x8100'0000'0000'0000ULL}, // rooks
                 Bitboard{0x0800'0000'0000'0000ULL}, // queen
                 Bitboard{0x1000'0000'0000'0000ULL}  // king
             }}
            },
        m_allOccBB{}, m_teamsOccBB{}
    {
        // Builds occupancy bitboards
        for (std::size_t piece = 0; piece < Pieces::PIECES; piece++)
        {
            m_teamsOccBB[Colors::WHITE] |= m_piecesBB[Colors::WHITE][piece];
            m_teamsOccBB[Colors::BLACK] |= m_piecesBB[Colors::BLACK][piece];
        }

        m_allOccBB = m_teamsOccBB[Colors::WHITE] | m_teamsOccBB[Colors::BLACK];
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
        if (m_sideToMove == Colors::WHITE &&
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

        if (m_sideToMove == Colors::BLACK &&
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
        Colors enemyColor = (m_sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE);

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
                if (m_piecesBB[color][piece].isSet(squareIndex))
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
            if (m_piecesBB[color][piece].isSet(squareIndex))
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
        m_piecesBB[color][piece].set(squareIndex);
        m_allOccBB.set(squareIndex);
        m_teamsOccBB[color].set(squareIndex);

        LOG_DEBUG("Added {} {} to {}", utils::toString(color), utils::toString(piece),
                  utils::squareIndexToString(squareIndex));
    }

    void State::unsetPiece(const Colors color, const Pieces piece, const int squareIndex) noexcept
    {
        m_piecesBB[color][piece].unset(squareIndex);
        m_allOccBB.unset(squareIndex);
        m_teamsOccBB[color].unset(squareIndex);

        LOG_DEBUG("Removed {} {} from {}", utils::toString(color), utils::toString(piece),
                  utils::squareIndexToString(squareIndex));
    }

    void State::movePiece(const Pieces piece, const int fromSquare, const int toSquare) noexcept
    {
        // Keep track of the kings indices
        if (piece == Pieces::KING)
        {
            m_kgSquares[m_sideToMove] = toSquare;
        }

        this->unsetPiece(m_sideToMove, piece, fromSquare);
        this->setPiece(m_sideToMove, piece, toSquare);

        // If we move a rook or the king, check for castling rights removal
        this->checkCastlingRemoval(piece, fromSquare, toSquare);

        LOG_INFO("Moved {} {} from {} to {}", utils::toString(m_sideToMove), utils::toString(piece),
                 utils::squareIndexToString(fromSquare), utils::squareIndexToString(toSquare));
    }

    void State::computePinnedPieces() noexcept
    {
        // Clear the actuel pinned pieces bitboards
        m_pinnedBB[m_sideToMove].fill(Bitboard{0ULL});

        const Colors enemyColor = m_sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            int count = 0;
        #endif
        // clang-format on

        for (int direction = 0; direction < Directions::DIRECTIONS; direction++)
        {
            // Get every piece on the ray, skip if ray is empty
            Bitboard ray = RAY_MASKS[m_kgSquares[m_sideToMove]][direction] & m_allOccBB;
            if (ray.isEmpty())
            {
                continue;
            }

            // Check if the direction makes at least 1 of the indices increase or not
            bool isDirIncr = direction == Directions::NORTH || direction == Directions::EAST ||
                             direction == Directions::NORTH_EAST || direction == Directions::NORTH_WEST;

            // Check if the first piece on the ray is an ally, skip if not
            int firstSquare = isDirIncr ? ray.lsbIndex() : ray.msbIndex();
            if (m_teamsOccBB[m_sideToMove].isSet(firstSquare) == false)
            {
                continue;
            }

            // Get every piece behind the first one, skip if there are none
            Bitboard rayBehind = RAY_MASKS[firstSquare][direction] & m_allOccBB;
            if (rayBehind.isEmpty())
            {
                continue;
            }

            // Check if first piece behind ally piece from
            // first square is an enemy piece, skip if not
            int enemySquare = isDirIncr ? rayBehind.lsbIndex() : rayBehind.msbIndex();
            if (m_teamsOccBB[enemyColor].isSet(enemySquare) == false)
            {
                continue;
            }

            bool isDirOrtho = direction == Directions::NORTH || direction == Directions::SOUTH ||
                              direction == Directions::EAST || direction == Directions::WEST;

            // clang-format off
            // Check if enemy piece is a sliding piece that can move
            // on the current ray given the current direction
            Pieces enemyPiece = this->getPiece(enemyColor, enemySquare);
            if (enemyPiece != Pieces::QUEEN)
            {
                if (isDirOrtho && enemyPiece != Pieces::ROOK)
                {
                    continue;
                }

                if (!isDirOrtho && enemyPiece != Pieces::BISHOP)
                {
                    continue;
                }
            }
            // clang-format on

            m_pinnedBB[m_sideToMove][firstSquare] =
                BETWEEN_MASKS[m_kgSquares[m_sideToMove]][enemySquare] | Bitboard{1ULL << enemySquare};

            // clang-format off
            #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
                count++;
            #endif
            // clang-format on
        }

        LOG_DEBUG("Computed {} {} pinned pieces", count, utils::toString(m_sideToMove));
    }

    void State::computeEnemyTargetedSquares() noexcept
    {
        const Colors enemyColor = m_sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        // Restore m_checkersBB and m_blockersBB Bitboard
        m_checkersBB = Bitboard{0ULL};
        m_blockersBB = Bitboard{0ULL};

        // Work on an occupancy where our King is removed,
        // so that he doesn't block any target square
        Bitboard occWithoutKing = m_allOccBB;
        occWithoutKing.unset(m_kgSquares[m_sideToMove]);

        // Reset the current Bitboard
        m_targetsBB = Bitboard{0ULL};

        // Lambda that adds a checker and updates m_blockersBB if
        // attacker is a sliding piece
        auto addChecker = [&](const int attackerSquare, const bool isSlider) {
            m_checkersBB.set(attackerSquare);

            if (isSlider && m_checkersBB.popCount() == 1)
            {
                m_blockersBB = BETWEEN_MASKS[m_kgSquares[m_sideToMove]][attackerSquare];
            }
            else
            {
                m_blockersBB = Bitboard{0};
            }
        };

        // Now computes every square under enemy pieces attack

        // --- Pawns ---
        Bitboard enemyPawns = m_piecesBB[enemyColor][Pieces::PAWN];
        while (enemyPawns.isEmpty() == false)
        {
            const int square = enemyPawns.lsbIndex();

            enemyPawns.unset(square);
            m_targetsBB |= PAWN_CAPTURES_MASKS[enemyColor][square];

            // Check if our king is targeted
            if (PAWN_CAPTURES_MASKS[enemyColor][square].isSet(m_kgSquares[m_sideToMove]))
            {
                addChecker(square, false);
            }
        }

        // --- Knights ---
        Bitboard enemyKnights = m_piecesBB[enemyColor][Pieces::KNIGHT];
        while (enemyKnights.isEmpty() == false)
        {
            const int square = enemyKnights.lsbIndex();
            enemyKnights.unset(square);
            m_targetsBB |= KNIGHT_ATTACKS_MASKS[square];

            // Check if our king is targeted
            if (KNIGHT_ATTACKS_MASKS[square].isSet(m_kgSquares[m_sideToMove]))
            {
                addChecker(square, false);
            }
        }

        // --- Rook ---
        Bitboard rooks = m_piecesBB[enemyColor][Pieces::ROOK] | m_piecesBB[enemyColor][Pieces::QUEEN];
        while (rooks.isEmpty() == false)
        {
            const int square = rooks.lsbIndex();
            Bitboard relevantOcc = occWithoutKing & ROOK_RELEVANT_MASKS[square];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.getData() * rookMagics[square].getData()) >> rookShifts[square];

            rooks.unset(square);
            m_targetsBB |= ROOK_ATTACKS_TABLE[square][magicIndex];

            // Check if our king is targeted
            if (ROOK_ATTACKS_TABLE[square][magicIndex].isSet(m_kgSquares[m_sideToMove]))
            {
                addChecker(square, true);
            }
        }

        // --- Bishop ---
        Bitboard bishops = m_piecesBB[enemyColor][Pieces::BISHOP] | m_piecesBB[enemyColor][Pieces::QUEEN];
        while (bishops.isEmpty() == false)
        {
            const int square = bishops.lsbIndex();
            Bitboard relevantOcc = occWithoutKing & BISHOP_RELEVANT_MASKS[square];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.getData() * bishopMagics[square].getData()) >> bishopShifts[square];

            bishops.unset(square);
            m_targetsBB |= BISHOP_ATTACKS_TABLE[square][magicIndex];

            // Check if our king is targeted
            if (BISHOP_ATTACKS_TABLE[square][magicIndex].isSet(m_kgSquares[m_sideToMove]))
            {
                addChecker(square, true);
            }
        }

        const int enemyKingSq = m_kgSquares[enemyColor];
        m_targetsBB |= KING_ATTACKS_MASKS[enemyKingSq];

        // Check if our king is targeted
        if (KING_ATTACKS_MASKS[enemyKingSq].isSet(m_kgSquares[m_sideToMove]))
        {
            addChecker(enemyKingSq, false);
        }

        // Update check states
        uint8_t numCheckers = m_checkersBB.popCount();
        m_isChecked = (numCheckers >= 1);
        m_isDoubleChecked = (numCheckers >= 2);

        LOG_DEBUG("Computed {} targeted squares from {} team", m_targetsBB.popCount(), utils::toString(enemyColor));
    }

} // namespace engine::board