#ifndef   COMMON_H
#define   COMMON_H

//-----------------------------------------------------------------------------

#define CORCT_SIGN 0xBACAFE
#define WRONG_SIGN 0xDEADAC

#define MASK_IMM  0x20
#define MASK_REG  0x40
#define MASK_RAM  0x80
#define MASK_CMD  0x1F

//-----------------------------------------------------------------------------

enum CMD_CODES
{
    CMD_HLT,
    CMD_PUSH,
    CMD_POP,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_OUT,
    CMD_DUMP,
    CMD_JBE,
    CMD_JAE,
    CMD_JA,
    CMD_JB,
    CMD_JE,
    CMD_JNE,
};

enum ERROR_CODE
{
    ERR_CTOR = 1,
    ERR_ASM  = 2,
    ERR_CPU  = 3,
};

//-----------------------------------------------------------------------------

#endif //COMMON_H
