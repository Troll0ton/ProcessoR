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
    CMD_MAX_LEN      = 20,
    CODE_SIZE_INIT   = 100,
    LABEL_SIZE_INIT  = 50,
    LIMIT_DIFFERENCE = 10,
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
    bool    double_pass;
} Asm_info;

//-----------------------------------------------------------------------------

typedef struct Command
{
    bool  flag;
    int   mask;
    int   number;
} Command;

//-----------------------------------------------------------------------------

typedef struct Argument
{
    bool   flag;
    double value;
    char   reg_sym;
    int    amount;
} Argument;

//-----------------------------------------------------------------------------

typedef struct Code_array
{
    double *array;
    int     size;
    int     capacity;
} Code_array;

//-----------------------------------------------------------------------------

typedef struct Label_array
{
    int *array;
    int  size;
    int  capacity;
} Label_array;

//-----------------------------------------------------------------------------

typedef struct Assembler
{
    Asm_info    Info;
    Line        Cur_line;
    int         cur_pos;
    int         offset;
    Code_array  Code;
    Label_array Label;
} Assembler;

//-----------------------------------------------------------------------------

int  assembler_ctor (Assembler *Asm, char *argv[]);

int  asm_info_ctor  (Asm_info *Info, char *argv[]);

void assembling     (Assembler *Asm);

void handle_text    (Assembler *Asm, Line *Text, File *File_input);

void parse_line     (Assembler *Asm, Command *Cmd, Argument *Arg);

void parse_label    (Assembler *Asm, Argument *Arg);

void parse_cmd      (Assembler *Asm, Command *Cmd, Argument *Arg);

void parse_arg      (Assembler *Asm, Command *Cmd, Argument *Arg);

void write_in_code  (Assembler *Asm, Command Cmd, Argument Arg);

void assembler_dtor (Assembler *Asm);

void asm_info_dtor  (Asm_info *Info);

void write_res_sums (Assembler *Asm);

void asm_dump       (Assembler *Asm);


//-----------------------------------------------------------------------------

#endif //ASM_H
