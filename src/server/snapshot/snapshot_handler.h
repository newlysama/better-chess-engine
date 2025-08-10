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

#include "server/snapshot/snapshots.h"

namespace server::snapshot
{
    /**
     * @brief Fills a StringBuffer with MoveSnapthot's elements.
     *
     * @param [out] writer       : the buffer to fill
     * @param [in]  moveSnapshot : the snapshot to fill with
     */
    void moveToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, const MoveSnapshot& moveSnapshot) noexcept;

    /**
     * @brief Fills a MoveSnapshot with Value's elements, doing strong checkings.
     *
     * @param [in]  value : document to fill the snapshot with
     * @param [out] move  : snapshot to be filled
     *
     * @return Nothing if value is valid, error message if not
     */
    std::expected<void, std::string> jsonToMove(const rapidjson::Value& value, MoveSnapshot& move) noexcept;

    /**
     * @brief Fills a StringBuffer with GameSnapshots elements.
     *
     * @param [out] writer       : the buffer to fill
     * @param [in]  gameSnapshot : the snapshot to fill with
     */
    void gameToJson(rapidjson::StringBuffer& buffer, const GameSnapshot& gameSnapshot) noexcept;

    /**
     * @brief Fills a GameSnapshot with Value's elements, doing strong checkings.
     *
     * @param [in]  value : document to fill the snapshot with
     * @param [out] game  : snapshot to be filled
     *
     * @return Nothing if value is valid, error message if not
     */
    std::expected<void, std::string> jsonToGame(const rapidjson::Value& value, GameSnapshot& game) noexcept;

} // namespace server::snapshot

#endif // SNAPSHOT_HANDLER_H_
