/**
 * @file      id_pool.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Ids management using concurrent priority queue.
 */

#ifndef ID_POOL_H_
#define ID_POOL_H_

#include <atomic>
#include <functional>
#include <oneapi/tbb/concurrent_priority_queue.h>

#include "server/core/types.h"

namespace server::core
{
    class IdPool
    {
      public:
        explicit IdPool(Id start = 1) noexcept
            : m_next_{start}
        {
        }

        /**
         * @brief Get the smallest available id in the pool
         * if there is any, create one otherwise.
         */
        Id acquire() noexcept
        {
            Id id;

            if (m_free_.try_pop(id))
                return id;

            return m_next_.fetch_add(1, std::memory_order_relaxed);
        }

        /**
         * @brief Release an id by pushing it back in the pool.
         *
         * @param [in] id : id to me released
         */
        void release(const Id id) noexcept
        {
            m_free_.push(id);
        }

      private:
        std::atomic<Id> m_next_;

        // min-heap : top is the smallest element
        tbb::concurrent_priority_queue<Id, std::greater<Id>> m_free_;
    };
} // namespace server::core

#endif // ID_POOL_H_
