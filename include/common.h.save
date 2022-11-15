#ifndef   COMMON_H
#define   COMMON_H

//-----------------------------------------------------------------------------

#include <stdint.h>

//-----------------------------------------------------------------------------

#define POISON_STK 12340

#define DELETED_PAR -1
#define NOT_FOUND   -1

//-----------------------------------------------------------------------------

#define N(suffix)  NUM_OF_##suffix

#define SG(suffix) SIGNATURE_##suffix

#define SZ(suffix) SIZE_OF_##suffix

#define L(suffix)  LIMIT_##suffix

#define F(suffix)  FLAG_##suffix

//-----------------------------------------------------------------------------

enum Cmd_codes
{
    #define CMD_DEF(cmd, ...) \
    cmd,

    #include "codegen.h"

    #undef CMD_DEF
};

//-----------------------------------------------------------------------------

#define CJMP(...)                                   \
    double second_number = stack_pop (&Cpu->Stk);   \
    double first_number  = stack_pop (&Cpu->Stk);   \
                                                    \
    if(__VA_ARGS__)                                 \
    {                                               \
        int pos_ch = arg_value;                     \
        curr_pos = pos_ch - 2;                      \
    }                                               \
                                                    \
    stack_push (&Cpu->Stk, first_number);           \
    stack_push (&Cpu->Stk, second_number);

//-----------------------------------------------------------------------------

enum OFFSETS
{
    BASIC_OFFSET    = 1,
    TWO_ARGS_OFFSET = 2,
};

//-----------------------------------------------------------------------------

enum SIGNATURES
{
    SIGNATURE           = 0xBACAFE,
    SIGNATURE_DESTROYED = 0xDEADAC,
};

//-----------------------------------------------------------------------------

enum BIT_MASKS
{
    MASK_IMM = 0x20,
    MASK_REG = 0x40,
    MASK_RAM = 0x80,
    MASK_CMD = 0x1F,
};

//-----------------------------------------------------------------------------

enum CODES_OF_ERROR
{
    E(CTOR  = 1,
    E(ASM   = 2,
    E(CPU   = 3,
    E(DASM  = 4,
};

//-----------------------------------------------------------------------------

#endif //COMMON_H
