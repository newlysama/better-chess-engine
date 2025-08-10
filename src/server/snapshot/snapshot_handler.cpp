/**
 * @file      snapshots_handler.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief JSON <---> Snapshots translations implementations.
 */

#include "snapshot_handler.h"

#include <string>

#include "utils/utils.h"

namespace server::snapshot
{
    void moveToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, const MoveSnapshot& moveSnapshot) noexcept
    {
        writer.StartArray();

        writer.Uint(static_cast<unsigned>(moveSnapshot.fromSquare));
        writer.Uint(static_cast<unsigned>(moveSnapshot.toSquare));

        // Optionals
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

    std::expected<void, std::string> jsonToMove(const rapidjson::Value& value, MoveSnapshot& move) noexcept
    {
        if (!value.IsArray())
            return std::unexpected(
                std::format("Received a move document that is not an array : {}", std::to_string(value.GetType())));
        if (value.Size() < 2)
            return std::unexpected(
                std::format("Received a move document with invalid minimum size : {}", value.Size()));

        const rapidjson::GenericArray array = value.GetArray();

        if (!array[0].IsUint())
            return std::unexpected(std::format("Received a move document that has invalid fromSquares type : {}",
                                               std::to_string(array[0].GetType())));
        if (!array[1].IsUint())
            return std::unexpected(std::format("Received a move document that has invalid toSquare type : {}",
                                               std::to_string(array[1].GetType())));

        unsigned fromSquare = array[0].GetUint();
        unsigned toSquare = array[1].GetUint();

        if (fromSquare > 63)
            return std::unexpected(std::format("Received a move document with invalid fromSquare : {}", fromSquare));
        if (toSquare > 63)
            return std::unexpected(std::format("Received a move document with invalid toSquare : {}", toSquare));

        move.fromSquare = static_cast<int>(fromSquare);
        move.toSquare = static_cast<int>(toSquare);

        move.capturedSquare.reset();
        move.promotionPiece.reset();
        move.castlingType.reset();

        // Check for optional parameters
        std::size_t i = 2;
        if (i < array.Size()) // capturedSquare
        {
            if (!array[i].IsInt())
                return std::unexpected(std::format("Received a move document with invalid capturedSquare type",
                                                   std::to_string(array[i].GetType())));

            int capturedSquare = array[i].GetInt();

            if (capturedSquare < 0 || capturedSquare > 63)
                return std::unexpected(
                    std::format("Received a move document with invalid capturedSquare : {}", capturedSquare));

            move.capturedSquare = capturedSquare;
            i++;
        }
        if (i < array.Size()) // promotionPiece
        {
            if (!array[i].IsString())
                return std::unexpected(std::format("Received a move document with invalid promotionPiece type : {}",
                                                   std::to_string(array[i].GetType())));

            std::string_view promotionPiece = std::string_view{array[i].GetString(), array[i].GetStringLength()};

            if (promotionPiece.size() != 1)
                return std::unexpected(
                    std::format("Received a move document with promotionPiece of invalid size : {}", promotionPiece));

            move.promotionPiece = promotionPiece;
            i++;
        }
        if (i < array.Size() && array[i].IsString()) // castlingType
        {
            move.castlingType = std::string_view(array[i].GetString(), array[i].GetStringLength());
            i++;
        }

        return std::expected<void, std::string>{};
    }

    void gameToJson(rapidjson::StringBuffer& buffer, const GameSnapshot& gameSnapshot) noexcept
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

    std::expected<void, std::string> jsonToGame(const rapidjson::Value& value, GameSnapshot& game) noexcept
    {
        if (!value.IsObject())
            return std::unexpected(
                std::format("Received a game document that is not an object : {}", std::to_string(value.GetType())));

        const rapidjson::GenericMemberIterator end = value.MemberEnd();

        // roomId
        if (rapidjson::GenericMemberIterator it = value.FindMember("roomId"); it == end)
            return std::unexpected("Received a game document with missing field : roomId");
        else if (!it->value.IsUint())
            return std::unexpected(std::format("Received a game document that has invalid roomId type : {}",
                                               std::to_string(it->value.GetType())));
        else
            game.roomId = static_cast<server::core::RoomId>(it->value.GetUint());

        // fen
        if (rapidjson::GenericMemberIterator it = value.FindMember("fen"); it == end)
            return std::unexpected("Received a game document with missing field : fen");
        else if (!it->value.IsString())
            return std::unexpected(std::format("Received a game document that has invalid fen type : {}",
                                               std::to_string(it->value.GetType())));
        else
            game.fen = std::string_view(it->value.GetString(), it->value.GetStringLength());

        // turn
        if (rapidjson::GenericMemberIterator it = value.FindMember("turn"); it == end)
            return std::unexpected("Received a game document with missing field : turn");
        else if (!it->value.IsString())
            return std::unexpected(std::format("Received a game document that has invalid turn type : {}",
                                               std::to_string(it->value.GetType())));
        else
        {
            const auto len = it->value.GetStringLength();
            if (len != 1)
                return std::unexpected(std::format("Received a game document with turn of invalid size : {}", len));

            game.turn = std::string_view(it->value.GetString(), len);
        }

        // halfmove
        if (rapidjson::GenericMemberIterator it = value.FindMember("halfmove"); it == end)
            return std::unexpected("Received a game document with missing field : halfmove");
        else if (!it->value.IsUint())
            return std::unexpected(std::format("Received a game document that has invalid halfmove type : {}",
                                               std::to_string(it->value.GetType())));
        else
            game.halfmove = static_cast<uint16_t>(it->value.GetUint());

        // fullmove
        if (rapidjson::GenericMemberIterator it = value.FindMember("fullmove"); it == end)
            return std::unexpected("Received a game document with missing field : fullmove");
        else if (!it->value.IsUint())
            return std::unexpected(std::format("Received a game document that has invalid fullmove type : {}",
                                               std::to_string(it->value.GetType())));
        else
            game.fullmove = static_cast<uint16_t>(it->value.GetUint());

        // inCheck
        if (rapidjson::GenericMemberIterator it = value.FindMember("inCheck"); it == end)
            return std::unexpected("Received a game document with missing field : inCheck");
        else if (!it->value.IsBool())
            return std::unexpected(std::format("Received a game document that has invalid inCheck type : {}",
                                               std::to_string(it->value.GetType())));
        else
            game.inCheck = it->value.GetBool();

        // checkMate
        if (rapidjson::GenericMemberIterator it = value.FindMember("checkMate"); it == end)
            return std::unexpected("Received a game document with missing field : checkMate");
        else if (!it->value.IsBool())
            return std::unexpected(std::format("Received a game document that has invalid checkMate type : {}",
                                               std::to_string(it->value.GetType())));
        else
            game.checkMate = it->value.GetBool();

        // lastMove
        if (rapidjson::GenericMemberIterator it = value.FindMember("lastMove"); it == end)
            return std::unexpected("Received a game document with missing field : lastMove");
        else
        {
            if (std::expected<void, std::string> res = jsonToMove(it->value, game.lastMove); !res.has_value())
                return res;
        }

        // legalMoves
        if (rapidjson::GenericMemberIterator it = value.FindMember("legalMoves"); it == end)
            return std::unexpected("Received a game document with missing field : legalMoves");
        else if (!it->value.IsArray())
            return std::unexpected(std::format("Received a game document that has invalid legalMoves type : {}",
                                               std::to_string(it->value.GetType())));
        else
        {
            game.legalMoves.clear();
            for (const rapidjson::Value& mv : it->value.GetArray())
            {
                MoveSnapshot m{};
                if (std::expected<void, std::string> res = jsonToMove(mv, m); !res.has_value())
                    return res;

                game.legalMoves.push_back(m);
            }
        }

        return std::expected<void, std::string>{};
    }

} // namespace server::snapshot
