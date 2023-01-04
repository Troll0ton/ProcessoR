//! @file DASM.h

#ifndef   DASM_H
#define   DASM_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../../COMMON/include/common.h"

//-----------------------------------------------------------------------------

#define print_file(...) fprintf (Dasm->Info.file_out, __VA_ARGS__)

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
    char     *code;
    int       code_size;
    Stack     Stk;
} Disassember;

//-----------------------------------------------------------------------------

void read_code_file   (Disassember *Dasm);

int  disassember_ctor (Disassember *Dasm);

int  dasm_info_ctor   (Dasm_info *Info);

void disassember_dtor (Disassember *Dasm);

void dasm_info_dtor   (Dasm_info *Info);

void disassembling    (Disassember *Dasm);

bool is_equal         (double a, double b);

//-----------------------------------------------------------------------------

#endif //DASM_H

