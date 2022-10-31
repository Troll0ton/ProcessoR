//! @file ASM.H

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"

//-----------------------------------------------------------------------------

#define NUM_SUP_CMD 10
#define NUM_OF_JMPS 6

#define CORCT_SIGN 0xBACAFE

#define MASK_IMM 0x20
#define MASK_REG   0x40
#define MASK_RAM   0x80

//-----------------------------------------------------------------------------

enum CMD_CODES_
{
    CMD_HLT_     = 0,
    CMD_PUSH_    = 1  + MASK_IMM,
    CMD_ADD_     = 2,
    CMD_SUB_     = 3,
    CMD_MUL_     = 4,
    CMD_DIV_     = 5,
    CMD_OUT_     = 6,
    CMD_DUMP_    = 7,
    CMD_RG_PUSH_ = 1  + MASK_REG,
    CMD_RM_PUSH_ = 1  + MASK_RAM,
    CMD_JBE_     = 10 + MASK_IMM,
    CMD_JAE_     = 11 + MASK_IMM,
    CMD_JA_      = 12 + MASK_IMM,
    CMD_JB_      = 13 + MASK_IMM,
    CMD_JE_      = 14 + MASK_IMM,
    CMD_JNE_     = 15 + MASK_IMM,
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
