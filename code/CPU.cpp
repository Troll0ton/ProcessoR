#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    Processor Cpu = { 0 };

    if(processor_ctor (&Cpu, calculator) == ERR_CTOR)
    {
        return ERR_CPU;
    }

    read_code_file (&Cpu);

    cpu_dump (&Cpu);

    handle_cmds (&Cpu);

    processor_dtor (&Cpu);

    return 0;
}

//-----------------------------------------------------------------------------

int processor_ctor (Processor *Cpu, void (*funct) (CMD_FUNCT))
{
    Cpu->Info = { 0 };

    Cpu->func = funct;

    Cpu->Stk = { 0 };
    stack_ctor (&Cpu->Stk, 2);

    // N(REGS)
    // 300
    Cpu->regs = (double*) calloc (5, sizeof (double));
    Cpu->ram  = (double*) calloc (3, sizeof (double));

    if(Cpu->regs == NULL || Cpu->ram == NULL)
    {
        return ERR_CTOR;
    }

    return (cpu_info_ctor (&Cpu->Info));
}

//-----------------------------------------------------------------------------

int cpu_info_ctor (Cpu_info *Info)
{
    Info->code_file = fopen ("../files/code.bin",   "rb");
    Info->file_out  = fopen ("../dump/log.txt",     "w+");

    if(Info->code_file == NULL ||
       Info->file_out  == NULL   )
    {
        return ERR_CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void processor_dtor (Processor *Cpu)
{
    stack_dtor (&Cpu->Stk);

    free (Cpu->regs);
    free (Cpu->ram);
    free (Cpu->code);

    cpu_info_dtor (&Cpu->Info);
}

//-----------------------------------------------------------------------------

void cpu_info_dtor (Cpu_info *Info)
{
    fclose (Info->code_file);
    fclose (Info->file_out);
}


// call :123 [300]
// push 5.5  [308]


// :123  [1300]
// ...
// ret   [1400]
//
//


//-----------------------------------------------------------------------------

void read_code_file (Processor *Cpu)
{
    double res_sum = -1;
    double code_sgntr = -1;

    fread (&res_sum, sizeof(double), 1, Cpu->Info.code_file);
    Cpu->code_size = res_sum - 1;

    fread (&code_sgntr, sizeof(double), 1, Cpu->Info.code_file);
    // one fread ...

    if(code_sgntr == CORCT_SIGN)
    {
        Cpu->code = (double*) calloc (res_sum, sizeof (double));

        if(Cpu->code == NULL)
        {
            printf ("__________|ERROR - NULL pointer code|__________\n");
        }

        Cpu->code[0] = res_sum;

        fread (Cpu->code + 1, sizeof(double), res_sum - 1, Cpu->Info.code_file);
    }

    else
    {
        printf ("__________|WRONG SIGNATURE!|__________\n");
    }
}

//-----------------------------------------------------------------------------

void handle_cmds (Processor *Cpu)
{
    for(int curr_pos = 1; curr_pos < Cpu->code_size; curr_pos++)
    {
        int    curr_cmd = Cpu->code[curr_pos];
        int    offset   = 0;
        double curr_arg = 0;

        if(curr_cmd & MASK_REG)
        {
            curr_arg += Cpu->regs[(int) Cpu->code[curr_pos + 1]];
            offset++;
        }

        if(curr_cmd & MASK_IMM)
        {
            curr_arg += Cpu->code[curr_pos + 1 + offset];
        }

        if(curr_cmd & MASK_RAM)
        {
            curr_arg = Cpu->ram[(int) curr_arg];
        }

        if(curr_cmd & MASK_RAM &&
           curr_cmd & MASK_IMM &&
           curr_cmd & MASK_REG   )
            offset = 1;

        else offset = 0;

        Cpu->func (curr_cmd, curr_arg, &curr_pos, Cpu);
        curr_pos += offset;
    }
}

//-----------------------------------------------------------------------------

bool is_equal (double a, double b)
{
    const double EPS = 1e-1;

    return (a - b < EPS && a - b > -EPS);
}

//-----------------------------------------------------------------------------
// execute_cmd

void calculator (int curr_cmd, double curr_arg, int *curr_ptr, Processor *Cpu)
{
    int curr_pos = *curr_ptr;

    curr_cmd &= MASK_CMD;

    #define CMD_DEF(cmd, name, code, ...) \
        case cmd:                         \
        {                                 \
            code                          \
            __VA_ARGS__                   \
            break;                        \
        }

    switch (curr_cmd)
    {
        #include "../include/codegen.h"

        default:
            printf ("?%d \n", curr_cmd);
            break;
    }

    #undef CMD_DEF

    stack_dumps (&Cpu->Stk, Cpu->Info.file_out);

    *curr_ptr = curr_pos;
}

//-----------------------------------------------------------------------------

void cpu_dump (Processor *Cpu)
{
    FILE *code_dmp_file = fopen ("../dump/code_cpu_dump.txt", "w+");

    for(int i = 0; i < Cpu->code_size; i++)
    {
        fprintf (code_dmp_file, "%06d || %lg\n", i, Cpu->code[i]);
    }

    fclose (code_dmp_file);
}

//-----------------------------------------------------------------------------




