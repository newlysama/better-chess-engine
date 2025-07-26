/**
 * @file      move_list.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Implements MoveList class.
 */

#include "move_list.h"

#include <assert.h>
#include <cmath>

#include "engine/board/magic_const.h"
#include "engine/board/mask.h"
#include "logging/logging.h"

#if !defined(BUILD_RELEASE) && !defined(BENCHMARK)
#include "utils/enums_to_string.h"
#include "utils/utils.h"
#endif

namespace engine::game
{
    using namespace engine::core;
    using namespace engine::board;

    MoveList::MoveList() noexcept
        : _moves{}
    {
    }

    void MoveList::add(const Move& move) noexcept
    {
        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BENCHMARK)
            assert(this->_size < 256);
        #endif
        // clang-format on

        this->_moves[this->_size++] = move;
    }

    void MoveList::clear() noexcept
    {
        this->_size = 0;
    }

    std::size_t MoveList::size() const noexcept
    {
        return this->_size;
    }

    Move& MoveList::operator[](std::size_t index) noexcept
    {
        return this->_moves[index];
    }

    bool MoveList::contains(const Move& move) noexcept
    {
        for (int i = 0; i < this->_size; i++)
        {
            if (move == this->_moves[i])
            {
                return true;
            }
        }

        return false;
    }

    void MoveList::processTargets(const State& state, Bitboard& targets, const int fromSquare, const MoveType moveType,
                                  const Piece fromPiece) noexcept
    {
        // Filter pinned pieces moves
        Bitboard pinnedTargets = state.m_pinnedBB[state.m_sideToMove][fromSquare];
        if (pinnedTargets.isEmpty() == false && fromPiece != Piece::KING)
        {
            targets &= pinnedTargets;
        }

        // If the king is in simple check, we can only
        // capture checking pieces, and only move to
        // square that block checking sliding pieces (if any)
        if (state.m_isChecked)
        {
            if (moveType == MoveType::CAPTURE)
            {
                targets &= state.m_checkersBB;
            }
            else if (fromPiece != Piece::KING)
            {
                targets &= state.m_blockersBB;
            }
        }

        // clang-format off
        #if !defined (BENCHMARK) && !defined(BUILD_RELEASE)
            if (targets.popCount() > 0)
            {
                LOG_DEBUG("Generated {} {} {} legal moves",
                            targets.popCount(),
                            utils::toString(fromPiece),
                            utils::toString(moveType)
                        );
            }
        #endif
        // clang-format on

        while (targets.m_data)
        {
            int toSquare = targets.lsbIndex();

            Move move{fromSquare, toSquare, moveType, fromPiece};
            this->add(std::move(move));

            // Move to the next target
            targets.unset(toSquare);
        }
    }

    template <Castling Castling>
    void MoveList::getCastlingMoves(const State& state, int fromSquare) noexcept
    {
        // Can't castle
        if (!state.hasCastlingRight<Castling>() || state.m_isChecked)
        {
            return;
        }

        // Check that square between rook and king are free
        if ((state.m_allOccBB & CASTLING_BETWEEN_MASKS[Castling]).m_data != 0ULL)
            return;

        // Check that no square on the king's path is under attack
        if ((state.m_targetsBB & CASTLING_KING_PATH_MASKS[Castling]).m_data != 0ULL)
            return;

        Move castle{fromSquare, CASTLING_TO_SQUARE[Castling], MoveType::CASTLE, Piece::KING, Castling};
        this->add(std::move(castle));

        LOG_DEBUG("Generated {} legal move", utils::toString(Castling));
    }

    void MoveList::getEnPassantMoves(const State& state, int fromSquare) noexcept
    {
        int rankFrom = State::getRankIndex(fromSquare);

        if (!((state.m_sideToMove == Color::WHITE && rankFrom == 4) ||
              (state.m_sideToMove == Color::BLACK && rankFrom == 3)))
        {
            return;
        }

        int fileFrom = State::getFileIndex(fromSquare);
        int fileEnPassant = State::getFileIndex(state.m_epSquare);
        if (std::abs(fileFrom - fileEnPassant) != 1)
        {
            return;
        }

        // Check that enPassant doesn't leave our king in check
        int capturedSquare = state.m_sideToMove == Color::WHITE ? state.m_epSquare - 8 : state.m_epSquare + 8;
        if (State::getRankIndex(state.m_kgSquares[state.m_sideToMove]) == State::getRankIndex(capturedSquare))
        {
            Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

            // Get enemy Rooks and Queens that are on the same rank than our king
            Bitboard attackers = state.m_piecesBB[enemyColor][Piece::ROOK] |
                                 state.m_piecesBB[enemyColor][Piece::QUEEN] &
                                     RANKS_MASKS[State::getRankIndex(state.m_kgSquares[state.m_sideToMove])];

            while (attackers.isEmpty() == false)
            {
                int attackSquare = attackers.lsbIndex();
                attackers.unset(attackSquare);

                // Get blockers between the king and an attacker (excluding the captured enemy pawn)
                Bitboard blockers = BETWEEN_MASKS[state.m_kgSquares[state.m_sideToMove]][attackSquare] &
                                    state.m_allOccBB & ~Bitboard{1ULL << capturedSquare};

                // Captured enemy pawn is the only blocker preventing
                // our king from check, En Passant is illegal
                if (blockers.isEmpty())
                {
                    return;
                }
            }
        }

        Move enPassant{fromSquare, state.m_epSquare, MoveType::EN_PASSANT, Piece::PAWN};
        this->add(std::move(enPassant));

        LOG_DEBUG("Generated {} legal move from {} to {}", utils::toString(enPassant.getMoveType()),
                  utils::squareIndexToString(fromSquare), utils::squareIndexToString(state.m_epSquare));
    }

    /**
     * @todo Generate EnPassant and Promotions
     */
    void MoveList::generatePawnsMoves(const State& state) noexcept
    {
        Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        Bitboard pawns = state.m_piecesBB[state.m_sideToMove][Piece::PAWN];

        // While there is bits set to 1
        while (pawns.m_data)
        {
            // Extract bit index to the next '1'
            int fromSquare = pawns.lsbIndex();

            // Move to the next pawn
            pawns.unset(fromSquare);

            // Rechable empty squares
            Bitboard pushTargets = PAWN_PUSHES_MASKS[state.m_sideToMove][fromSquare] & ~state.m_allOccBB;

            // Rechable capture square
            Bitboard captureTargets =
                PAWN_CAPTURES_MASKS[state.m_sideToMove][fromSquare] & state.m_teamsOccBB[enemyColor];

            this->processTargets(state, captureTargets, fromSquare, MoveType::CAPTURE, Piece::PAWN);
            this->processTargets(state, pushTargets, fromSquare, MoveType::QUIET, Piece::PAWN);

            // Generate double pushes if the square between start and dest is empty
            if ((PAWN_PUSHES_MASKS[state.m_sideToMove][fromSquare] & state.m_allOccBB) == 0)
            {
                Bitboard doublePushTargets =
                    PAWN_DOUBLE_PUSHES_MASKS[state.m_sideToMove][fromSquare] & ~state.m_allOccBB;

                this->processTargets(state, doublePushTargets, fromSquare, MoveType::DOUBLE_PUSH, Piece::PAWN);
            }

            // Generate enPassant if enabled
            if (state.m_epSquare != -1) [[unlikely]]
            {
                this->getEnPassantMoves(state, fromSquare);
            }
        }
    }

    void MoveList::generateKnightsMoves(const State& state) noexcept
    {
        Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        Bitboard knights = state.m_piecesBB[state.m_sideToMove][Piece::KNIGHT];

        // While there is bits set to 1
        while (knights.m_data)
        {
            // Extract bit index to the next '1'
            int fromSquare = knights.lsbIndex();

            // Move to the next knight
            knights.unset(fromSquare);

            // Get the targets
            Bitboard targets = KNIGHT_ATTACKS_MASKS[fromSquare] & ~state.m_teamsOccBB[state.m_sideToMove];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.m_teamsOccBB[enemyColor];

            // Rechable squares occupied by an enemy piece
            Bitboard captureTargets = targets & state.m_teamsOccBB[enemyColor];

            this->processTargets(state, captureTargets, fromSquare, MoveType::CAPTURE, Piece::KNIGHT);
            this->processTargets(state, quietTargets, fromSquare, MoveType::QUIET, Piece::KNIGHT);
        }
    }

    void MoveList::generateRooksMoves(const State& state) noexcept
    {
        Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        Bitboard rooks = state.m_piecesBB[state.m_sideToMove][Piece::ROOK];

        // While there is bits set to 1
        while (rooks.m_data)
        {
            // Extract bit index to the next '1'
            int fromSquare = rooks.lsbIndex();

            // Move to the next rook
            rooks.unset(fromSquare);

            Bitboard relevantOcc = state.m_allOccBB & ROOK_RELEVANT_MASKS[fromSquare];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.m_data * rookMagics[fromSquare].m_data) >> rookShifts[fromSquare];

            // Get the targets
            Bitboard targets = ROOK_ATTACKS_TABLE[fromSquare][magicIndex] & ~state.m_teamsOccBB[state.m_sideToMove];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.m_teamsOccBB[enemyColor];

            // Rechable squares occupied by an enemy piece
            Bitboard captureTargets = targets & state.m_teamsOccBB[enemyColor];

            this->processTargets(state, captureTargets, fromSquare, MoveType::CAPTURE, Piece::ROOK);
            this->processTargets(state, quietTargets, fromSquare, MoveType::QUIET, Piece::ROOK);
        }
    }

    void MoveList::generateBishopsMoves(const State& state) noexcept
    {
        Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        Bitboard bishops = state.m_piecesBB[state.m_sideToMove][Piece::BISHOP];

        // While there is bits set to 1
        while (bishops.m_data)
        {
            // Extract bit index to the next '1'
            int fromSquare = bishops.lsbIndex();

            // Move to the next bishop
            bishops.unset(fromSquare);

            Bitboard relevantOcc = state.m_allOccBB & BISHOP_RELEVANT_MASKS[fromSquare];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.m_data * bishopMagics[fromSquare].m_data) >> bishopShifts[fromSquare];

            // Get the targets
            Bitboard targets = BISHOP_ATTACKS_TABLE[fromSquare][magicIndex] & ~state.m_teamsOccBB[state.m_sideToMove];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.m_teamsOccBB[enemyColor];

            // Rechable squares occupied by an enemy piece
            Bitboard captureTargets = targets & state.m_teamsOccBB[enemyColor];

            this->processTargets(state, captureTargets, fromSquare, MoveType::CAPTURE, Piece::BISHOP);
            this->processTargets(state, quietTargets, fromSquare, MoveType::QUIET, Piece::BISHOP);
        }
    }

    void MoveList::generateQueenMoves(const State& state) noexcept
    {
        Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        Bitboard queen = state.m_piecesBB[state.m_sideToMove][Piece::QUEEN];

        // While there is bits set to 1
        // Remember that thank's to promotions, we can have several queens alive
        while (queen.m_data)
        {
            // Extract queen's index
            int fromSquare = queen.lsbIndex();

            // Remove queen's bit
            queen.unset(fromSquare);

            // Magic rook
            Bitboard rookRelevantOcc = state.m_allOccBB & ROOK_RELEVANT_MASKS[fromSquare];
            size_t rookMagicIndex = (rookRelevantOcc.m_data * rookMagics[fromSquare].m_data) >> rookShifts[fromSquare];

            // Magic bishop
            Bitboard bishopRelevantOcc = state.m_allOccBB & BISHOP_RELEVANT_MASKS[fromSquare];
            size_t bishopMagicIndex =
                (bishopRelevantOcc.m_data * bishopMagics[fromSquare].m_data) >> bishopShifts[fromSquare];

            // Combine rooks and bishops attack tables
            Bitboard attacks =
                ROOK_ATTACKS_TABLE[fromSquare][rookMagicIndex] | BISHOP_ATTACKS_TABLE[fromSquare][bishopMagicIndex];

            Bitboard targets = attacks & ~state.m_teamsOccBB[state.m_sideToMove];
            Bitboard quietTargets = targets & ~state.m_teamsOccBB[enemyColor];
            Bitboard captureTargets = targets & state.m_teamsOccBB[enemyColor];

            this->processTargets(state, captureTargets, fromSquare, MoveType::CAPTURE, Piece::QUEEN);
            this->processTargets(state, quietTargets, fromSquare, MoveType::QUIET, Piece::QUEEN);
        }
    }

    void MoveList::generateKingMoves(const State& state) noexcept
    {
        Color enemyColor = state.m_sideToMove == Color::WHITE ? Color::BLACK : Color::WHITE;

        Bitboard king = state.m_piecesBB[state.m_sideToMove][Piece::KING];

        // Extract king's index
        int fromSquare = king.lsbIndex();

        // Generate classic targets
        Bitboard targets =
            KING_ATTACKS_MASKS[fromSquare] & ~state.m_teamsOccBB[state.m_sideToMove] & ~state.m_targetsBB;
        Bitboard quietTargets = targets & ~state.m_teamsOccBB[enemyColor];
        Bitboard captureTargets = targets & state.m_teamsOccBB[enemyColor];

        // Generate Castling targets
        if (state.m_sideToMove == Color::WHITE)
        {
            getCastlingMoves<Castling::WHITE_KING_SIDE>(state, fromSquare);
            getCastlingMoves<Castling::WHITE_QUEEN_SIDE>(state, fromSquare);
        }
        else
        {
            getCastlingMoves<Castling::BLACK_KING_SIDE>(state, fromSquare);
            getCastlingMoves<Castling::BLACK_QUEEN_SIDE>(state, fromSquare);
        }

        this->processTargets(state, captureTargets, fromSquare, MoveType::CAPTURE, Piece::KING);
        this->processTargets(state, quietTargets, fromSquare, MoveType::QUIET, Piece::KING);
    }

    void MoveList::generateAllMoves(State& state) noexcept
    {
        // Clear the actual list
        this->clear();

        // Computes pinned pieces
        state.computePinnedPieces();

        // Computes enemy targeted squares
        state.computeEnemyTargetedSquares();

        LOG_DEBUG("Generating {} legal moves...", utils::toString(state.m_sideToMove));

        if (state.m_isDoubleChecked == false)
        {
            this->generatePawnsMoves(state);
            this->generateKnightsMoves(state);
            this->generateRooksMoves(state);
            this->generateBishopsMoves(state);
            this->generateQueenMoves(state);
        }

        this->generateKingMoves(state);

        LOG_DEBUG("{} {} legal moves generated", this->_size, utils::toString(state.m_sideToMove));
    }
} // namespace engine::game
