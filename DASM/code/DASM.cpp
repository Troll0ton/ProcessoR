#include "../include/DASM.h"

//-----------------------------------------------------------------------------

void disassembling (Disassember *dasm)
{
    for(int curr_pos = O(CODE_SIGNATURE); curr_pos < dasm->code_size; curr_pos++)
    {
        print_file("%07d | ", curr_pos);

        int curr_cmd = dasm->code[curr_pos];

        #define CMD_DEF(cmd, name, ...)  \
            case cmd:                    \
            {                            \
                print_file("%s ", name); \
                break;                   \
            }

        switch (curr_cmd & MASK_CMD)
        {
            #include "../../COMMON/include/codegen/codegen.h"

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

        #include "../../COMMON/include/codegen/dasm_codegen.h"

        //-----------------------------------------------------------------------------

        #undef PRINT_ARG

        /*else*/ print_file("\n");
    }
}

//-----------------------------------------------------------------------------

int disassember_ctor (Disassember *dasm)
{
    dasm->info = { 0 };

    dasm->Stk = { 0 };
    stack_ctor (&dasm->Stk, 2);

    return (Dasm_info_ctor (&dasm->info));
}

//-----------------------------------------------------------------------------

int Dasm_info_ctor (Dasm_info *info)
{
    info->code_file = fopen ("COMMON/files/code.bin",   "rb");
    info->file_out  = fopen ("DASM/dump/dasm_file.txt", "w+");

    if(info->code_file == NULL ||
       info->file_out  == NULL   )
    {
        return ERROR_CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void disassember_dtor (Disassember *dasm)
{
    stack_dtor (&dasm->Stk);

    free (dasm->code);

    Dasm_info_dtor (&dasm->info);
}

//-----------------------------------------------------------------------------

void Dasm_info_dtor (Dasm_info *info)
{
    fclose (info->code_file);
    fclose (info->file_out);
}

//-----------------------------------------------------------------------------

void read_code_file (Disassember *dasm)
{
    double code_signature = 0;
    double res_sum        = 0;

    fread (&res_sum,        sizeof(char), O(ARG), dasm->info.code_file);
    fread (&code_signature, sizeof(char), O(ARG), dasm->info.code_file);

    dasm->code_size = res_sum;

    if(is_equal (code_signature, SIGNATURE))
    {
        dasm->code = (char*) calloc (dasm->code_size, sizeof (char));

        *(elem_t*)(dasm->code + 0) = res_sum;
        *(elem_t*)(dasm->code + O(ARG)) = code_signature;

        if(dasm->code == NULL)
        {
            printf ("__________|ERROR - NULL pointer code|__________\n");
        }

        fread (dasm->code + O(CODE_SIGNATURE), sizeof(char), dasm->code_size - O(CODE_SIGNATURE), dasm->info.code_file);
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
