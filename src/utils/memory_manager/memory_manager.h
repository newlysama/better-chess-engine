/**
 * @file      memory_manager.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Bump allocator definition.
 */

#include <array>
#include <cstddef>
#include <exception>

#include "logging/logging.h"

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

/**
 * @namespace utils::memory_manager
 */
namespace utils::memory_manager
{
    /**
     * @class FixedArena
     */
    template <std::size_t Capacity>
    class FixedArena
    {
      public:
        /**
         * @brief Constructor
         */
        FixedArena() noexcept
            : writeIndex(0)
        {
        }

        /**
         * @brief Returns a pointer to writable memory area for a certain number of bytes,
         * and increment writeIndex to the next one.
         *
         * @param [in] bytes : the number of bytes to allocate
         * @return void* : pointer the the writable memory area
         */
        inline constexpr void* allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t))
        {
            // Computes offset for alignment
            std::size_t current = writeIndex;
            std::size_t pad = (alignment - (current % alignment)) % alignment;
            std::size_t need = current + pad + bytes;

            if ([[unlikely]] need > Capacity)
            {
                // clang-format off
                LOG_ERROR(
                    "Tried to allocate to much bytes on bump allocator. Capacity: {} - Write index: {} - Requested bytes: {}"
                    , Capacity
                    , this->writeIndex
                    , bytes
                );

                throw std::out_of_range{"BumpAllocator overflow"};
            }

            void* ptr = buffer.data() + current + pad;
            writeIndex = need;
            return ptr;
        }

        inline void reset() noexcept;
        {
            this->writeIndex = 0;
        }

        // Need to make this public as we will use it as size attribute
        // to browse our buffer
        std::size_t writeIndex; // Current free byte

      private:
        std::array<char, Capacity> buffer; // Pre-allocated buffer
    };
} // namespace utils::memory_manager

#endif // MEMORY_MANAGER_H