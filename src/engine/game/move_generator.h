/**
 * @file      move_generator.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Handles the moves generation.
 */

#ifndef MOVE_GENERATOR_H_
#define MOVE_GENERATOR_H_

#include <cstdint>

#include "conf/types.h"
#include "engine/board/board.h"

/**
 * @namespace engine::move
 */
namespace engine::move
{
    /**
     * @brief Make a move.
     *
     * @param [in] board : The board to make the move on
     * @param [in] move  : The move to make
     */
    static inline void makeMove(board::Board &board, const conf::types::Move move) noexcept;

    /**
     * @brief Unmake a move.
     *
     * @param [in] board : The board to unmake the move on
     * @param [in] move  : The move to unmake
     */
    static inline void unmakeMove(board::Board &board, const conf::types::Move move) noexcept;

    /**
     * @brief Generates pseudo-legal Pawns moves.
     *
     * @param [in]  board      : The board to work with
     * @param [out] legalMoves : The vector of legal moves to fill
     */
    static inline void generatePawnsMoves(const board::Board &board, conf::types::LegalMoves &legalMoves) noexcept;

    /**
     * @brief Generates pseudo-legal Knights moves.
     *
     * @param [in]  board      : The board to work with
     * @param [out] legalMoves : The vector of legal moves to fill
     */
    static inline void generateKnightsMoves(const board::Board &board, conf::types::LegalMoves &legalMoves) noexcept;

    /**
     * @brief Generates pseudo-legal Bishops moves.
     *
     * @param [in]  board      : The board to work with
     * @param [out] legalMoves : The vector of legal moves to fill
     */
    static inline void generateBishopsMoves(const board::Board &board, conf::types::LegalMoves &legalMoves) noexcept;

    /**
     * @brief Generates pseudo-legal Rooks moves.
     *
     * @param [in]  board      : The board to work with
     * @param [out] legalMoves : The vector of legal moves to fill
     */
    static inline void generateRooksMoves(const board::Board &board, conf::types::LegalMoves &legalMoves) noexcept;

    /**
     * @brief Generates pseudo-legal Queen moves.
     *
     * @param [in]  board      : The board to work with
     * @param [out] legalMoves : The vector of legal moves to fill
     */
    static inline void generateQueenMoves(const board::Board &board, conf::types::LegalMoves &legalMoves) noexcept;

    /**
     * @brief Generates pseudo-legal King moves.
     *
     * @param [in]  board      : The board to work with
     * @param [out] legalMoves : The vector of legal moves to fill
     */
    static inline void generateKingMoves(const board::Board &board, conf::types::LegalMoves &legalMoves) noexcept;
} // namespace engine::move

#endif // MOVE_GENERATOR_H_