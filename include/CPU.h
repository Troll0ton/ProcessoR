//! @file CPU.h

#ifndef   CPU_H
#define   CPU_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"

//-----------------------------------------------------------------------------

#define num_sup_cmd 11
#define ARG_REG   0x10
#define ARG_IMMED 0x20
#define ARG_RAM   0x40

//-----------------------------------------------------------------------------

enum CMD_CODES
{
    CMD_HLT,
    CMD_PUSH,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_OUT,
    CMD_DUMP,
    CMD_JUMP,
    CMD_RG_PUSH,
    CMD_RM_RPUSH,
};

//-----------------------------------------------------------------------------

enum CMD_CODES_
{
    CMD_HLT_,
    CMD_PUSH_,
    CMD_ADD_,
    CMD_SUB_,
    CMD_MUL_,
    CMD_DIV_,
    CMD_OUT_,
    CMD_DUMP_,
    CMD_JUMP_,
    CMD_RG_PUSH_ = 0x11,
    CMD_RM_PUSH_ = 0x41,
};

//-----------------------------------------------------------------------------

typedef struct Cmd_got
{
    int num;
    char par;
} Cmd_got;

//-----------------------------------------------------------------------------

const Cmd_got Cmd_cpu[] =
{
    {CMD_HLT,   0},
    {CMD_PUSH,  1},
    {CMD_ADD,   0},
    {CMD_SUB,   0},
    {CMD_MUL,   0},
    {CMD_DIV,   0},
    {CMD_OUT,   0},
    {CMD_DUMP,  0},
    {CMD_JUMP,  2},
    {0x11    ,  3},
    {0x41    ,  3},
};

//-----------------------------------------------------------------------------

void processor ();

void code_dump (double *code, int size);

void read_label_file (FILE *label_file_, int **labels_);

void read_code_file (FILE *code_file_, double **code_);

void calculator (Stack *stk_, double *code_, int *regs_, double *ram_, int *labels_);

//-----------------------------------------------------------------------------

#endif //CPU_H
