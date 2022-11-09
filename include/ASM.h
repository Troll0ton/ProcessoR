//! @file ASM.h

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/lines_handle.h"
#include "../include/input_output.h"
#include "../include/stack.h"
#include "../include/common.h"

//-----------------------------------------------------------------------------

#define SUCCESS_READ 1
#define EMPTY_LINE   0

//-----------------------------------------------------------------------------

// N(SUPPORTED_CMD)

#define N(suffix) NUM_OF_##suffix
// enum CMD_INFO
// {
//     NUM_OF_SUPPORTED_CMD = 15;
//     ...
// }


enum Sizes
{
    // ??
    NUM_OF_SUP_CMD  = 15,
    NUM_OF_JMPS     = 6,
    NUM_OF_CMDS_RAM = 2,
    NUM_OF_CMDS_RGS = 2,


    // ??
    CMD_MAX_LEN     = 20,
    CODE_SIZE_INIT  = 100,
    LABEL_SIZE_INIT = 50,
    LIMIT_DIFRCE    = 10,
};

//-----------------------------------------------------------------------------

enum Numbers
{
    CMD_REG_NUM    = 1,
    CMD_RAM_NUM    = 1,
    CMD_JMP_NUM    = 9,
    // OFFSET_SIGNATURE
    NUM_FRST_EL_CD = 2,
    NUM_FRST_EL_LB = 1,
};

//-----------------------------------------------------------------------------

typedef struct Asm_info
{
    FILE   *file_in;
    FILE   *code_file;
    int     code_sgntr;
    bool    dbl_pass;
} Asm_info;

//-----------------------------------------------------------------------------

typedef struct Command
{
    int   flag_cmd;
    int   mask;
    char  name[CMD_MAX_LEN];
} Command;

//-----------------------------------------------------------------------------

typedef struct Assembler
{
    Asm_info   Info;
    Line       Cur_line;
    // CodeArray code;
    // Asm.code.capacity
    // Asm.label.capacity
    double    *code_array;
    int       *label_array;
    int        cur_pos;
    int        offset;
    int        code_arr_size;
    int        label_arr_size;
    int        code_arr_capct;
    int        label_arr_capct;
} Assembler;

//-----------------------------------------------------------------------------

void assembling     (Assembler *Asm);

int  assembler_ctor (Assembler *Asm, char *argv[]);

int  asm_info_ctor  (Asm_info *Info, char *argv[]);

void handle_line    (Assembler *Asm);

void assembler_dtor (Assembler *Asm);

void asm_info_dtor  (Asm_info *Info);

void write_res_sums (Assembler *Asm);

void parse_label    (Assembler *Asm, int label);

void parse_arg      (Assembler *Asm, Command *Cmd);

void parse_cmd      (Assembler *Asm, Command Cmd);

void parse_jmp      (Assembler *Asm, Command *Cmd, int label);

void handle_text    (Assembler *Asm, Line *Text, File *File_input);

void write_in_arg   (Assembler *Asm, Command *Cmd, double arg_val, int mask);

void asm_dump       (Assembler *Asm);

//-----------------------------------------------------------------------------

#endif //ASM_H
