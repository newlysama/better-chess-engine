/**
 * @file      snapshots_handler.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief JSON <---> Snapshots translations.
 */

// snapshots_handler.h
#ifndef SNAPSHOT_HANDLER_H_
#define SNAPSHOT_HANDLER_H_

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "server/snapshot/snapshots.h"

namespace server::snapshot
{

    inline void moveToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, const MoveSnapshot& moveSnapshot)
    {
        writer.StartArray();

        writer.Uint(static_cast<unsigned>(moveSnapshot.fromSquare));
        writer.Uint(static_cast<unsigned>(moveSnapshot.toSquare));

        if (moveSnapshot.capturedSquare)
        {
            writer.Int(moveSnapshot.capturedSquare.value());
        }

        if (moveSnapshot.promotionPiece)
        {
            const auto& s = moveSnapshot.promotionPiece.value();
            writer.String(s.data(), static_cast<rapidjson::SizeType>(s.size()));
        }

        if (moveSnapshot.castlingType)
        {
            const auto& s = moveSnapshot.castlingType.value();
            writer.String(s.data(), static_cast<rapidjson::SizeType>(s.size()));
        }

        writer.EndArray();
    }

    inline void gameToJson(rapidjson::StringBuffer& buffer, const GameSnapshot& gameSnapshot)
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        writer.StartObject();

        writer.Key("roomId");
        writer.Uint(gameSnapshot.roomId);

        writer.Key("fen");
        writer.String(gameSnapshot.fen.data(), static_cast<rapidjson::SizeType>(gameSnapshot.fen.size()));

        writer.Key("turn");
        writer.String(gameSnapshot.turn.data(), static_cast<rapidjson::SizeType>(gameSnapshot.turn.size()));

        writer.Key("halfmove");
        writer.Uint(gameSnapshot.halfmove);

        writer.Key("fullmove");
        writer.Uint(gameSnapshot.fullmove);

        writer.Key("inCheck");
        writer.Bool(gameSnapshot.inCheck);

        writer.Key("checkMate");
        writer.Bool(gameSnapshot.checkMate);

        writer.Key("lastMove");
        moveToJson(writer, gameSnapshot.lastMove);

        writer.Key("legalMoves");
        writer.StartArray();

        for (const MoveSnapshot& mv : gameSnapshot.legalMoves)
            moveToJson(writer, mv);

        writer.EndArray();

        writer.EndObject();
    }

} // namespace server::snapshot

#endif // SNAPSHOT_HANDLER_H_
