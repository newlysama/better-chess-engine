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
        : m_pinnedBB{}
        , m_allOccBB{}
        , m_teamsOccBB{}
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
            }
    {
        // Fills pieceAt with empty pieces
        for (auto& row : m_pieceAt)
            row.fill(Piece::UNKNOWN_PIECE);

        // Builds occupancy bitboards and fills m_pieceAt
        for (std::size_t piece = 0; piece < Piece::N_PIECES; piece++)
        {
            for (Color color : {WHITE, BLACK})
            {
                Bitboard bb = m_piecesBB[color][piece];
                m_teamsOccBB[color] |= bb;

                while (!bb.isEmpty())
                {
                    int square = bb.lsbIndex();
                    m_pieceAt[color][square] = static_cast<Piece>(piece);
                    bb.unset(square);
                }
            }
        }

        m_allOccBB = m_teamsOccBB[Color::WHITE] | m_teamsOccBB[Color::BLACK];
    }

    State::State(const std::array<std::string, 6> fenNotation)
    {
        std::expected<void, std::string> res{};

        // Fill m_pieceAt with UNKNOWN_PIECEs
        for (auto& row : m_pieceAt)
            row.fill(Piece::UNKNOWN_PIECE);

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

    std::string State::buildFenOccupancy() const noexcept
    {
        std::string fen;

        for (int rank = 7; rank >= 0; rank--)
        {
            int emptyCount = 0;

            for (int file = 0; file < 8; file++)
            {
                const int square = this->getSquareIndex(rank, file);

                if (Piece piece = m_pieceAt[Color::WHITE][square]; piece != Piece::UNKNOWN_PIECE)
                {
                    if (emptyCount > 0)
                    {
                        fen += char('0' + emptyCount);
                        emptyCount = 0;
                    }
                    fen += utils::pieceToFenChar(Color::WHITE, piece);
                }
                else if (Piece piece = m_pieceAt[Color::BLACK][square]; piece != Piece::UNKNOWN_PIECE)
                {
                    if (emptyCount > 0)
                    {
                        fen += char('0' + emptyCount);
                        emptyCount = 0;
                    }
                    fen += utils::pieceToFenChar(Color::BLACK, piece);
                }
                else
                {
                    emptyCount++;
                }
            }

            if (emptyCount > 0)
                fen += char('0' + emptyCount);
            if (rank > 0)
                fen += '/';
        }

        return fen;
    }

    Piece State::getPiece(int square) const noexcept
    {
        for (Color color : {Color::WHITE, Color::BLACK})
        {
            if (m_pieceAt[color][square] != Piece::UNKNOWN_PIECE)
            {
                return m_pieceAt[color][square];
            }
        }

        LOG_ERROR("Trying to access piece at square {}, but no piece was found", square);
        return Piece::UNKNOWN_PIECE;
    }

    void State::setPiece(const Color color, const Piece piece, const int square) noexcept
    {
        m_piecesBB[color][piece].set(square);
        m_allOccBB.set(square);
        m_teamsOccBB[color].set(square);
        m_pieceAt[color][square] = piece;
    }

    void State::unsetPiece(const Color color, const Piece piece, const int square) noexcept
    {
        m_piecesBB[color][piece].unset(square);
        m_allOccBB.unset(square);
        m_teamsOccBB[color].unset(square);
        m_pieceAt[color][square] = Piece::UNKNOWN_PIECE;
    }

    void State::movePiece(const Piece piece, const int fromSquare, const int toSquare) noexcept
    {
        if (piece == Piece::KING)
        {
            // Keep track of the kings indexes
            m_kgSquares[m_sideToMove] = toSquare;

            // Update castling rights
            if (m_sideToMove == Color::WHITE)
                m_castlingRights &= ~((1U << Castling::WHITE_KING_SIDE) | (1U << Castling::WHITE_QUEEN_SIDE));
            else
                m_castlingRights &= ~((1U << Castling::BLACK_KING_SIDE) | (1U << Castling::BLACK_QUEEN_SIDE));
        }
        else if (piece == Piece::ROOK && m_rookCastlingMap.find(fromSquare) != m_rookCastlingMap.end())
        {
            this->clearCastlingRight(m_rookCastlingMap[fromSquare]);
        }

        this->unsetPiece(m_sideToMove, piece, fromSquare);
        this->setPiece(m_sideToMove, piece, toSquare);

        LOG_INFO("Moved {} {} from {} to {}", utils::toString(m_sideToMove), utils::toString(piece),
                 utils::squareIndexToString(fromSquare), utils::squareIndexToString(toSquare));
    }

    void State::computePinnedPieces() noexcept
    {
        const Color enemyColor = this->getEnemyColor();

        // Clear the pinned pieces bitboards
        m_pinnedBB[m_sideToMove].fill(Bitboard{0ULL});
        m_pinnedBB[enemyColor].fill(Bitboard{0ULL});

        for (int direction = 0; direction < Direction::N_DIRECTIONS; direction++)
        {
            // Get every piece on the ray, skip if ray is empty
            Bitboard ray = RAY_MASKS[m_kgSquares[m_sideToMove]][direction] & m_allOccBB;
            if (ray.isEmpty())
            {
                continue;
            }

            // Check if the direction makes at least 1 of the indexes increase or not
            bool isDirIncr = direction == Direction::NORTH || direction == Direction::EAST ||
                             direction == Direction::NORTH_EAST || direction == Direction::NORTH_WEST;

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

            bool isDirOrtho = direction == Direction::NORTH || direction == Direction::SOUTH ||
                              direction == Direction::EAST || direction == Direction::WEST;

            // Check if enemy piece is a sliding piece that can move
            // on the current ray given the current direction
            Piece enemyPiece = m_pieceAt[enemyColor][enemySquare];
            if (enemyPiece != Piece::QUEEN)
            {
                if (isDirOrtho && enemyPiece != Piece::ROOK)
                {
                    continue;
                }

                if (!isDirOrtho && enemyPiece != Piece::BISHOP)
                {
                    continue;
                }
            }

            m_pinnedBB[m_sideToMove][firstSquare] =
                BETWEEN_MASKS[m_kgSquares[m_sideToMove]][enemySquare] | Bitboard{1ULL << enemySquare};
        }
    }

    void State::computeEnemyTargetedSquares() noexcept
    {
        const Color enemyColor = this->getEnemyColor();

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
        Bitboard enemyPawns = m_piecesBB[enemyColor][Piece::PAWN];
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
        Bitboard enemyKnights = m_piecesBB[enemyColor][Piece::KNIGHT];
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
        Bitboard rooks = m_piecesBB[enemyColor][Piece::ROOK] | m_piecesBB[enemyColor][Piece::QUEEN];
        while (rooks.isEmpty() == false)
        {
            const int square = rooks.lsbIndex();
            Bitboard relevantOcc = occWithoutKing & ROOK_RELEVANT_MASKS[square];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.m_data * rookMagics[square].m_data) >> rookShifts[square];

            rooks.unset(square);
            m_targetsBB |= ROOK_ATTACKS_TABLE[square][magicIndex];

            // Check if our king is targeted
            if (ROOK_ATTACKS_TABLE[square][magicIndex].isSet(m_kgSquares[m_sideToMove]))
            {
                addChecker(square, true);
            }
        }

        // --- Bishop ---
        Bitboard bishops = m_piecesBB[enemyColor][Piece::BISHOP] | m_piecesBB[enemyColor][Piece::QUEEN];
        while (bishops.isEmpty() == false)
        {
            const int square = bishops.lsbIndex();
            Bitboard relevantOcc = occWithoutKing & BISHOP_RELEVANT_MASKS[square];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.m_data * bishopMagics[square].m_data) >> bishopShifts[square];

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
    }

} // namespace engine::board