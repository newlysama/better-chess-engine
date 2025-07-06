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

/**
 * @namespace engine::board::magic_const
 */
namespace engine::board::magic_const
{
    inline constexpr conf::types::BitboardTable rookMagics = {
        Bitboard{0x80084002801020ULL},   Bitboard{0xc040100040002000ULL}, Bitboard{0x50010200041002bULL},
        Bitboard{0x9000850010004a0ULL},  Bitboard{0x200102508200200ULL},  Bitboard{0x1200081114308200ULL},
        Bitboard{0x200220004008108ULL},  Bitboard{0x2100010000208546ULL}, Bitboard{0x108000814000e1ULL},
        Bitboard{0x1001004001812506ULL}, Bitboard{0x21001120010044ULL},   Bitboard{0xc108801000080080ULL},
        Bitboard{0x404803401080180ULL},  Bitboard{0x401000209000400ULL},  Bitboard{0x1004000188021c10ULL},
        Bitboard{0xa10a000200e10184ULL}, Bitboard{0x105808000400020ULL},  Bitboard{0x10054000502002ULL},
        Bitboard{0xd010002020080400ULL}, Bitboard{0x80122000a001040ULL},  Bitboard{0x88018008540081ULL},
        Bitboard{0x8001080104c01020ULL}, Bitboard{0x40040008021009ULL},   Bitboard{0x201020001048244ULL},
        Bitboard{0x1012400080002280ULL}, Bitboard{0x200500040002008ULL},  Bitboard{0x4080200100110044ULL},
        Bitboard{0x4001080080801000ULL}, Bitboard{0x182a80180040080ULL},  Bitboard{0x10220080040080ULL},
        Bitboard{0x1020002004c0108ULL},  Bitboard{0x400240e00008043ULL},  Bitboard{0x108800041002100ULL},
        Bitboard{0xa42804102002201ULL},  Bitboard{0x2010200080801000ULL}, Bitboard{0x8020201001000900ULL},
        Bitboard{0x850080025003100ULL},  Bitboard{0x1103000401000648ULL}, Bitboard{0x920008010c000250ULL},
        Bitboard{0x4002804a000104ULL},   Bitboard{0x200400480008020ULL},  Bitboard{0x20100240004020ULL},
        Bitboard{0x600102001010040ULL},  Bitboard{0x30008100180800ULL},   Bitboard{0x1001280004008080ULL},
        Bitboard{0x94a000804020010ULL},  Bitboard{0x888880221040010ULL},  Bitboard{0x42800c2080420001ULL},
        Bitboard{0x8845400820800180ULL}, Bitboard{0x1002400020810300ULL}, Bitboard{0x4101042000401100ULL},
        Bitboard{0x2201005090100ULL},    Bitboard{0x4820100428010100ULL}, Bitboard{0x8004000c80020080ULL},
        Bitboard{0x101241081400ULL},     Bitboard{0x20802500074080ULL},   Bitboard{0x11d82100408001ULL},
        Bitboard{0x80080c001021521ULL},  Bitboard{0x8900a003302843ULL},   Bitboard{0x80500100021ULL},
        Bitboard{0x4100100c080007ULL},   Bitboard{0x802008310280402ULL},  Bitboard{0x4201020810008304ULL},
        Bitboard{0x124460840028c102ULL}};

    inline constexpr conf::types::BitboardTable bishopMagics = {
        Bitboard{0x1010010b002209ULL},   Bitboard{0x1100202034810ULL},    Bitboard{0x6040400820000ULL},
        Bitboard{0xa0020a0240802000ULL}, Bitboard{0x2123000021022ULL},    Bitboard{0x1001209040010010ULL},
        Bitboard{0x4088210100402c1ULL},  Bitboard{0x4001149044044200ULL}, Bitboard{0x8800822088018448ULL},
        Bitboard{0x248220401041506ULL},  Bitboard{0x800410041840400aULL}, Bitboard{0x240420800026ULL},
        Bitboard{0x404040460800040ULL},  Bitboard{0x820002080a088065ULL}, Bitboard{0x10000505080a4200ULL},
        Bitboard{0x20608420a00ULL},      Bitboard{0x108080210641808ULL},  Bitboard{0x4901881280200ULL},
        Bitboard{0x204002800222a02ULL},  Bitboard{0xa01000801410328ULL},  Bitboard{0x4401000090400201ULL},
        Bitboard{0x122000918060220ULL},  Bitboard{0x200041308808a808ULL}, Bitboard{0x1009c0820030840cULL},
        Bitboard{0xa9020004041028ULL},   Bitboard{0x1804042010811800ULL}, Bitboard{0x80200b404001aULL},
        Bitboard{0xa285004024040012ULL}, Bitboard{0x240940108802000ULL},  Bitboard{0x60105000a004106ULL},
        Bitboard{0x200940001041208ULL},  Bitboard{0x404052c001002800ULL}, Bitboard{0x8a0282c000200804ULL},
        Bitboard{0x450040008150aULL},    Bitboard{0x824002400080042ULL},  Bitboard{0xa002008020560200ULL},
        Bitboard{0x47080200802200ULL},   Bitboard{0x2062008600010800ULL}, Bitboard{0x6048210045110800ULL},
        Bitboard{0x608020c0020300ULL},   Bitboard{0x802480410001410ULL},  Bitboard{0x880110804800ULL},
        Bitboard{0x202a0100480a8110ULL}, Bitboard{0x4000426011000801ULL}, Bitboard{0x220c202008800300ULL},
        Bitboard{0x842081000207100ULL},  Bitboard{0x1c50100081080884ULL}, Bitboard{0x206005d000200ULL},
        Bitboard{0xc1010050102a0000ULL}, Bitboard{0xa022482082120ULL},    Bitboard{0x2420010c01040082ULL},
        Bitboard{0x4841000120981400ULL}, Bitboard{0x24403010602a0044ULL}, Bitboard{0x8083010708080ULL},
        Bitboard{0x90c09e0811010018ULL}, Bitboard{0x410a08091120000ULL},  Bitboard{0x8030402808021100ULL},
        Bitboard{0x20018445820100ULL},   Bitboard{0x300420410411040ULL},  Bitboard{0x144202012c02228ULL},
        Bitboard{0xe10000884014400ULL},  Bitboard{0x8841052060112860ULL}, Bitboard{0x430d022014008640ULL},
        Bitboard{0x9808140100409780ULL}};

    inline constexpr std::array<uint8_t, 64> rookShifts = {
        52, 53, 53, 53, 53, 53, 53, 52, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54,
        54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54,
        54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 52, 53, 53, 53, 53, 53, 53, 52};

    inline constexpr std::array<uint8_t, 64> bishopShifts = {
        57, 58, 58, 58, 58, 58, 58, 57, 58, 59, 59, 59, 59, 59, 59, 58, 58, 59, 57, 57, 57, 57,
        59, 58, 58, 59, 57, 55, 55, 57, 59, 58, 58, 59, 57, 55, 55, 57, 59, 58, 58, 59, 57, 57,
        57, 57, 59, 58, 58, 59, 59, 59, 59, 59, 59, 58, 57, 58, 58, 58, 58, 58, 58, 57};

} // namespace engine::board::magic_const

#endif // MAGIC_CONST_H_