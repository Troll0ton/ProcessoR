//! @file CPU.h

#ifndef   CPU_H
#define   CPU_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../include/common.h"

//-----------------------------------------------------------------------------

enum CMD_CODES_
{
    // #include ???? code generation
    CMD_HLT_,
    CMD_PUSH_,
    CMD_POP_,
    CMD_ADD_,
    CMD_SUB_,
    CMD_MUL_,
    CMD_DIV_,
    CMD_OUT_,
    CMD_DUMP_,
    CMD_JBE_,
    CMD_JAE_,
    CMD_JA_,
    CMD_JB_,
    CMD_JE_,
    CMD_JNE_,
};

//-----------------------------------------------------------------------------

typedef struct Cpu_data_
{
    int    *regs;
    double *ram;

    int    *labels;
    double *code;
} Cpu_data_;

//-----------------------------------------------------------------------------

void processor       ();

void Cpu_data_ctor   (Cpu_data_ *data);

void code_dump       (double *code, int size, int32_t code_sgntr);

void read_label_file (FILE *label_file_, Cpu_data_ *data);

void read_code_file  (FILE *code_file_, Cpu_data_ *data);

void calculator      (Stack *stk_, Cpu_data_ data, FILE *file_log);

void label_dump      (int *labels, int size);

void fill_code_array (FILE *code_file_, int res_sum_, Cpu_data_ *data);

bool is_equal        (double a, double b);

void handle_cmds     (Stack *stk, int cmd_d, double arg_d, int *ipp, Cpu_data_ data, FILE *file_log);

void free_Cpu_info   (Cpu_data_ *data);

//-----------------------------------------------------------------------------

#endif //CPU_H
