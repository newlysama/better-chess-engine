/**
 * @file      bitboard.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Bitboard wrapper.
 * Every bit of this wrapped 64 bits unsigned integer represents a square of the Board :
 * bit to 1 ==> square is occupied
 * bit to 0 ==> square is empty
 */

#ifndef BITBOARD_H_
#define BITBOARD_H_

#include <assert.h>
#include <bit>
#include <cstdint>

namespace engine::board
{
    class Bitboard
    {
      public:
        /*----------------------------------------*
         *      CONSTRUCTORS / DESTRUCTORS        *
         *----------------------------------------*/

        /**
         * @brief Constructor.
         *
         * @param [in] data : Value to initialize m_data member
         */
        constexpr Bitboard(const uint64_t& data = 0ULL) noexcept
            : m_data(data)
        {
        }

        /**
         * @brief Copy constructor.
         *
         * @param [in] Bitboard : instance to copy
         */
        constexpr Bitboard(const Bitboard&) noexcept = default;

        /**
         * @brief Default destructor.
         */
        ~Bitboard() noexcept = default;

        /*----------------------------------------*
         *         COMPARAISON OPERATORS          *
         *----------------------------------------*/

        inline constexpr bool operator==(const Bitboard& bb) const noexcept
        {
            return m_data == bb.m_data;
        }

        inline constexpr bool operator!=(const Bitboard& bb) const noexcept
        {
            return m_data != bb.m_data;
        }

        /*----------------------------------------*
         *           BITWISE OPERATORS            *
         *----------------------------------------*/

        inline constexpr Bitboard operator&(const Bitboard& bb) const noexcept
        {
            return Bitboard{m_data & bb.m_data};
        }

        inline constexpr Bitboard operator|(const Bitboard& bb) const noexcept
        {
            return Bitboard{m_data | bb.m_data};
        }

        inline constexpr Bitboard operator~() const noexcept
        {
            return Bitboard{~m_data};
        }

        inline constexpr Bitboard operator^(const Bitboard& bb) const noexcept
        {
            return Bitboard{m_data ^ bb.m_data};
        }

        inline constexpr Bitboard operator<<(const int shift) const noexcept
        {
            return Bitboard{m_data << shift};
        }

        inline constexpr Bitboard operator>>(const int shift) const noexcept
        {
            return Bitboard{m_data >> shift};
        }

        /*----------------------------------------*
         *         ASSIGNMENT OPERATORS           *
         *----------------------------------------*/

        inline constexpr Bitboard& operator=(const Bitboard&) noexcept = default;

        inline constexpr Bitboard& operator&=(const Bitboard& bb) noexcept
        {
            m_data &= bb.m_data;
            return *this;
        }

        inline constexpr Bitboard& operator|=(const Bitboard& bb) noexcept
        {
            m_data |= bb.m_data;
            return *this;
        }

        inline constexpr Bitboard& operator^=(const Bitboard& bb) noexcept
        {
            m_data ^= bb.m_data;
            return *this;
        }

        inline constexpr Bitboard& operator<<=(const int shift) noexcept
        {
            m_data <<= shift;
            return *this;
        }

        inline constexpr Bitboard& operator>>=(const int shift) noexcept
        {
            m_data >>= shift;
            return *this;
        }

        /*----------------------------------------*
         *           UTILITY FUNCTIONS            *
         *----------------------------------------*/

        /**
         * @brief Get a bit's value at a specific index.
         *
         * @param [in] index : index to look at
         *
         * @return The bit's value
         */
        inline constexpr int at(int index) const noexcept
        {
            return int((m_data >> index) & 1ULL);
        }

        /**
         * @brief  Get the number of bits set to 1.
         *
         * @return The number of bits set to 1
         */
        inline constexpr int popCount() const noexcept
        {
            return std::popcount(m_data);
        }

        /**
         * @brief Checker whether a bit is set to 1 at a specific index.
         *
         * @param [in] index : the bit's index
         *
         * @return True if the bit at index is set to 1, false otherwise
         */
        inline constexpr bool isSet(int index) const noexcept
        {
            return (((m_data >> index) & 1ULL) == 1ULL);
        }

        /**
         * @brief Set a bit to to 1.
         *
         * @param [in] index : the bit's index
         */
        inline constexpr void set(int index) noexcept
        {
            m_data |= (1ULL << index);
        }

        /**
         * @brief Set a bit to to 0.
         *
         * @param [in] index : the bit's index
         */
        inline constexpr void unset(int index) noexcept
        {
            m_data &= ~(1ULL << index);
        }

        /**
         * @brief  Check if the bitboard is empty.
         *
         * @return True if m_data is 0, false otherwise
         */
        inline constexpr bool isEmpty() const noexcept
        {
            return m_data == 0;
        }

        /**
         * @brief  Get LSB's index.
         *
         * @return LSB's index
         */
        inline constexpr int lsbIndex() const noexcept
        {
            assert(m_data != 0);
            return std::countr_zero(m_data);
        }

        /**
         * @brief  Get MSB's index.
         *
         * @return MSB's index
         */
        inline constexpr int msbIndex() const noexcept
        {
            assert(m_data != 0);
            return 63 - std::countl_zero(m_data);
        }

        /**
         * @brief Pops the LSB.
         */
        inline constexpr void popLsb() noexcept
        {
            assert(m_data != 0);
            m_data &= ~(1ULL << this->lsbIndex());
        }

        /**
         * @brief Pops the MSB.
         */
        inline constexpr void popMsb() noexcept
        {
            assert(m_data != 0);
            m_data &= ~(1ULL << this->msbIndex());
        }

        uint64_t m_data; // Actual bitboard
    };
} // namespace engine::board

#endif // BITBOARD_H_