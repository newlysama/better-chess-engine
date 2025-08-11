/**
 * @file      snapshots_handler.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief JSON <---> Snapshots translations.
 */

#ifndef SNAPSHOT_HANDLER_H_
#define SNAPSHOT_HANDLER_H_

#include <expected>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "engine/game/game.h"
#include "engine/game/move.h"
#include "server/snapshot/snapshots.h"

namespace server::snapshot
{
    /**
     * @brief Builds a MoveSnapshot from an engine Move.
     *
     * @param [in] move  : the engine Move object
     * @param [in] color : the team playing the move
     *
     * @return The built snapshot
     */
    MoveSnapshot moveToMoveSnapshot(const engine::game::Move& move, const engine::core::Color color) noexcept;

    /**
     * @brief Fills a Writer with MoveSnapthot's elements.
     *
     * @param [out] writer       : the Writer to write in
     * @param [in]  moveSnapshot : the snapshot to fill with
     */
    void moveSnapshotToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer,
                            const MoveSnapshot& moveSnapshot) noexcept;

    /**
     * @brief Builds a MoveSnapshot with Value's elements, doing strong checkings.
     *
     * @param [in]  value : document to fill the snapshot with
     *
     * @return Nothing if value is valid, error message if not
     */
    std::expected<MoveSnapshot, std::string> jsonToMoveSnapshot(const rapidjson::Value& value) noexcept;

    /**
     * @brief Builds a GameSnapshot from an engine Game.
     * The room's id and last played move's snapshot are filled elsewhere.
     *
     * @param [in] game : the engine Game object
     *
     * @return The built GameSnapshot
     */
    GameSnapshot gameToGameSnapshot(engine::game::Game& game) noexcept;

    /**
     * @brief Fills a StringBuffer with GameSnapshots elements.
     *
     * @param [out] buffer       : the StringBuffer to write in
     * @param [in]  gameSnapshot : the snapshot to fill with
     */
    void gameSnapshotToJson(rapidjson::StringBuffer& buffer, const GameSnapshot& gameSnapshot) noexcept;

} // namespace server::snapshot

#endif // SNAPSHOT_HANDLER_H_
