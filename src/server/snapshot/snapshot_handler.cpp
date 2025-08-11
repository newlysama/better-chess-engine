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

#include "engine/core/enums.h"
#include "engine/game/move_list.h"
#include "utils/utils.h"

namespace server::snapshot
{
    using namespace engine::core;
    using namespace engine::game;
    using namespace engine::board;

    MoveSnapshot moveToMoveSnapshot(const engine::game::Move& move, const engine::core::Color color) noexcept
    {
        MoveSnapshot snapshot;

        snapshot.fromSquare = move.getFromSquare();
        snapshot.toSquare = move.getToSquare();

        MoveType type = move.getMoveType();
        if (type == MoveType::CAPTURE)
        {
            snapshot.capturedSquare = snapshot.toSquare;
        }
        else if (type == MoveType::EN_PASSANT)
        {
            snapshot.capturedSquare = color == Color::WHITE ? snapshot.toSquare - 8 : snapshot.toSquare + 8;
        }
        else if (type == MoveType::CASTLE)
        {
            Castling castling = move.getCastlingType();
            switch (castling)
            {
            case Castling::WHITE_KING_SIDE:
                snapshot.castlingType = "WK";
                break;
            case Castling::WHITE_QUEEN_SIDE:
                snapshot.castlingType = "WQ";
                break;
            case Castling::BLACK_KING_SIDE:
                snapshot.castlingType = "BK";
                break;
            case Castling::BLACK_QUEEN_SIDE:
                snapshot.castlingType = "BQ";
                break;
            default:
                snapshot.castlingType = "?";
                break;
            }
        }

        if (move.isPromotion())
        {
            Piece promotionPiece = move.getPromotionPiece();
            snapshot.promotionPiece = std::string{utils::pieceToFenChar(color, promotionPiece)};
        }

        return snapshot;
    }

    void moveSnapshotToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer,
                            const MoveSnapshot& moveSnapshot) noexcept
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

    std::expected<MoveSnapshot, std::string> jsonToMoveSnapshot(const rapidjson::Value& value) noexcept
    {
        MoveSnapshot snapshot;

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

        snapshot.fromSquare = static_cast<int>(fromSquare);
        snapshot.toSquare = static_cast<int>(toSquare);

        snapshot.capturedSquare.reset();
        snapshot.promotionPiece.reset();
        snapshot.castlingType.reset();

        // Check for optional parameters
        std::size_t i = 2;
        if (i < array.Size()) // capturedSquare
        {
            if (!array[i].IsInt())
                return std::unexpected(std::format("Received a move document with invalid capturedSquare type : {}",
                                                   std::to_string(array[i].GetType())));

            int capturedSquare = array[i].GetInt();

            if (capturedSquare < 0 || capturedSquare > 63)
                return std::unexpected(
                    std::format("Received a move document with invalid capturedSquare : {}", capturedSquare));

            snapshot.capturedSquare = capturedSquare;
            i++;
        }
        if (i < array.Size()) // promotionPiece
        {
            if (!array[i].IsString())
                return std::unexpected(std::format("Received a move document with invalid promotionPiece type : {}",
                                                   std::to_string(array[i].GetType())));

            std::string promotionPiece = std::string{array[i].GetString(), array[i].GetStringLength()};

            if (promotionPiece.size() != 1)
                return std::unexpected(
                    std::format("Received a move document with promotionPiece of invalid size : {}", promotionPiece));

            snapshot.promotionPiece = promotionPiece;
            i++;
        }
        if (i < array.Size() && array[i].IsString()) // castlingType
        {
            snapshot.castlingType = std::string(array[i].GetString(), array[i].GetStringLength());
            i++;
        }

        return snapshot;
    }

    GameSnapshot gameToGameSnapshot(engine::game::Game& game) noexcept
    {
        GameSnapshot snapshot;
        State& state = game.m_state;

        snapshot.fen = state.buildFenOccupancy();
        snapshot.turn = state.m_sideToMove == Color::WHITE ? "w" : "b";
        snapshot.halfmove = state.m_halfMoveClock;
        snapshot.fullmove = state.m_fullMoveClock;
        snapshot.inCheck = state.m_isChecked;
        snapshot.checkMate = state.m_isCheckMate;

        snapshot.legalMoves.reserve(game.m_moveList.size());
        for (const Move& move : game.m_moveList)
        {
            snapshot.legalMoves.push_back(moveToMoveSnapshot(move, state.m_sideToMove));
        }

        return snapshot;
    }

    void gameSnapshotToJson(rapidjson::StringBuffer& buffer, const GameSnapshot& gameSnapshot) noexcept
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
        moveSnapshotToJson(writer, gameSnapshot.lastMove);

        writer.Key("legalMoves");
        writer.StartArray();

        for (const MoveSnapshot& moveSnapshot : gameSnapshot.legalMoves)
            moveSnapshotToJson(writer, moveSnapshot);

        writer.EndArray();

        writer.EndObject();
    }

} // namespace server::snapshot
