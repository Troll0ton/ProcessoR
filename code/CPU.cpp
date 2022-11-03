#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int processor ()
{
    Processor Cpu = { 0 };

    if(processor_ctor (&Cpu, calculator) == ERR_CTOR)
    {
        return ERR_CPU;
    }

    read_files (&Cpu);

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

    Cpu->regs = (int*)    calloc (5, sizeof (int));
    Cpu->ram  = (double*) calloc (3, sizeof (double));

    return (cpu_info_ctor (&Cpu->Info));
}

//-----------------------------------------------------------------------------

int cpu_info_ctor (Cpu_info *Info)
{
    Info->code_file  = fopen ("../files/code.bin",   "rb");
    Info->label_file = fopen ("../files/labels.bin", "rb");
    Info->file_out   = fopen ("../dump/log.txt",     "w+");

    if(Info->label_file == NULL ||
       Info->code_file  == NULL ||
       Info->file_out   == NULL   )
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
    free (Cpu->labels);
    free (Cpu->code);

    cpu_info_dtor (&Cpu->Info);
}

//-----------------------------------------------------------------------------

void cpu_info_dtor (Cpu_info *Info)
{
    fclose (Info->code_file);
    fclose (Info->label_file);
    fclose (Info->file_out);
}

//-----------------------------------------------------------------------------

void read_files (Processor *Cpu)
{
    read_label_file (Cpu);

    read_code_file  (Cpu);
}

//-----------------------------------------------------------------------------

void read_label_file (Processor *Cpu)
{
    int res_label = -1;

    fread (&res_label, sizeof(int), 1, Cpu->Info.label_file);

    Cpu->labels = (int*) calloc (res_label + 1, sizeof (int));

    Cpu->labels[0] = res_label;

    fread (Cpu->labels + 1, sizeof(int), res_label, Cpu->Info.label_file);
}

//-----------------------------------------------------------------------------

void read_code_file (Processor *Cpu)
{
    double res_sum = -1;
    double code_sgntr = -1;

    fread (&res_sum,    sizeof(double), 1, Cpu->Info.code_file);
    fread (&code_sgntr, sizeof(double), 1, Cpu->Info.code_file);

    Cpu->code = (double*) calloc (res_sum, sizeof (double));

    if(code_sgntr == CORCT_SIGN)
    {
        fill_code_array (res_sum, Cpu);
    }
}

//-----------------------------------------------------------------------------

void handle_cmds (Processor *Cpu)
{
    for(int ip = 1; ip < (int) Cpu->code[0] - 1; ip++)
    {
        int    cmd_d = Cpu->code[ip];
        int    pos   = 0;
        double arg_d = 0;

        if(cmd_d & MASK_REG)
        {
            arg_d += Cpu->regs[(int) Cpu->code[ip + 1]];
            pos++;
        }

        if(cmd_d & MASK_IMM)
        {
            arg_d += Cpu->code[ip + 1 + pos];
        }

        if(cmd_d & MASK_RAM)
        {
            arg_d = Cpu->ram[(int) arg_d];
        }

        if(cmd_d & MASK_RAM &&
           cmd_d & MASK_IMM &&
           cmd_d & MASK_REG   ) pos = 1;

        else pos = 0;

        Cpu->func (cmd_d, arg_d, &ip, Cpu);
        ip += pos;
    }
}

//-----------------------------------------------------------------------------

void fill_code_array (int res_sum, Processor *Cpu)
{
    Cpu->code[0] = res_sum;

    fread (Cpu->code + 1, sizeof(double), res_sum - 1, Cpu->Info.code_file);
}

//-----------------------------------------------------------------------------

bool is_equal (double a, double b)
{
    const double EPS = 1e-1;

    return (a - b < EPS && a - b > -EPS);
}

//-----------------------------------------------------------------------------

void calculator (int cmd_d, double arg_d, int *ipp, Processor *Cpu)
{
    int ip = *ipp;

    cmd_d &= MASK_CMD;

    double f1 = -1;
    double f2 = -1;

    #define CMD_DEF(cmd, name, code, ...) \
        case cmd:                   \
            code                    \
            __VA_ARGS__             \
            break;

    switch (cmd_d)
    {
        #include "../include/codegen.h"

        default:
            printf ("?%d \n", cmd_d);
            break;
    }

    #undef CMD_DEF

    stack_dumps (&Cpu->Stk, Cpu->Info.file_out);

    *ipp = ip;
}

//-----------------------------------------------------------------------------

void cpu_dump (Processor *Cpu)
{
    FILE *code_dmp_file  = fopen ("../dump/code_dump.txt", "w+");
    FILE *label_dmp_file = fopen ("../dump/label_dump.txt", "w+");

    for(int i = 0; i < Cpu->code[0] - 1; i++)
    {
        print_num_dmp (code_dmp_file, i);

        fprintf (code_dmp_file, "%lg\n", Cpu->code[i]);
    }

    for(int i = 0; i <= Cpu->labels[0]; i++)
    {
        print_num_dmp (label_dmp_file, i);

        fprintf (label_dmp_file, "%d\n", Cpu->labels[i]);
    }

    fclose  (code_dmp_file);
    fclose  (label_dmp_file);
}

//-----------------------------------------------------------------------------

void print_num_dmp (FILE *file, int pos)
{
    int num_nul = 0;

    int pow = 1;

    while(pos / pow != 0)
    {
        pow *= 10;
        num_nul++;
    }

    if(num_nul == 0) num_nul++;

    for(int j = 0; j < 5 - num_nul; j++)
    {
        fprintf (file, "0");
    }

    fprintf (file, "%d || ", pos);
}

//-----------------------------------------------------------------------------



