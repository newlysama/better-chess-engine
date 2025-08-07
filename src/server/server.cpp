/**
 * @file      server.cpp
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Server implementation.
 */

#include "server.h"

namespace server
{
    explicit Server::Server(uint16_t port)
        : m_port(port)
        , m_router(Pistache::Rest::Router())
        , m_http(Pistache::Http::Endpoint())
    {
    }

    Server::setupRoutes()
    {
    }
} // namespace server