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
#include <vector>

#include "engine/game/move.h"
#include "server/core/types.h"

namespace server::snapshots
{
    struct MoveSnapshot
    {
        uint8_t fromSquare;
        uint8_t toSquare;
    }

    struct GameSnapshot
    {
        core::RoomId roomId;
        std::string fen;
        char8_t turn;
        uint16_t halfmove;
        uint16_t fullmove;
        bool inCheck;
        bool checkMate;
        engine::game::Move lastMove;
        std::vector<> legalMoves;
    }
} // namespace server::snapshots

#endif // SNAPSHOTS_H_
