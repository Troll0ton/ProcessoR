//! @file CPU.h

#ifndef   CPU_H
#define   CPU_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../include/ASM.h"

//-----------------------------------------------------------------------------

#define ARG_IMMED 0x20
#define ARG_REG   0x40
#define ARG_RAM   0x80

//-----------------------------------------------------------------------------

enum CMD_CODES_
{
    CMD_HLT_     = 0,
    CMD_PUSH_    = 1  + ARG_IMMED,
    CMD_ADD_     = 2,
    CMD_SUB_     = 3,
    CMD_MUL_     = 4,
    CMD_DIV_     = 5,
    CMD_OUT_     = 6,
    CMD_DUMP_    = 7,
    CMD_RG_PUSH_ = 1  + ARG_REG,
    CMD_RM_PUSH_ = 1  + ARG_RAM,
    CMD_JBE_     = 10 + ARG_IMMED,
    CMD_JAE_     = 11 + ARG_IMMED,
    CMD_JA_      = 12 + ARG_IMMED,
    CMD_JB_      = 13 + ARG_IMMED,
    CMD_JE_      = 14 + ARG_IMMED,
    CMD_JNE_     = 15 + ARG_IMMED,
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
    {CMD_HLT_,      0},
    {CMD_PUSH_,     1},
    {CMD_ADD_,      0},
    {CMD_SUB_,      0},
    {CMD_MUL_,      0},
    {CMD_DIV_,      0},
    {CMD_OUT_,      0},
    {CMD_DUMP_,     0},
    {CMD_RG_PUSH_,  3},
    {CMD_RM_PUSH_,  3},
    {CMD_JBE_,      2},
    {CMD_JAE_,      2},
    {CMD_JB_,       2},
    {CMD_JA_,       2},
    {CMD_JE_,       2},
    {CMD_JNE_,      2},
};

//-----------------------------------------------------------------------------

typedef struct Cpu_data_
{
    int *regs;
    double *ram;

    int *labels;
    double *code;
} Cpu_data_;

//-----------------------------------------------------------------------------

void processor       ();

void Cpu_data_ctor   (Cpu_data_ *data);

void code_dump       (Cpu_data_ data, int size, int32_t code_sgntr);

void read_label_file (FILE *label_file_, Cpu_data_ *data);

void read_code_file  (FILE *code_file_, Cpu_data_ *data);

void calculator      (Stack *stk_, Cpu_data_ data, FILE *file_log);

void label_dump      (Cpu_data_ data, int size);

void fill_code_array (FILE *code_file_, int res_sum_, Cpu_data_ *data);

bool is_equal        (double a, double b);

void handle_cmds     (Stack *stk, int cmd_d, double arg_d, int *ipp, Cpu_data_ data, FILE *file_log);

void free_Cpu_info   (Cpu_data_ *data);

//-----------------------------------------------------------------------------

#endif //CPU_H
