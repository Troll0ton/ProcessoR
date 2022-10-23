//! @file ASM.h

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/lines_handle.h"
#include "../include/input_output.h"

//-----------------------------------------------------------------------------

#define num_sup_cmd 11

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
    CMD_RM_PUSH,
};

//-----------------------------------------------------------------------------

typedef struct Cmd
{
    char *name;
    int num;
    char par;
} Cmd;

//-----------------------------------------------------------------------------

typedef struct Asm_data_
{
    FILE *file_in;
    FILE *code_file;
    FILE *label_file;

    Line Cur_line;

    int res_sum;
    int num_of_labels;
} Asm_data_;

//-----------------------------------------------------------------------------

const Cmd Cmd_asm[] =
{
    {"hlt",  CMD_HLT,  0},
    {"push", CMD_PUSH, 1},
    {"add",  CMD_ADD,  0},
    {"sub",  CMD_SUB,  0},
    {"mul",  CMD_MUL,  0},
    {"div",  CMD_DIV,  0},
    {"out",  CMD_OUT,  0},
    {"dump", CMD_DUMP, 0},
    {"jump", CMD_JUMP, 2},
    {"push", 0x11    , 3},
    {"push", 0x41    , 4},
};

//-----------------------------------------------------------------------------

void assembler         ();

void label_utility     (Asm_data_ *data, char *cmd_);

void handle_label      (Asm_data_ *data);

void handle_jump       (Asm_data_ *data);

bool found_label       (Asm_data_ *data);

bool found_arg_funct   (char *cmd_);

void handle_arg_functs (Asm_data_ *data);

void handle_regs       (Asm_data_ *data, char *arg_);

void handle_ram_args   (Asm_data_ *data, char *arg_);

void handle_com_functs (Asm_data_ *data, char *cmd_);

void Asm_data_ctor     (Asm_data_ *data);

void codefile_ctor     (Asm_data_ *data);

//-----------------------------------------------------------------------------

#endif //ASM_H
