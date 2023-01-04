//! @file dasm.h

#ifndef   dasm_H
#define   dasm_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../../COMMON/include/common.h"

//-----------------------------------------------------------------------------

#define print_file(...) fprintf (dasm->info.file_out, __VA_ARGS__)

//-----------------------------------------------------------------------------

typedef struct Dasm_info
{
    FILE *code_file;
    FILE *file_out;
} Dasm_info;

//-----------------------------------------------------------------------------

typedef struct Disassember
{
    Dasm_info info;
    char     *code;
    int       code_size;
    Stack     Stk;
} Disassember;

//-----------------------------------------------------------------------------

void read_code_file   (Disassember *dasm);

int  disassember_ctor (Disassember *dasm);

int  Dasm_info_ctor   (Dasm_info *info);

void disassember_dtor (Disassember *dasm);

void Dasm_info_dtor   (Dasm_info *info);

void disassembling    (Disassember *dasm);

bool is_equal         (double a, double b);

//-----------------------------------------------------------------------------

#endif //dasm_H

