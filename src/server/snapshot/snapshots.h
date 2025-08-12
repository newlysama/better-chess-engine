/**
 * @file      snapshots.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Declare snapshot structures (meant to built / decode payloads).
 */

#ifndef SNAPSHOTS_H_
#define SNAPSHOTS_H_

#include <cstdint>
#include <optional>
#include <vector>

#include "server/core/types.h"

namespace server::snapshot
{
    /**
     * @typedef MoveSnapshot
     * @brief   C++ interface with RapidJson for Move representation.
     */
    typedef struct MoveSnapshot
    {
        engine::core::Color color;
        int fromSquare;
        int toSquare;
        std::optional<int> capturedSquare;
        std::optional<std::string> promotionPiece;
        std::optional<std::string> castlingType;

        MoveSnapshot() = default;
        ~MoveSnapshot() = default;
    } MoveSnapshot;

    /**
     * @typedef GameSnapshot
     * @brief   C++ interface with RapidJson for Game state representation.
     */
    typedef struct GameSnapshot
    {
        core::Id roomId;
        std::string fen; // Only the occupancy part (ex: 8/8/8/8/8/8/8/8)
        std::string turn;
        uint16_t halfmove;
        uint16_t fullmove;
        bool inCheck;
        bool checkMate;
        MoveSnapshot lastMove; // The move that was just played
        std::vector<MoveSnapshot> legalMoves;

        GameSnapshot() = default;
        ~GameSnapshot() = default;
    } GameSnapshot;
} // namespace server::snapshot

#endif // SNAPSHOTS_H_
