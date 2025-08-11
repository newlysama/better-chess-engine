/**
 * @file      snapshots.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Declare snapshot structures (payloads).
 */

#ifndef SNAPSHOTS_H_
#define SNAPSHOTS_H_

#include <cstdint>
#include <optional>
#include <vector>

#include "engine/game/move.h"
#include "server/core/types.h"

namespace server::snapshot
{
    struct MoveSnapshot
    {
        int fromSquare;
        int toSquare;
        std::optional<int> capturedSquare;
        std::optional<std::string> promotionPiece;
        std::optional<std::string> castlingType;
    };

    struct GameSnapshot
    {
        core::RoomId roomId;
        std::string fen;
        std::string turn;
        uint16_t halfmove;
        uint16_t fullmove;
        bool inCheck;
        bool checkMate;
        MoveSnapshot lastMove;
        std::vector<MoveSnapshot> legalMoves;
    };
} // namespace server::snapshot

#endif // SNAPSHOTS_H_
