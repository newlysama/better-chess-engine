/**
 * @file bitboard.h
 * @author Thibault THOMAS
 *
 * @brief Bitboard representation.
 */

#ifndef BITBOARD_H_
#define BITBOARD_H_

/*----- System -----*/
#include <bit>
#include <cstdint>

/**
 * @namespace engine
 */
namespace engine
{
    /**
     * @namespace board
     */
    namespace board
    {
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
                 * @param [in] data_ : Value to initialize data member
                 */
                constexpr Bitboard(const uint64_t& data_ = 0ULL) noexcept
                    : data(data_)
                    {}

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
                inline constexpr bool operator==(const Bitboard& bitboard_) const noexcept { return this->data == bitboard_.data; };
                inline constexpr bool operator!=(const Bitboard& bitboard_) const noexcept { return this->data != bitboard_.data; };

                /*----------------------------------------*
                *           BITWISE OPERATORS            *
                *----------------------------------------*/
                inline constexpr Bitboard  operator&(const Bitboard& bitboard_) const noexcept { return Bitboard(this->data & bitboard_.data); };
                inline constexpr Bitboard  operator|(const Bitboard& bitboard_) const noexcept { return Bitboard(this->data | bitboard_.data); };
                inline constexpr Bitboard  operator~()                          const noexcept { return Bitboard(~this->data); };
                inline constexpr Bitboard  operator^(const Bitboard& bitboard_) const noexcept { return Bitboard(this->data ^ bitboard_.data); };
                inline constexpr Bitboard  operator<<(const int shift)          const noexcept { return Bitboard(this->data << shift); };
                inline constexpr Bitboard  operator>>(const int shift)          const noexcept { return Bitboard(this->data >> shift); };


                /*----------------------------------------*
                *         ASSIGNMENT OPERATORS           *
                *----------------------------------------*/
                inline constexpr Bitboard& operator=(const Bitboard&) noexcept = default;

                inline constexpr Bitboard& operator&=(const Bitboard& bitboard_) noexcept{ this->data &= bitboard_.data; return *this; };
                inline constexpr Bitboard& operator|=(const Bitboard& bitboard_) noexcept { this->data |= bitboard_.data; return *this; };
                inline constexpr Bitboard& operator^=(const Bitboard& bitboard_) noexcept { this->data ^= bitboard_.data; return *this; };

                inline constexpr Bitboard& operator<<=(const int shift) noexcept { this->data <<= shift; return *this; };
                inline constexpr Bitboard& operator>>=(const int shift) noexcept { this->data >>= shift; return *this; };


                /*----------------------------------------*
                *           UTILITY FUNCTIONS            *
                *----------------------------------------*/

                /**
                 * @brief  Getter for data member.
                 *
                 * @return uint64_t : the actual bitboard
                 */
                inline constexpr uint64_t getData() const noexcept { return this->data; };

                /**
                 * @brief  Get the number of bits set to 1.
                 *
                 * @return uint8_t : the number of bits set to 1
                 */
                inline constexpr uint8_t popCount() const noexcept { return std::popcount(this->data); };

                /**
                 * @brief  Check if the bitboard is empty.
                 *
                 * @return bool : wether the bitboard is empty or not
                 */
                inline constexpr bool isEmpty() const noexcept { return this->data == 0; };

                /**
                 * @brief  Get LSB's index.
                 *
                 * @return uint8_t : LSB's index
                 */
                inline constexpr uint8_t lsbIndex() const noexcept { return std::countr_zero(this->data); };

                /**
                 * @brief  Get MSB's index.
                 *
                 * @return uint8_t : MSB's index
                 */
                inline constexpr uint8_t msbIndex() const noexcept { return 63 - std::countr_zero(this->data); };

            private:
                uint64_t data; //< Actual bitboard
        };
    } // namespace board
} // namespace engine

#endif // BITBOARD_H_