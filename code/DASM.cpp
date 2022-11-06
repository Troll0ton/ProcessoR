#include "../include/DASM.h"

//-----------------------------------------------------------------------------

int main ()
{
    Disassember Dasm = { 0 };

    if(disassember_ctor (&Dasm, disassembling) == ERR_CTOR)
    {
        return ERR_DASM;
    }

    read_code_file (&Dasm);

    handle_cmds (&Dasm);

    disassember_dtor (&Dasm);

    return 0;

    return 0;
}

//-----------------------------------------------------------------------------

void disassembling (int curr_cmd, double curr_arg, int *curr_ptr, Disassember *Dasm)
{
    int cur_pos = *curr_ptr;

    #define CMD_DEF(cmd, name, ...)                    \
        case cmd:                                      \
        {                                              \
            fprintf (Dasm->Info.file_out, "%s ", name); \
            break;                                     \
        }

    switch (curr_cmd & MASK_CMD)
    {
        #include "../include/codegen.h"

        default:
            fprintf (Dasm->Info.file_out, "?%d ", curr_cmd);
            break;
    }

    #undef CMD_DEF

    if(curr_cmd & MASK_RAM &&
       curr_cmd & MASK_IMM &&
       curr_cmd & MASK_REG   )
    {
        fprintf (Dasm->Info.file_out, "[r%cx + %d]", (int) Dasm->code[cur_pos + 1] + 'a', (int) Dasm->code[cur_pos + 2]);
        cur_pos += TWO_ARGS_OFFSET;
    }

    else if(curr_cmd & MASK_REG &&
            curr_cmd & MASK_RAM   )
    {
        fprintf (Dasm->Info.file_out, "[r%cx]", (int) Dasm->code[cur_pos + 1] + 'a');
        cur_pos += BASIC_OFFSET;
    }

    else if(curr_cmd & MASK_IMM &&
            curr_cmd & MASK_RAM   )
    {
        fprintf (Dasm->Info.file_out, "[%d]", (int) Dasm->code[cur_pos + 1]);
        cur_pos += BASIC_OFFSET;
    }

    else if(curr_cmd & MASK_IMM)
    {
        fprintf (Dasm->Info.file_out, "%d", (int) Dasm->code[cur_pos + 1]);
        cur_pos += BASIC_OFFSET;
    }

    else if(curr_cmd & MASK_REG)
    {
        fprintf (Dasm->Info.file_out, "r%cx", (int) Dasm->code[cur_pos + 1] + 'a');
        cur_pos += BASIC_OFFSET;
    }

    fprintf (Dasm->Info.file_out, "\n");

    *curr_ptr = cur_pos;
}

//-----------------------------------------------------------------------------

int disassember_ctor (Disassember *Dasm, void (*funct) (CMD_FUNCT))
{
    Dasm->Info = { 0 };

    Dasm->func = funct;

    Dasm->Stk = { 0 };
    stack_ctor (&Dasm->Stk, 2);

    Dasm->regs = (double*) calloc (5, sizeof (double));
    Dasm->ram  = (double*) calloc (3, sizeof (double));

    if(Dasm->regs == NULL || Dasm->ram == NULL)
    {
        return ERR_CTOR;
    }

    return (dasm_info_ctor (&Dasm->Info));
}

//-----------------------------------------------------------------------------

int dasm_info_ctor (Dasm_info *Info)
{
    Info->code_file  = fopen ("../files/code.bin",   "rb");
    Info->file_out   = fopen ("../dump/dasm_file.txt",     "w+");

    if(Info->code_file  == NULL ||
       Info->file_out   == NULL   )
    {
        return ERR_CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void disassember_dtor (Disassember *Dasm)
{
    stack_dtor (&Dasm->Stk);

    free (Dasm->regs);
    free (Dasm->ram);
    free (Dasm->code);

    dasm_info_dtor (&Dasm->Info);
}

//-----------------------------------------------------------------------------

void dasm_info_dtor (Dasm_info *Info)
{
    fclose (Info->code_file);
    fclose (Info->file_out);
}

//-----------------------------------------------------------------------------

void read_code_file (Disassember *Dasm)
{
    double res_sum = -1;
    double code_sgntr = -1;

    fread (&res_sum, sizeof(double), 1, Dasm->Info.code_file);
    Dasm->code_size = res_sum - 1;

    fread (&code_sgntr, sizeof(double), 1, Dasm->Info.code_file);

    if(code_sgntr == CORCT_SIGN)
    {
        Dasm->code = (double*) calloc (res_sum, sizeof (double));

        if(Dasm->code == NULL)
        {
            printf ("__________|ERROR - NULL pointer code|__________\n");
        }

        Dasm->code[0] = res_sum;

        fread (Dasm->code + 1, sizeof(double), res_sum - 1, Dasm->Info.code_file);
    }

    else printf ("__________|WRONG SIGNATURE!|__________\n");
}

//-----------------------------------------------------------------------------

void handle_cmds (Disassember *Dasm)
{
    for(int curr_pos = 1; curr_pos < Dasm->code_size; curr_pos++)
    {
        int    curr_cmd = Dasm->code[curr_pos];
        int    offset   = 0;
        double curr_arg = 0;

        if(curr_cmd & MASK_REG)
        {
            curr_arg += Dasm->regs[(int) Dasm->code[curr_pos + 1]];
            offset++;
        }

        if(curr_cmd & MASK_IMM)
        {
            curr_arg += Dasm->code[curr_pos + 1 + offset];
        }

        if(curr_cmd & MASK_RAM)
        {
            curr_arg = Dasm->ram[(int) curr_arg];
        }

        if(curr_cmd & MASK_RAM &&
           curr_cmd & MASK_IMM &&
           curr_cmd & MASK_REG   )
           offset = 1;

        else offset = 0;

        Dasm->func (curr_cmd, curr_arg, &curr_pos, Dasm);
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






