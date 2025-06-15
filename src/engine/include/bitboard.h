/**
 * @file bitboard.h
 * @author Thibault THOMAS
 *
 * @brief Bitboard representation
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
     * @class Bitboard
     * @brief Wrapper around an actual bitboard (64 bit unsigned int)
     * @details
     * Every bit of this wrapped 64 bits unsigned integer represents a square of the Board :
     * bit to 1 ==> square is occupied
     * bit to 0 ==> square is empty
     */
    class Bitboard
    {
        public:

            // Constructors / Destructor
            constexpr Bitboard(const uint64_t& bb_ = 0ULL) noexcept
                : bb(bb_)
                {}

            constexpr Bitboard(const Bitboard& ) noexcept = default;
            ~Bitboard() noexcept = default;


            /*----------------------------------------*
             *         COMPARAISON OPERATORS          *
             *----------------------------------------*/
            inline constexpr bool operator==(const Bitboard& bitboard_) const noexcept { return this->bb == bitboard_.bb; };
            inline constexpr bool operator!=(const Bitboard& bitboard_) const noexcept { return this->bb != bitboard_.bb; };

            /*----------------------------------------*
             *           BITWISE OPERATORS            *
             *----------------------------------------*/
            inline constexpr Bitboard  operator&(const Bitboard& bitboard_) const noexcept { return Bitboard(this->bb & bitboard_.bb); };
            inline constexpr Bitboard  operator|(const Bitboard& bitboard_) const noexcept { return Bitboard(this->bb | bitboard_.bb); };
            inline constexpr Bitboard  operator~()                          const noexcept { return Bitboard(~this->bb); };
            inline constexpr Bitboard  operator^(const Bitboard& bitboard_) const noexcept { return Bitboard(this->bb ^ bitboard_.bb); };
            inline constexpr Bitboard  operator<<(const int & shift)        const noexcept { return Bitboard(this->bb << shift); };
            inline constexpr Bitboard  operator>>(const int & shift)        const noexcept { return Bitboard(this->bb >> shift); };


            /*----------------------------------------*
             *         ASSIGNMENT OPERATORS           *
             *----------------------------------------*/
            inline constexpr Bitboard& operator=(const Bitboard&) noexcept = default;

            inline constexpr Bitboard& operator&=(const Bitboard& bitboard_) noexcept { this->bb &= bitboard_.bb; return *this; };
            inline constexpr Bitboard& operator|=(const Bitboard& bitboard_) noexcept { this->bb |= bitboard_.bb; return *this; };
            inline constexpr Bitboard& operator^=(const Bitboard& bitboard_) noexcept { this->bb ^= bitboard_.bb; return *this; };

            inline constexpr Bitboard& operator<<=(const int & shift) noexcept { this->bb <<= shift; return *this; };
            inline constexpr Bitboard& operator>>=(const int & shift) noexcept { this->bb >>= shift; return *this; };

        private:
            // Actual bitboard hold by the instance
            uint64_t bb;
    };
} // namespace engine

#endif // BITBOARD_H_