//! @file CPU.h

#ifndef   CPU_H
#define   CPU_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../include/ASM.h"

//-----------------------------------------------------------------------------

#define ARG_REG   0x10
#define ARG_IMMED 0x20
#define ARG_RAM   0x40

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
    {CMD_HLT_,   0},
    {CMD_PUSH_,  1},
    {CMD_ADD_,   0},
    {CMD_SUB_,   0},
    {CMD_MUL_,   0},
    {CMD_DIV_,   0},
    {CMD_OUT_,   0},
    {CMD_DUMP_,  0},
    {CMD_JUMP_,  2},
    {CMD_RG_PUSH_    ,  3},
    {CMD_RM_PUSH_    ,  3},
};

//-----------------------------------------------------------------------------

void processor ();

void code_dump (double *code, int size);

void read_label_file (FILE *label_file_, int **labels_);

void read_code_file (FILE *code_file_, double **code_);

void calculator (Stack *stk_, double *code_, int *regs_, double *ram_, int *labels_);

void label_dump (int *label, int size);

//-----------------------------------------------------------------------------

#endif //CPU_H
