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

#if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
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
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
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

    void MoveList::processTargets(Bitboard& targets, int fromSquare, MoveTypes moveType, Pieces fromPiece) noexcept
    {
        // clang-format off
        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            uint8_t count = 0;
        #endif

        while (targets.getData())
        {
            int toSquare = targets.lsbIndex();

            Move move{fromSquare, toSquare, moveType, fromPiece};

            this->add(move);

            // Move to the next target
            targets.unset(toSquare);

            #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
                count++;
            #endif
        }

        #if !defined(BUILD_RELEASE) && !defined(BUILD_BENCHMARK)
            if (count > 0)
            {
                LOG_DEBUG("Generated {} {} {} legal moves", count, utils::toString(fromPiece), utils::toString(moveType));
            }
        #endif
        // clang-format on
    }

    template <Castlings Castling>
    void MoveList::getCastlingMoves(const State& state, int fromSquare) noexcept
    {
        // Can't castle
        if (!state.hasCastlingRight<Castling>())
        {
            return;
        }

        // Chose the right castling mask
        // between the rook and the king
        // (squares that have to be empty)
        Bitboard between = CASTLING_MASKS[Castling];
        int toSquare = 0;

        switch (Castling)
        {
        case WHITE_KING_SIDE:
            toSquare = 6;
            break;
        case WHITE_QUEEN_SIDE:
            toSquare = 2;
            break;
        case BLACK_KING_SIDE:
            toSquare = 62;
            break;
        case BLACK_QUEEN_SIDE:
            toSquare = 58;
            break;
        default:
            break;
        }

        // Check that square between rook and king are free
        if ((state.generalOccupancy & between).getData() != 0ULL)
            return;

        Move castle{fromSquare, toSquare, MoveTypes::CASTLE, Pieces::KING, Castling};
        this->add(castle);

        LOG_DEBUG("Generated {} legal move", utils::toString(Castling));
    }

    void MoveList::getEnPassantMoves(const State& state, int fromSquare) noexcept
    {
        int rankFrom = State::getRankIndex(fromSquare);

        if ((state.sideToMove == Colors::WHITE && rankFrom == 4) ||
            (state.sideToMove == Colors::BLACK && rankFrom == 3))
        {
            int fileFrom = State::getFileIndex(fromSquare);
            int fileEnPassant = State::getFileIndex(state.enPassantSquare);

            if (std::abs(fileFrom - fileEnPassant) == 1)
            {
                Move enPassant{fromSquare, state.enPassantSquare, MoveTypes::EN_PASSANT, Pieces::PAWN};
                this->add(enPassant);

                LOG_DEBUG("Generated {} legal move from {} to {}", utils::toString(enPassant.getMoveType()),
                          utils::squareIndexToString(fromSquare), utils::squareIndexToString(state.enPassantSquare));
            }
        }
    }

    /**
     * @todo Generate EnPassant and Promotions
     */
    void MoveList::generatePawnsMoves(const State& state) noexcept
    {
        Colors enemyColor = state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        Bitboard pawns = state.allPieces[state.sideToMove][Pieces::PAWN];

        // While there is bits set to 1
        while (pawns.getData())
        {
            // Extract bit index to the next '1'
            int fromSquare = pawns.lsbIndex();

            // Move to the next pawn
            pawns.unset(fromSquare);

            // Rechable empty squares
            Bitboard pushTargets = PAWN_PUSHES_MASKS[state.sideToMove][fromSquare] & ~state.generalOccupancy;

            // Rechable capture square
            Bitboard captureTargets =
                PAWN_CAPTURES_MASKS[state.sideToMove][fromSquare] & state.coloredOccupancies[enemyColor];

            this->processTargets(captureTargets, fromSquare, MoveTypes::CAPTURE, Pieces::PAWN);
            this->processTargets(pushTargets, fromSquare, MoveTypes::QUIET, Pieces::PAWN);

            // Generate double pushes if the square between start and dest is empty
            if ((PAWN_PUSHES_MASKS[state.sideToMove][fromSquare] & state.generalOccupancy) == 0)
            {
                Bitboard doublePushTargets =
                    PAWN_DOUBLE_PUSHES_MASKS[state.sideToMove][fromSquare] & ~state.generalOccupancy;

                this->processTargets(doublePushTargets, fromSquare, MoveTypes::DOUBLE_PUSH, Pieces::PAWN);
            }

            // Generate enPassant if enabled
            if (state.enPassantSquare != -1) [[unlikely]]
            {
                this->getEnPassantMoves(state, fromSquare);
            }
        }
    }

    void MoveList::generateKnightsMoves(const State& state) noexcept
    {
        Colors enemyColor = state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        Bitboard knights = state.allPieces[state.sideToMove][Pieces::KNIGHT];

        // While there is bits set to 1
        while (knights.getData())
        {
            // Extract bit index to the next '1'
            int fromSquare = knights.lsbIndex();

            // Move to the next knight
            knights.unset(fromSquare);

            // Get the targets
            Bitboard targets = KNIGHT_ATTACKS_MASKS[fromSquare] & ~state.coloredOccupancies[state.sideToMove];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.coloredOccupancies[enemyColor];

            // Rechable squares occupied by an enemy piece
            Bitboard captureTargets = targets & state.coloredOccupancies[enemyColor];

            this->processTargets(captureTargets, fromSquare, MoveTypes::CAPTURE, Pieces::KNIGHT);
            this->processTargets(quietTargets, fromSquare, MoveTypes::QUIET, Pieces::KNIGHT);
        }
    }

    void MoveList::generateRooksMoves(const State& state) noexcept
    {
        Colors enemyColor = state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        Bitboard rooks = state.allPieces[state.sideToMove][Pieces::ROOK];

        // While there is bits set to 1
        while (rooks.getData())
        {
            // Extract bit index to the next '1'
            int fromSquare = rooks.lsbIndex();

            // Move to the next rook
            rooks.unset(fromSquare);

            Bitboard relevantOcc = state.generalOccupancy & ROOK_RELEVANT_MASKS[fromSquare];

            // Index the attack table using magic bitboards
            size_t magicIndex = (relevantOcc.getData() * rookMagics[fromSquare].getData()) >> rookShifts[fromSquare];

            // Get the targets
            Bitboard targets = ROOK_ATTACKS_TABLE[fromSquare][magicIndex] & ~state.coloredOccupancies[state.sideToMove];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.coloredOccupancies[enemyColor];

            // Rechable squares occupied by an enemy piece
            Bitboard captureTargets = targets & state.coloredOccupancies[enemyColor];

            this->processTargets(captureTargets, fromSquare, MoveTypes::CAPTURE, Pieces::ROOK);
            this->processTargets(quietTargets, fromSquare, MoveTypes::QUIET, Pieces::ROOK);
        }
    }

    void MoveList::generateBishopsMoves(const State& state) noexcept
    {
        Colors enemyColor = state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        Bitboard bishops = state.allPieces[state.sideToMove][Pieces::BISHOP];

        // While there is bits set to 1
        while (bishops.getData())
        {
            // Extract bit index to the next '1'
            int fromSquare = bishops.lsbIndex();

            // Move to the next bishop
            bishops.unset(fromSquare);

            Bitboard relevantOcc = state.generalOccupancy & BISHOP_RELEVANT_MASKS[fromSquare];

            // Index the attack table using magic bitboards
            size_t magicIndex =
                (relevantOcc.getData() * bishopMagics[fromSquare].getData()) >> bishopShifts[fromSquare];

            // Get the targets
            Bitboard targets =
                BISHOP_ATTACKS_TABLE[fromSquare][magicIndex] & ~state.coloredOccupancies[state.sideToMove];

            // Rechable empty squares
            Bitboard quietTargets = targets & ~state.coloredOccupancies[enemyColor];

            // Rechable squares occupied by an enemy piece
            Bitboard captureTargets = targets & state.coloredOccupancies[enemyColor];

            this->processTargets(captureTargets, fromSquare, MoveTypes::CAPTURE, Pieces::BISHOP);
            this->processTargets(quietTargets, fromSquare, MoveTypes::QUIET, Pieces::BISHOP);
        }
    }

    void MoveList::generateQueenMoves(const State& state) noexcept
    {
        Colors enemyColor = state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        Bitboard queen = state.allPieces[state.sideToMove][Pieces::QUEEN];

        // While there is bits set to 1
        // Remember that thank's to promotions, we can have several queens alive
        while (queen.getData())
        {
            // Extract queen's index
            int fromSquare = queen.lsbIndex();

            // Remove queen's bit
            queen.unset(fromSquare);

            // Magic rook
            Bitboard rookRelevantOcc = state.generalOccupancy & ROOK_RELEVANT_MASKS[fromSquare];
            size_t rookMagicIndex =
                (rookRelevantOcc.getData() * rookMagics[fromSquare].getData()) >> rookShifts[fromSquare];

            // Magic bishop
            Bitboard bishopRelevantOcc = state.generalOccupancy & BISHOP_RELEVANT_MASKS[fromSquare];
            size_t bishopMagicIndex =
                (bishopRelevantOcc.getData() * bishopMagics[fromSquare].getData()) >> bishopShifts[fromSquare];

            // Combine rooks and bishops attack tables
            Bitboard attacks =
                ROOK_ATTACKS_TABLE[fromSquare][rookMagicIndex] | BISHOP_ATTACKS_TABLE[fromSquare][bishopMagicIndex];

            Bitboard targets = attacks & ~state.coloredOccupancies[state.sideToMove];
            Bitboard quietTargets = targets & ~state.coloredOccupancies[enemyColor];
            Bitboard captureTargets = targets & state.coloredOccupancies[enemyColor];

            this->processTargets(captureTargets, fromSquare, MoveTypes::CAPTURE, Pieces::QUEEN);
            this->processTargets(quietTargets, fromSquare, MoveTypes::QUIET, Pieces::QUEEN);
        }
    }

    void MoveList::generateKingMoves(const State& state) noexcept
    {
        Colors enemyColor = state.sideToMove == Colors::WHITE ? Colors::BLACK : Colors::WHITE;

        Bitboard king = state.allPieces[state.sideToMove][Pieces::KING];

        // Extract king's index
        int fromSquare = king.lsbIndex();

        // Generate classic targets
        Bitboard targets = KING_ATTACKS_MASKS[fromSquare] & ~state.coloredOccupancies[state.sideToMove];
        Bitboard quietTargets = targets & ~state.coloredOccupancies[enemyColor];
        Bitboard captureTargets = targets & state.coloredOccupancies[enemyColor];

        // Generate Castling targets
        if (state.sideToMove == Colors::WHITE)
        {
            getCastlingMoves<Castlings::WHITE_KING_SIDE>(state, fromSquare);
            getCastlingMoves<Castlings::WHITE_QUEEN_SIDE>(state, fromSquare);
        }
        else
        {
            getCastlingMoves<Castlings::BLACK_KING_SIDE>(state, fromSquare);
            getCastlingMoves<Castlings::BLACK_QUEEN_SIDE>(state, fromSquare);
        }

        this->processTargets(captureTargets, fromSquare, MoveTypes::CAPTURE, Pieces::KING);
        this->processTargets(quietTargets, fromSquare, MoveTypes::QUIET, Pieces::KING);
    }

    void MoveList::generateAllMoves(const State& state) noexcept
    {
        this->clear();

        LOG_DEBUG("Generating {} legal moves...", utils::toString(state.sideToMove));

        this->generatePawnsMoves(state);
        this->generateKnightsMoves(state);
        this->generateRooksMoves(state);
        this->generateBishopsMoves(state);
        this->generateQueenMoves(state);
        this->generateKingMoves(state);

        LOG_DEBUG("{} {} legal moves generated", this->_size, utils::toString(state.sideToMove));
    }
} // namespace engine::game
