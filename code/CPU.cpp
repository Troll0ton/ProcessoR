#include "../include/CPU.h"

//-----------------------------------------------------------------------------

void processor ()
{
    Cpu_data_ *Cpu_data = (Cpu_data_*) calloc (1, sizeof (Cpu_data_));
    Cpu_data_ctor (Cpu_data);

    FILE *code_file  = fopen ("../files/code.bin",   "rb");
    FILE *label_file = fopen ("../files/labels.bin", "rb");
    FILE *log_file   = fopen ("../dump/log.txt",     "w+");

    Stack stk1 = { 0 };
    stack_ctor (&stk1, 2);

    read_label_file (label_file, Cpu_data);

    read_code_file  (code_file,  Cpu_data);

    calculator (&stk1, *Cpu_data, log_file);

    stack_dtor (&stk1);

    fclose (code_file);
    fclose (label_file);
    fclose (log_file);
}

//-----------------------------------------------------------------------------

void Cpu_data_ctor (Cpu_data_ *data) //?
{
    data->regs = (int*) calloc (5, sizeof (int));
    // regs[RAX]
    // rax = 0
    data->regs[0] = 99;   // 000??
    data->regs[1] = 98;
    data->regs[2] = 97;
    data->regs[3] = 96;
    data->regs[4] = 95;

    data->ram =  (double*) calloc (3, sizeof (double));
    data->ram[0] = 999;  // ??
    data->ram[1] = 999;
    data->ram[2] = 999;

    data->labels = NULL;
    data->code   = NULL;
}

//-----------------------------------------------------------------------------

void read_label_file (FILE *label_file_, Cpu_data_ *data)
{
    int res_lab = -1;

    fread (&res_lab, sizeof(int), 1, label_file_);

    data->labels = (int*) calloc (res_lab + 1, sizeof (int));

    data->labels[0] = res_lab; // res_label ??

    fread (data->labels + 1, sizeof(int), res_lab, label_file_);

    label_dump (data->labels, res_lab);
}

//-----------------------------------------------------------------------------

void read_code_file (FILE *code_file_, Cpu_data_ *data)
{
    double res_sum    = -1;
    double code_sgntr = -1;

    fread (&res_sum,    sizeof(double), 1, code_file_);
    fread (&code_sgntr, sizeof(double), 1, code_file_);

    data->code = (double*) calloc (res_sum, sizeof (double));

    if(code_sgntr == CORCT_SIGN)
    {
        fill_code_array (code_file_, res_sum, data);

        code_dump (data->code - 1, res_sum, code_sgntr);
    }
}

//-----------------------------------------------------------------------------

// 5 + 3 ?

void calculator (Stack *stk_, Cpu_data_ data, FILE *file_log)
{
    for(int ip = 1; ip <= (int) data.code[0]; ip++)
    {
        int cmd_d = data.code[ip];
        double arg_d = 0;
        int pos = 0;

        if(cmd_d & MASK_REG)
        {
            arg_d += data.regs[(int) data.code[ip + 1]];
            pos++;
        }

        if(cmd_d & MASK_IMM) arg_d += data.code[ip + 1 + pos];  // ??

        if(cmd_d & MASK_RAM) arg_d =  data.ram[(int) arg_d];

        handle_cmds (stk_, cmd_d & MASK_CMD, arg_d, &ip, data, file_log);
    }
}

//-----------------------------------------------------------------------------

void fill_code_array (FILE *code_file_, int res_sum_, Cpu_data_ *data)
{
    data->code[0] = res_sum_;

    fread (data->code + 1, sizeof(double), res_sum_ - 1, code_file_);
}

//-----------------------------------------------------------------------------

bool is_equal (double a, double b)
{
    const double EPS = 1e-1;

    return (a - b < EPS && a - b > -EPS);
}

//-----------------------------------------------------------------------------

void handle_cmds (Stack *stk, int cmd_d, double arg_d, int *ipp, Cpu_data_ data, FILE *file_log)
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

    stack_dumps (stk, file_log);

    *ipp = ip;
}

//-----------------------------------------------------------------------------

void free_Cpu_info (Cpu_data_ *data)
{
    free (data->labels);
    free (data->code);
    free (data->regs);
    free (data->ram);
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


