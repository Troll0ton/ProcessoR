#include "../include/DASM.h"

//-----------------------------------------------------------------------------

int main ()
{
    Disassember Dasm = { 0 };

    if(disassember_ctor (&Dasm) == E(CTOR)
    {
        return E(DASM;
    }

    read_code_file (&Dasm);

    disassembling (&Dasm);

    disassember_dtor (&Dasm);

    return 0;
}

//-----------------------------------------------------------------------------

void disassembling (Disassember *Dasm)
{
    for(int curr_pos = ARG_OFFSET; curr_pos < Dasm->code_size; curr_pos++)
    {
        int curr_cmd = Dasm->code[curr_pos];

        #define CMD_DEF(cmd, name, ...)  \
            case cmd:                    \
            {                            \
                print_file("%s ", name); \
                break;                   \
            }

        switch (curr_cmd & MASK_CMD)
        {
            #include "../include/codegen.h"

            default:
                print_file("?%d ", curr_cmd);
                break;
        }

        #undef CMD_DEF

        #define PRINT_ARG(offset, name_msk, format, ...) \
            if((curr_cmd & (name_msk)) == (name_msk))    \
            {                                            \
                print_file(format, __VA_ARGS__);         \
                curr_pos += offset;                      \
            }                                            \
            else

        //-----------------------------------------------------------------------------

        #include "../include/dasm_codegen.h"

        //-----------------------------------------------------------------------------

        #undef PRINT_ARG

        /*else*/print_file("\n");
    }
}

//-----------------------------------------------------------------------------

int disassember_ctor (Disassember *Dasm)
{
    Dasm->Info = { 0 };

    Dasm->Stk = { 0 };
    stack_ctor (&Dasm->Stk, 2);

    return (dasm_info_ctor (&Dasm->Info));
}

//-----------------------------------------------------------------------------

int dasm_info_ctor (Dasm_info *Info)
{
    Info->code_file = fopen ("../files/code.bin",     "rb");
    Info->file_out  = fopen ("../dump/dasm_file.txt", "w+");

    if(Info->code_file == NULL ||
       Info->file_out  == NULL   )
    {
        return E(CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void disassember_dtor (Disassember *Dasm)
{
    stack_dtor (&Dasm->Stk);

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

void read_code_file (Processor *Cpu)
{
    double res_sum        = 0;
    double code_signature = 0;

    fread (&res_sum, sizeof(elem_t), 1, Cpu->Info.code_file);
    Cpu->code_size = res_sum - ARG_OFFSET;

    fread (&code_signature, sizeof(elem_t), 1, Cpu->Info.code_file);

    if(code_signature == SIGNATURE)
    {
        Cpu->code = (char*) calloc (res_sum, sizeof (char));

        if(Cpu->code == NULL)
        {
            printf ("__________|ERROR - NULL pointer code|__________\n");
        }

        *(elem_t*)(Cpu->code + 0) = res_sum;

        fread (Cpu->code + ARG_OFFSET, sizeof(char), res_sum - ARG_OFFSET, Cpu->Info.code_file);
    }

    else
    {
        printf ("__________|WRONG SIGNATURE!|__________\n");
    }
}
//-----------------------------------------------------------------------------

bool is_equal (double a, double b)
{
    const double EPS = 1e-1;

    return (a - b < EPS && a - b > -EPS);
}

//-----------------------------------------------------------------------------






