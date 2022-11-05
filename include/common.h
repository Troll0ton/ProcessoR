#ifndef   COMMON_H
#define   COMMON_H

//-----------------------------------------------------------------------------

#include <stdint.h>

//-----------------------------------------------------------------------------

#define CMD_FUNCT int cmd_d,        \
                      double arg_d, \
                      int *ipp,     \
                      Processor *Cpu

//-----------------------------------------------------------------------------

enum CMD_CODES
{
    #define CMD_DEF(cmd, ...) \
    cmd,

    #include "codegen.h"

    #undef CMD_DEF
};

//-----------------------------------------------------------------------------

enum Signatures
{
    CORCT_SIGN = 0xBACAFE,
    WRONG_SIGN = 0xDEADAC,
};

//-----------------------------------------------------------------------------

enum Bit_masks
{
    MASK_IMM = 0x20,
    MASK_REG = 0x40,
    MASK_RAM = 0x80,
    MASK_CMD = 0x1F,
};

//-----------------------------------------------------------------------------

enum Error_codes
{
    ERR_CTOR  = 1,
    ERR_ASM   = 2,
    ERR_CPU   = 3,
    ERR_DASM  = 4,
};

//-----------------------------------------------------------------------------

#endif //COMMON_H
