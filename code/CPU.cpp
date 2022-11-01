#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int processor ()
{
    Processor Cpu = { 0 };

    if(processor_ctor (&Cpu) == ERR_CTOR)
    {
        return ERR_CPU;
    }

    read_files (&Cpu);

    calculator (&Cpu);

    processor_dtor (&Cpu);

    return 0;
}

//-----------------------------------------------------------------------------

int processor_ctor (Processor *Cpu)
{
    Cpu->Info = { 0 };

    Cpu->Stk = { 0 };
    stack_ctor (&Cpu->Stk, 2);

    Cpu->regs = (int*) calloc (5, sizeof (int));

    for(int rx = 0; rx < num_of_regs; rx++)
    {
        Cpu->regs[rx] = 0;
    }

    Cpu->ram = (double*) calloc (3, sizeof (double));

    for(int i = 0; i < ram_size; i++)
    {
        Cpu->ram[i] = 0;
    }

    return (cpu_info_ctor (&Cpu->Info));
}

//-----------------------------------------------------------------------------

int cpu_info_ctor (Cpu_info *Info)
{
    Info->code_file  = fopen ("../files/code.bin",   "rb");
    Info->label_file = fopen ("../files/labels.bin", "rb");
    Info->log_file   = fopen ("../dump/log.txt",     "w+");

    if(Info->label_file == NULL ||
       Info->code_file  == NULL ||
       Info->log_file   == NULL   )
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
    fclose (Info->log_file);
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

    label_dump (Cpu->labels, res_label);
}

//-----------------------------------------------------------------------------

void read_code_file (Processor *Cpu)
{
    double res_sum    = -1;
    double code_sgntr = -1;

    fread (&res_sum,    sizeof(double), 1, Cpu->Info.code_file);
    fread (&code_sgntr, sizeof(double), 1, Cpu->Info.code_file);

    Cpu->code = (double*) calloc (res_sum, sizeof (double));

    if(code_sgntr == CORCT_SIGN)
    {
        fill_code_array (res_sum, Cpu);

        code_dump (Cpu->code, res_sum - 1, code_sgntr);
    }
}

//-----------------------------------------------------------------------------

void calculator (Processor *Cpu)
{
    for(int ip = 1; ip <= (int) Cpu->code[0]; ip++)
    {
        int cmd_d = Cpu->code[ip];
        double arg_d = 0;
        int pos = 0;

        if(cmd_d & MASK_REG)
        {
            arg_d += Cpu->regs[(int) Cpu->code[ip + 1]];
            pos++;
        }

        if(cmd_d & MASK_IMM)
        {
            arg_d += Cpu->code[ip + 1 + pos];

            if(cmd_d & MASK_REG) pos = 1;
        }

        if(cmd_d & MASK_RAM) arg_d =  Cpu->ram[(int) arg_d];

        handle_cmds (cmd_d & MASK_CMD, arg_d, &ip, Cpu);
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

void handle_cmds (int cmd_d, double arg_d, int *ipp, Processor *Cpu)
{
    int ip = *ipp;

    double f1 = -1;
    double f2 = -1;

    #define CMD_(cmd, code, ...)   \
        case cmd:                  \
            code                   \
            __VA_ARGS__            \
            break;

    switch (cmd_d)
    {
        #include "../include/codegen.h"

        default:
            printf ("?%d \n", cmd_d);
            break;
    }

    #undef CMD_

    stack_dumps (&Cpu->Stk, Cpu->Info.log_file);

    *ipp = ip;
}

// BAN!
//-----------------------------------------------------------------------------

void code_dump (double *code, int size, int32_t code_sgntr)
{
    FILE *code_dmp_file  = fopen ("../dump/code_dump.txt", "w+");

    fprintf (code_dmp_file,
             "\n________________________CODE_DUMP__________________________\n\n"
             "|RES SUM|   - %d\n"
             "|Signature| - %x\n", size, code_sgntr);

    for(int i = 2; i < size; i++)
    {
        int num_nul = 0;

        int pow = 1;

        while(i / pow != 0)
        {
            pow *= 10;
            num_nul++;
        }

        if(num_nul == 0) num_nul++;

        for(int j = 0; j < 5 - num_nul; j++)
        {
            fprintf (code_dmp_file, "0");
        }

        fprintf (code_dmp_file, "%d || %lg\n", i, code[i]);
    }

    fprintf (code_dmp_file, "___________________________________________________________\n\n");

    fclose  (code_dmp_file);
}

//-----------------------------------------------------------------------------

void label_dump (int *labels, int size)
{
    FILE *label_dmp_file  = fopen ("../dump/label_dump.txt", "w+");

    fprintf (label_dmp_file,
             "\n________________________LABEL_DUMP__________________________\n\n"
             "|RES SUM|   - %d\n", labels[0]);

    for(int i = 1; i <= size; i++)
    {
        int num_nul = 0;

        int pow = 1;

        while(i / pow != 0)
        {
            pow *= 10;
            num_nul++;
        }

        if(num_nul == 0) num_nul++;

        for(int j = 0; j < 5 - num_nul; j++)
        {
            fprintf (label_dmp_file, "0");
        }

        fprintf (label_dmp_file, "%d || %d\n", i, labels[i]);
    }

    fprintf (label_dmp_file, "____________________________________________________________\n\n");

    fclose  (label_dmp_file);
}

//-----------------------------------------------------------------------------


