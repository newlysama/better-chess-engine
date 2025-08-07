/**
 * @file      server.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Server API, routes HTTP endpoints.
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <cstdint>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/net.h>
#include <pistache/router.h>

namespace server
{
    class Server
    {
      public:
        /**
         * @brief Constructor.
         *
         * @param [in] port : Listening port
         */
        explicit Server(std::uint16_t port);

        /**
         * @brief Default destructor.
         */
        ~Server() = default;

        /**
         * @brief Start the server.
         */
        void start();

        /**
         * @brief Stop the server.
         */
        void stop();

      private:
        /**
         * @brief Initialize REST routes.
         */
        void setupRoutes();

        uint16_t m_port;
        Pistache::Rest::Router m_router;
        std::shared_ptr<Pistache::Http::Endpoint> m_http;

        // Non copiable server
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
    };
} // namespace server

#endif // SERVER_H_