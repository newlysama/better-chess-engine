/**
 * @file      bitboard.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Bitboard representation.
 */

#ifndef BITBOARD_H_
#define BITBOARD_H_

#include <bit>
#include <cstdint>

#include "conf/enums.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    using namespace conf::enums;

    /**
     * @class Bitboard
     * @brief Wrapper around an actual bitboard (64 bits unsigned int).
     * @details
     * Every bit of this wrapped 64 bits unsigned integer represents a square of the Board :
     * bit to 1 ==> square is occupied
     * bit to 0 ==> square is empty
     */
    class Bitboard
    {
      public:
        /*----------------------------------------*
         *      CONSTRUCTORS / DESTRUCTORS        *
         *----------------------------------------*/

        /**
         * @brief Constructor.
         *
         * @param [in] _data_ : Value to initialize _data member
         */
        constexpr Bitboard(const uint64_t& _data_ = 0ULL) noexcept
            : _data(_data_)
        {
        }

        /**
         * @brief Copy constructor.
         *
         * @param [in] : Bitboard instance to copy
         */
        constexpr Bitboard(const Bitboard&) noexcept = default;

        /**
         * @brief Default destructor.
         */
        ~Bitboard() noexcept = default;

        /*----------------------------------------*
         *         COMPARAISON OPERATORS          *
         *----------------------------------------*/
        inline constexpr bool operator==(const Bitboard& bitboard_) const noexcept
        {
            return this->_data == bitboard_._data;
        }

        inline constexpr bool operator!=(const Bitboard& bitboard_) const noexcept
        {
            return this->_data != bitboard_._data;
        }

        /*----------------------------------------*
         *           BITWISE OPERATORS            *
         *----------------------------------------*/
        inline constexpr Bitboard operator&(const Bitboard& bitboard_) const noexcept
        {
            return Bitboard{this->_data & bitboard_._data};
        }

        inline constexpr Bitboard operator|(const Bitboard& bitboard_) const noexcept
        {
            return Bitboard{this->_data | bitboard_._data};
        }

        inline constexpr Bitboard operator~() const noexcept
        {
            return Bitboard{~this->_data};
        }

        inline constexpr Bitboard operator^(const Bitboard& bitboard_) const noexcept
        {
            return Bitboard{this->_data ^ bitboard_._data};
        }

        inline constexpr Bitboard operator<<(const int shift) const noexcept
        {
            return Bitboard{this->_data << shift};
        }

        inline constexpr Bitboard operator>>(const int shift) const noexcept
        {
            return Bitboard{this->_data >> shift};
        }

        /*----------------------------------------*
         *         ASSIGNMENT OPERATORS           *
         *----------------------------------------*/
        inline constexpr Bitboard& operator=(const Bitboard&) noexcept = default;

        inline constexpr Bitboard& operator&=(const Bitboard& bitboard_) noexcept
        {
            this->_data &= bitboard_._data;
            return *this;
        }

        inline constexpr Bitboard& operator|=(const Bitboard& bitboard_) noexcept
        {
            this->_data |= bitboard_._data;
            return *this;
        }

        inline constexpr Bitboard& operator^=(const Bitboard& bitboard_) noexcept
        {
            this->_data ^= bitboard_._data;
            return *this;
        }

        inline constexpr Bitboard& operator<<=(const int shift) noexcept
        {
            this->_data <<= shift;
            return *this;
        }

        inline constexpr Bitboard& operator>>=(const int shift) noexcept
        {
            this->_data >>= shift;
            return *this;
        }

        /*----------------------------------------*
         *           UTILITY FUNCTIONS            *
         *----------------------------------------*/

        /**
         * @brief  Getter for _data member.
         *
         * @return uint64_t : the actual bitboard
         */
        inline constexpr uint64_t getData() const noexcept
        {
            return this->_data;
        }

        /**
         * @brief  Get the number of bits set to 1.
         *
         * @return int : the number of bits set to 1
         */
        inline constexpr int popCount() const noexcept
        {
            return std::popcount(this->_data);
        }

        /**
         * @brief  Check if the bitboard is empty.
         *
         * @return bool : wether the bitboard is empty or not
         */
        inline constexpr bool isEmpty() const noexcept
        {
            return this->_data == 0;
        }

        /**
         * @brief  Get LSB's index.
         *
         * @return int : LSB's index
         */
        inline constexpr int lsbIndex() const noexcept
        {
            return std::countr_zero(this->_data);
        }

        /**
         * @brief  Get MSB's index.
         *
         * @return int : MSB's index
         */
        inline constexpr int msbIndex() const noexcept
        {
            return 63 - std::countl_zero(this->_data);
        }

      private:
        uint64_t _data; // Actual bitboard
    };
} // namespace engine::board

#endif // BITBOARD_H_