//! @file ASM.H

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"

//-----------------------------------------------------------------------------

#define Num_sup_cmd 10
#define Num_sup_jmps 6

#define Cor_signature 0xBACAFE

#define Arg_immed 0x20
#define Arg_reg   0x40
#define Arg_ram   0x80

//-----------------------------------------------------------------------------

enum CMD_CODES_
{
    CMD_HLT_     = 0,
    CMD_PUSH_    = 1  + Arg_immed,
    CMD_ADD_     = 2,
    CMD_SUB_     = 3,
    CMD_MUL_     = 4,
    CMD_DIV_     = 5,
    CMD_OUT_     = 6,
    CMD_DUMP_    = 7,
    CMD_RG_PUSH_ = 1  + Arg_reg,
    CMD_RM_PUSH_ = 1  + Arg_ram,
    CMD_JBE_     = 10 + Arg_immed,
    CMD_JAE_     = 11 + Arg_immed,
    CMD_JA_      = 12 + Arg_immed,
    CMD_JB_      = 13 + Arg_immed,
    CMD_JE_      = 14 + Arg_immed,
    CMD_JNE_     = 15 + Arg_immed,
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

void disassembler    ();

void code_dump       (double *code, int size, int32_t code_sgntr);

void read_label_file (FILE *label_file_, int **labels_);

void read_code_file  (FILE *code_file_, double **code_);

void file_maker      (double *code_, int *labels_);

void label_dump      (int *label, int size);

void fill_code_array (FILE *code_file_, int res_sum_, double *code_);

bool is_equal        (double a, double b);

void handle_cmds     (int cmd_d, double arg_d, int *ipp, FILE *file_out);

void label_input     (int ip, int *labels, FILE *file_out);

//-----------------------------------------------------------------------------

#endif //ASM_H
