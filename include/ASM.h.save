//! @file ASM.h

#ifndef   ASM_H
#define   ASM_H

//-----------------------------------------------------------------------------

#include "../include/lines_handle.h"
#include "../include/input_output.h"
#include "../include/stack.h"
#include "../include/common.h"

//-----------------------------------------------------------------------------

enum Sizes
{
    NUM_OF_SUP_CMD = 15,
    NUM_OF_JMPS = 6,
    NUM_OF_CMDS_RAM = 2,
    NUM_OF_CMDS_RGS = 2,
    CMD_MAX_LEN = 20,
    CODE_SIZE_INIT = 100,
    LABEL_SIZE_INIT = 50,
};

//-----------------------------------------------------------------------------

enum Numbers
{
    CMD_REG_NUM = 1,
    CMD_RAM_NUM = 1,
    CMD_JMP_NUM = 9,
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

typedef struct Input_pars
{
    char   cmd[CMD_MAX_LEN];
    double val_dbl;
    int    val_int;
    char   reg_sym;
    int    mask;
    int    flag_cmd;
    int    num_readed_codes;
} Input_pars;

//-----------------------------------------------------------------------------

typedef struct Assembler
{
    Asm_info   Info;
    Input_pars Pars;
    Line       Cur_line;
    double    *code_array;
    int       *label_array;
    int        code_arr_size;
    int        label_arr_size;
    int        code_arr_capct;
    int        label_arr_capct;
} Assembler;

//-----------------------------------------------------------------------------

int   assembling      (char *argv[]);

int   assembler_ctor  (Assembler *Asm, char *argv[]);

int   asm_info_ctor   (Asm_info *Info, char *argv[]);

void  fill_asm_arrays (Assembler *Asm);

void  handle_line     (Assembler *Asm);

void  files_ctor      (Assembler *Asm);

void  assembler_dtor  (Assembler *Asm);

void  asm_info_dtor   (Asm_info *Info);

void  write_res_sums  (Assembler *Asm);

void  asm_pars_ctor   (Assembler *Asm);

void  parse_label     (Assembler *Asm);

void  parse_arg       (Assembler *Asm);

void  parse_cmd       (Assembler *Asm);

int   line_empty      (Assembler *Asm);

void  write_in_arg    (Assembler *Asm, double val, int mask);

//-----------------------------------------------------------------------------

#endif //ASM_H
