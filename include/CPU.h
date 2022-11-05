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
    FILE *file_out;
} Cpu_info;

//-----------------------------------------------------------------------------

typedef struct Processor
{
    Cpu_info Info;
    double  *regs;
    double  *ram;
    double  *code;
    int      code_size;
    Stack    Stk;
    void (*func) (CMD_FUNCT);
} Processor;

//-----------------------------------------------------------------------------

int  processor_ctor  (Processor *Cpu, void (*funct) (CMD_FUNCT));

int  cpu_info_ctor   (Cpu_info *Info);

void processor_dtor  (Processor *Cpu);

void cpu_info_dtor   (Cpu_info *Info);

void read_code_file  (Processor *Cpu);

void handle_cmds     (Processor *Cpu);

bool is_equal        (double a, double b);

void calculator      (int curr_cmd, double curr_arg, int *curr_ptr, Processor *Cpu);

void cpu_dump        (Processor *Cpu);

//-----------------------------------------------------------------------------

#endif //CPU_H
