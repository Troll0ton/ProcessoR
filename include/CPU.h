//! @file CPU.h

#ifndef   CPU_H
#define   CPU_H

//-----------------------------------------------------------------------------

#include "../include/stack.h"
#include "../include/common.h"

//-----------------------------------------------------------------------------

enum Nums
{
    NUM_OF_REGS = 5,
    RAM_SIZE = 3,
};

//-----------------------------------------------------------------------------

enum Regs
{
    RAX,
    RBX,
    RCX,
    RDX,
    REX,
};

//-----------------------------------------------------------------------------

typedef struct Cpu_info
{
    FILE *code_file;
    FILE *label_file;
    FILE *log_file;
} Cpu_info;

//-----------------------------------------------------------------------------

typedef struct Processor
{
    Cpu_info Info;
    int     *regs;
    int     *labels;
    double  *ram;
    double  *code;
    Stack    Stk;
} Processor;

//-----------------------------------------------------------------------------

int  processor       ();

int  processor_ctor  (Processor *Cpu);

int  cpu_info_ctor   (Cpu_info *Info);

void processor_dtor  (Processor *Cpu);

void cpu_info_dtor   (Cpu_info *Info);

void read_files      (Processor *Cpu);

void read_label_file (Processor *Cpu);

void read_code_file  (Processor *Cpu);

void calculator      (Processor *Cpu);

void fill_code_array (int res_sum, Processor *Cpu);

bool is_equal        (double a, double b);

void handle_cmds     (int cmd_d, double arg_d, int *ipp, Processor *Cpu);

void cpu_dump        (Processor *Cpu);

void print_num_dmp   (FILE *file, int pos);

//-----------------------------------------------------------------------------

#endif //CPU_H
