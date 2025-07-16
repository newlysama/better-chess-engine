/**
 * @file      magic_consts.h
 * @author    Thibault THOMAS
 * @copyright Copyright 2025 Better Chess Engine
 * @par       This project is released under the MIT License
 *
 * @brief Magic numbers constants.
 */

#ifndef MAGIC_CONST_H_
#define MAGIC_CONST_H_

#include <array>
#include <cstdint>

#include "engine/board/bitboard.h"
#include "engine/core/types.h"

/**
 * @namespace engine::board
 */
namespace engine::board
{
    inline constexpr core::BitboardTable rookMagics = {
        Bitboard{0x80001420400280ULL},   Bitboard{0xcc0002000900842ULL},  Bitboard{0x100200102104008ULL},
        Bitboard{0x100200900041001ULL},  Bitboard{0x2080020c00080080ULL}, Bitboard{0x200080600019004ULL},
        Bitboard{0x400320400991018ULL},  Bitboard{0x20000884021020cULL},  Bitboard{0x2400800040106080ULL},
        Bitboard{0x2000401001200149ULL}, Bitboard{0x2000801008200080ULL}, Bitboard{0x42a2004020120038ULL},
        Bitboard{0x10800800800402ULL},   Bitboard{0x6000804420030ULL},    Bitboard{0x400b00044e008100ULL},
        Bitboard{0x2005800040800100ULL}, Bitboard{0x848000c00020ULL},     Bitboard{0x20038020400080ULL},
        Bitboard{0xa820010204201ULL},    Bitboard{0x808010001800ULL},     Bitboard{0x287010008010411ULL},
        Bitboard{0x48808044000600ULL},   Bitboard{0x9808001000200ULL},    Bitboard{0x18020001408904ULL},
        Bitboard{0x104802080004001ULL},  Bitboard{0x10002004c0005003ULL}, Bitboard{0x100880200082ULL},
        Bitboard{0x8008100080280080ULL}, Bitboard{0x48008280088400ULL},   Bitboard{0x4200040080020080ULL},
        Bitboard{0x2000200080104ULL},    Bitboard{0x4020208200004104ULL}, Bitboard{0x5280002018400441ULL},
        Bitboard{0x1000802103004008ULL}, Bitboard{0x10a0002480801002ULL}, Bitboard{0x201808800805000ULL},
        Bitboard{0x8060140080800800ULL}, Bitboard{0x200402018011004ULL},  Bitboard{0x800210030400c812ULL},
        Bitboard{0x2408c02000051ULL},    Bitboard{0x844002208000ULL},     Bitboard{0x3002200242c002ULL},
        Bitboard{0x2408200051010040ULL}, Bitboard{0x2500021010008ULL},    Bitboard{0x2054440008008080ULL},
        Bitboard{0x2002001885020010ULL}, Bitboard{0x102581308a040008ULL}, Bitboard{0x200100806a0004ULL},
        Bitboard{0x20248000400080ULL},   Bitboard{0x40c0008040601180ULL}, Bitboard{0x40110040200300ULL},
        Bitboard{0x2040300180080180ULL}, Bitboard{0x3300800800040080ULL}, Bitboard{0xc82004490080200ULL},
        Bitboard{0x8006121085080400ULL}, Bitboard{0x80004b000080ULL},     Bitboard{0x2001025040800021ULL},
        Bitboard{0x800110020400581ULL},  Bitboard{0xa19412000500b01ULL},  Bitboard{0x1010002008041101ULL},
        Bitboard{0x522000418201002ULL},  Bitboard{0x2049002400080205ULL}, Bitboard{0x8190218802010cULL},
        Bitboard{0x8400024081002c02ULL}};

    inline constexpr core::BitboardTable bishopMagics = {
        Bitboard{0x8925484040400a1ULL},  Bitboard{0x62204c0300410400ULL}, Bitboard{0x13010401000985ULL},
        Bitboard{0x1220920040013200ULL}, Bitboard{0x3065104050040240ULL}, Bitboard{0x8222013028100000ULL},
        Bitboard{0x440424404810ULL},     Bitboard{0x70648210100e10ULL},   Bitboard{0x200241082020400ULL},
        Bitboard{0x2804310401122220ULL}, Bitboard{0x80c01408220a4004ULL}, Bitboard{0xc0400818002ULL},
        Bitboard{0x71040210400ULL},      Bitboard{0x28012820500080ULL},   Bitboard{0x24008250900cULL},
        Bitboard{0x5820ee088041020ULL},  Bitboard{0x22088900210013aULL},  Bitboard{0x1802011948010410ULL},
        Bitboard{0x10c000800540208ULL},  Bitboard{0x2008402404000804ULL}, Bitboard{0x840800400a02000ULL},
        Bitboard{0x631000080414000ULL},  Bitboard{0xc0008042180c0208ULL}, Bitboard{0x804080021041081cULL},
        Bitboard{0x3008048018101084ULL}, Bitboard{0x1541a0010020842ULL},  Bitboard{0x80820230006200ULL},
        Bitboard{0x3008080001820022ULL}, Bitboard{0x2ca840004802020ULL},  Bitboard{0x40801100a010ULL},
        Bitboard{0x21004111041010ULL},   Bitboard{0x80090440002a0800ULL}, Bitboard{0x8210021003200c02ULL},
        Bitboard{0x9012000301410ULL},    Bitboard{0x60c002400021404ULL},  Bitboard{0xb000400a00002200ULL},
        Bitboard{0x2010008200002200ULL}, Bitboard{0x84080480503000ULL},   Bitboard{0x84210400120280ULL},
        Bitboard{0x28046100028080ULL},   Bitboard{0x2022004a02010ULL},    Bitboard{0x1001040504102080ULL},
        Bitboard{0x221620048401004ULL},  Bitboard{0x400214010440204ULL},  Bitboard{0x100080304002044ULL},
        Bitboard{0x40181a81202100ULL},   Bitboard{0x30020244000040ULL},   Bitboard{0x4010064602208040ULL},
        Bitboard{0x10a020304410a00ULL},  Bitboard{0x19210808140040ULL},   Bitboard{0x10088048011ULL},
        Bitboard{0x1000281020a80104ULL}, Bitboard{0x20f0001002020000ULL}, Bitboard{0x802014102b8300ULL},
        Bitboard{0x1008021802040050ULL}, Bitboard{0x8102080e00820000ULL}, Bitboard{0x40130802022002ULL},
        Bitboard{0x200602402280401ULL},  Bitboard{0x82042a0200441200ULL}, Bitboard{0x2045000411080ULL},
        Bitboard{0x2000008040250101ULL}, Bitboard{0x4202040102ULL},       Bitboard{0x103026080160ULL},
        Bitboard{0x10200a20820010ULL}};

    inline constexpr std::array<uint8_t, 64> rookShifts = {
        52, 53, 53, 53, 53, 53, 53, 52, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54,
        54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54,
        54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 52, 53, 53, 53, 53, 53, 53, 52};

    inline constexpr std::array<uint8_t, 64> bishopShifts = {
        58, 59, 59, 59, 59, 59, 59, 58, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 57, 57, 57, 57,
        59, 59, 59, 59, 57, 55, 55, 57, 59, 59, 59, 59, 57, 55, 55, 57, 59, 59, 59, 59, 57, 57,
        57, 57, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 58, 59, 59, 59, 59, 59, 59, 58};

} // namespace engine::board

#endif // MAGIC_CONST_H_