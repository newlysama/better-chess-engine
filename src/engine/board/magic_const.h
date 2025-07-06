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

#include "conf/types.h"
#include "engine/board/mask.h"

namespace engine::board::magic_const
{
    inline constexpr conf::types::BitboardTable rookMagics = {
        Bitboard{0x20400a000840800ULL},  Bitboard{0x1040000840060410ULL}, Bitboard{0x8060010244202000ULL},
        Bitboard{0x220008002200140ULL},  Bitboard{0x1300080000840201ULL}, Bitboard{0x10000410000220cULL},
        Bitboard{0x400002102004804ULL},  Bitboard{0x20400110090020c0ULL}, Bitboard{0x2382080018008400ULL},
        Bitboard{0x8000080002284000ULL}, Bitboard{0x5104408008000440ULL}, Bitboard{0x1100200108900220ULL},
        Bitboard{0x1000080104408100ULL}, Bitboard{0x8202000100904a00ULL}, Bitboard{0x210400081a20210ULL},
        Bitboard{0x483080040003108ULL},  Bitboard{0x221000049000ULL},     Bitboard{0x101000440022ULL},
        Bitboard{0x1108011000100a80ULL}, Bitboard{0x81040100200ULL},      Bitboard{0x14040200020080ULL},
        Bitboard{0x109108000420020ULL},  Bitboard{0x44a00089300ULL},      Bitboard{0x800024000100028ULL},
        Bitboard{0x400408004800ULL},     Bitboard{0x80620008400ULL},      Bitboard{0x100150000240ULL},
        Bitboard{0x280044001000ULL},     Bitboard{0x8040c000040ULL},      Bitboard{0x40804008220ULL},
        Bitboard{0x400001005400020ULL},  Bitboard{0x8a00021040000910ULL}, Bitboard{0x180404206100ULL},
        Bitboard{0x500024180ac00030ULL}, Bitboard{0x80440088100ULL},      Bitboard{0x200028906000120ULL},
        Bitboard{0x4000010003000148ULL}, Bitboard{0x800008020040040ULL},  Bitboard{0x840000a002054008ULL},
        Bitboard{0xa00002400410080ULL},  Bitboard{0x1000608090400ULL},    Bitboard{0x408820841400ULL},
        Bitboard{0x10052010200ULL},      Bitboard{0x209020051000ULL},     Bitboard{0x5000004064244800ULL},
        Bitboard{0xa854304000200400ULL}, Bitboard{0x8000280404ULL},       Bitboard{0x220c0278408ULL},
        Bitboard{0x800500088000410ULL},  Bitboard{0x808010261a000600ULL}, Bitboard{0x21009220200ULL},
        Bitboard{0x474081000815a00ULL},  Bitboard{0x2000020040088040ULL}, Bitboard{0x1000018400200020ULL},
        Bitboard{0x220802102024400ULL},  Bitboard{0x18004110810ULL},      Bitboard{0x48110080202441ULL},
        Bitboard{0x900502402105ULL},     Bitboard{0x400050212040040aULL}, Bitboard{0x4000010700022295ULL},
        Bitboard{0x20100479006ULL},      Bitboard{0x4041880401a902ULL},   Bitboard{0xc000010020524884ULL},
        Bitboard{0x1800004a81044022ULL}};

    inline constexpr conf::types::BitboardTable bishopMagics = {
        Bitboard{0x10031004081210ULL},   Bitboard{0x18a08100148000ULL},   Bitboard{0x108024480600000ULL},
        Bitboard{0x8060260000020ULL},    Bitboard{0x80646140088d000ULL},  Bitboard{0x10421002a0002848ULL},
        Bitboard{0x1008208049220001ULL}, Bitboard{0x1001a08028200220ULL}, Bitboard{0x401240820530ULL},
        Bitboard{0x2624803420d14ULL},    Bitboard{0x6006408a00100202ULL}, Bitboard{0x8000004844062004ULL},
        Bitboard{0x8004010040000800ULL}, Bitboard{0x8410008200600004ULL}, Bitboard{0x202020a060200ULL},
        Bitboard{0x500038c401004100ULL}, Bitboard{0xd8006080808a8ULL},    Bitboard{0x20040a408090ULL},
        Bitboard{0x4000900400040410ULL}, Bitboard{0x8000480802004000ULL}, Bitboard{0x8000104802022021ULL},
        Bitboard{0x5000b000800ULL},      Bitboard{0x402d801110202000ULL}, Bitboard{0x1002822809804800ULL},
        Bitboard{0x60200100600d8600ULL}, Bitboard{0x100210000284400ULL},  Bitboard{0x800044002000230ULL},
        Bitboard{0x3a00880008200040ULL}, Bitboard{0x8080100401021c00ULL}, Bitboard{0x180040408601000ULL},
        Bitboard{0x420004480a20ULL},     Bitboard{0x40001020e8aad040ULL}, Bitboard{0x6282900041000ULL},
        Bitboard{0x4000807080200820ULL}, Bitboard{0x40404002100ULL},      Bitboard{0x1000008ca1012400ULL},
        Bitboard{0x1081480700200ULL},    Bitboard{0x452100a020100200ULL}, Bitboard{0x8000158101016400ULL},
        Bitboard{0x8a02081074028040ULL}, Bitboard{0x8886029060180ULL},    Bitboard{0x50420c020220020ULL},
        Bitboard{0x9040a00418013000ULL}, Bitboard{0x108000200620280ULL},  Bitboard{0x1401040012000400ULL},
        Bitboard{0x500081900048d080ULL}, Bitboard{0x10004300ca8100ULL},   Bitboard{0x8202041022184180ULL},
        Bitboard{0x2000180610828400ULL}, Bitboard{0x281000802080200ULL},  Bitboard{0x2010020088a04200ULL},
        Bitboard{0x100308003d8a0020ULL}, Bitboard{0x8002000086940800ULL}, Bitboard{0x400480408400ULL},
        Bitboard{0x8811041446000ULL},    Bitboard{0x2004040240c00882ULL}, Bitboard{0x842410020808ULL},
        Bitboard{0x22340201a0804800ULL}, Bitboard{0xc8b2000008262202ULL}, Bitboard{0x818028000290400ULL},
        Bitboard{0x10200000c008280ULL},  Bitboard{0x2060400a20040108ULL}, Bitboard{0x11420840402ULL},
        Bitboard{0x108010040890200ULL}};

    inline constexpr std::array<uint8_t, 64> rookShifts = {
        50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
        50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
        50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};

    inline constexpr std::array<uint8_t, 64> bishopShifts = {
        57, 57, 57, 57, 57, 57, 57, 57, 57, 55, 55, 55, 55, 55, 55, 57, 57, 55, 53, 53, 53, 53,
        55, 57, 57, 55, 53, 51, 51, 53, 55, 57, 57, 55, 53, 51, 51, 53, 55, 57, 57, 55, 53, 53,
        53, 53, 55, 57, 57, 55, 55, 55, 55, 55, 55, 57, 57, 57, 57, 57, 57, 57, 57, 57};

} // namespace engine::board::magic_const

#endif // MAGIC_CONST_H_