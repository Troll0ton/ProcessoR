//! @file ASM.h

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/lines_handle.h"
#include "../include/input_output.h"
#include "../include/common.h"

//-----------------------------------------------------------------------------

#include <stdint.h>

//-----------------------------------------------------------------------------

#define NUM_SUP_CMD  15
#define NUM_OF_JMPS 6
#define NUM_OF_CMDS_RGS 2

#define MASK_CMD_RGS 1
#define MASK_CMD_RAM 1
#define NUM_OF_CMDS_RAM 2
#define NUM_OF_CMDS_RGS 2

#define MAX_LEN 20
#define MASK_JUMP 9
#define Arg_max_len  100

//-----------------------------------------------------------------------------

enum CMD_CODES
{
    // #include ???? code generation
    CMD_HLT,
    CMD_PUSH,
    CMD_POP,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_OUT,
    CMD_DUMP,
    CMD_JBE,
    CMD_JAE,
    CMD_JA,
    CMD_JB,
    CMD_JE,
    CMD_JNE,
};

//-----------------------------------------------------------------------------

enum ERROR_CODE
{
    ERR_CTOR = 1,
    ERR_ASM  = 2,
};

//-----------------------------------------------------------------------------

typedef struct Asm_info
{
    FILE   *file_in;
    FILE   *code_file;
    FILE   *label_file;
    int32_t code_sgntr;
} Asm_info;

//-----------------------------------------------------------------------------

typedef struct Input_opts
{
    char   cmd[MAX_LEN];
    double val_dbl;
    int    val_int;
    char   reg_sym;
    int    mask;
    int    type;
} Input_opts;

//-----------------------------------------------------------------------------

typedef struct Assembler
{
    Asm_info   Info;
    Input_opts Opts;
    double    *code_array;
    int       *label_array;
    Line       Cur_line;
    int        code_arr_size;
    int        label_arr_size;
    int        code_arr_capct;
    int        label_arr_capct;
} Assembler;

//-----------------------------------------------------------------------------

int  assembling      (char *argv[]);

int  assembler_ctor  (Assembler *Asm, char *argv[]);

int  asm_info_ctor   (Asm_info *Info, char *argv[]);

void fill_asm_arrays (Assembler *Asm);

void handle_line     (Assembler *Asm);

void files_ctor      (Assembler *Asm);

void assembler_dtor  (Assembler *Asm);

void asm_info_dtor   (Asm_info *Info);

void write_res_sums  (Assembler *Asm);

void asm_opts_ctor   (Assembler *Asm);

void parse_label     (Assembler *Asm);

void parse_arg       (Assembler *Asm);

void parse_cmd       (Assembler *Asm);

int  line_empty      (Assembler *Asm);

//-----------------------------------------------------------------------------

#endif //ASM_H
