//! @file DASM.h

#ifndef   DASM_H
#define   DASM_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../include/common.h"

//-----------------------------------------------------------------------------

#define Struct Disassember
#define Name   Dasm

//-----------------------------------------------------------------------------

typedef struct Dasm_info
{
    FILE *code_file;
    FILE *file_out;
} Dasm_info;

//-----------------------------------------------------------------------------

typedef struct Disassember
{
    Dasm_info Info;
    double   *regs;
    double   *ram;
    double   *code;
    int       code_size;
    Stack     Stk;
    void (*func) (CMD_FUNCT);
} Disassember;

//-----------------------------------------------------------------------------

void disassembling    (int curr_cmd, double curr_arg, int *curr_ptr, Disassember *Cpu);

void read_code_file   (Disassember *Cpu);

int  disassember_ctor (Disassember *Dasm, void (*funct) (CMD_FUNCT));

int  dasm_info_ctor   (Dasm_info *Info);

void disassember_dtor (Disassember *Dasm);

void dasm_info_dtor   (Dasm_info *Info);

void handle_cmds      (Disassember *Dasm);

bool is_equal         (double a, double b);

//-----------------------------------------------------------------------------

#endif //DASM_H

