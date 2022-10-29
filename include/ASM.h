//! @file ASM.h

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/lines_handle.h"
#include "../include/input_output.h"

//-----------------------------------------------------------------------------

#include <stdint.h>

//-----------------------------------------------------------------------------

#define Num_sup_cmd  10
#define Num_sup_jmps 6
#define Max_cmd_size 15
#define Arg_max_len  100
#define Jump_num     10
#define Push_len     6

// FAILED_OPEN
#define Failed_open  1
#define Error_asm    1


// common.h

#define Mask_immed   0x20
#define Mask_reg     0x40
#define Mask_ram     0x80

//-----------------------------------------------------------------------------

#define Cor_signature   0xBACAFE
#define Incor_signature 0xDEADAC

//-----------------------------------------------------------------------------

enum CMD_CODES
{
    // #include ???? code generation
    CMD_HLT,
    CMD_PUSH,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_OUT,
    CMD_DUMP,
    CMD_RG_PUSH,
    CMD_RM_PUSH,
    CMD_JBE,
    CMD_JAE,
    CMD_JA,
    CMD_JB,
    CMD_JE,
    CMD_JNE,
};

//-----------------------------------------------------------------------------

typedef struct Cmd
{
    char *name;
    int   num;
    char  par;
} Cmd;

//-----------------------------------------------------------------------------

typedef struct Asm_info_
{
    FILE   *file_in;     //
    FILE   *code_file;   //
    FILE   *label_file;  //
    Line    Cur_line;
    int     res_sum;
    int32_t code_sgntr;
    int     num_of_labels;
    // Asm_info
} Asm_info_;  // _

//-----------------------------------------------------------------------------

const Cmd Cmd_asm[] =
{
    {"hlt",  CMD_HLT,                0},
    {"push", CMD_PUSH + Mask_immed,  1},
    {"add",  CMD_ADD,                0},
    {"sub",  CMD_SUB,                0},
    {"mul",  CMD_MUL,                0},
    {"div",  CMD_DIV,                0},
    {"out",  CMD_OUT,                0},
    {"dump", CMD_DUMP,               0},
    {"push", CMD_PUSH + Mask_reg,    3},
    {"push", CMD_PUSH + Mask_ram,    4},
    {"jbe",  CMD_JBE  + Mask_immed,  3},
    {"jae",  CMD_JAE  + Mask_immed,  3},
    {"ja",   CMD_JB   + Mask_immed,  2},
    {"jb",   CMD_JA   + Mask_immed,  2},
    {"je",   CMD_JE   + Mask_immed,  2},
    {"jne",  CMD_JNE  + Mask_immed,  3},
};

//-----------------------------------------------------------------------------

int  assembling        (char *argv[]);

void label_utility     (Asm_info_ *data, char *cmd);

void handle_label      (Asm_info_ *data);

void handle_jump       (Asm_info_ *data, char *cmd);

bool found_label       (Asm_info_ *data);

bool found_arg_funct   (char *cmd);

void handle_arg_functs (Asm_info_ *data);

void handle_regs       (Asm_info_ *data, char *arg_);

void handle_ram_args   (Asm_info_ *data, char *arg_);

void handle_com_functs (Asm_info_ *data, char *cmd);

void Asm_info_ctor     (Asm_info_ *data); // Asm_info_ctor

void files_ctor        (Asm_info_ *data);

void write_res_sums    (Asm_info_ *data);

void close_files       (Asm_info_ *data);

int  open_files        (Asm_info_ *data, char *argv[]);

void handle_lines      (Asm_info_ *data, char *cmd);

//-----------------------------------------------------------------------------

#endif //ASM_H
