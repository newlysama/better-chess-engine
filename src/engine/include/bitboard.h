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
            /**
             * @brief Constructor
             */
            constexpr Bitboard(const uint64_t & bb_ = 0ULL) noexcept;

            /**
             * @brief Destructor
             */
            ~Bitboard() noexcept = default;

            /**
             * @brief Copy constructor
             */
            constexpr Bitboard(const Bitboard & bitboard_) noexcept;

        private:
            // Actual bitboard hold by the instance
            uint64_t bb;
    };
} // namespace engine

#endif // BITBOARD_H_