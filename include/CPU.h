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
    RAM_SIZE    = 3,
    STOP        = -1,
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
    FILE *file_out;
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
    void (*func) (CMD_FUNCT);
} Processor;

//-----------------------------------------------------------------------------

int  processor       ();

int processor_ctor   (Processor *Cpu, void (*funct) (CMD_FUNCT));

int  cpu_info_ctor   (Cpu_info *Info);

void processor_dtor  (Processor *Cpu);

void cpu_info_dtor   (Cpu_info *Info);

void read_files      (Processor *Cpu);

void read_label_file (Processor *Cpu);

void read_code_file  (Processor *Cpu);

void handle_cmds     (Processor *Cpu);

void fill_code_array (int res_sum, Processor *Cpu);

bool is_equal        (double a, double b);

void calculator      (int cmd_d, double arg_d, int *ipp, Processor *Cpu);

void cpu_dump        (Processor *Cpu);

void print_num_dmp   (FILE *file, int pos);

//-----------------------------------------------------------------------------

#endif //CPU_H
