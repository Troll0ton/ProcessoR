#include "../include/DASM.h"

//-----------------------------------------------------------------------------

int disassembling ()
{
    Disassember Dasm = { 0 };

    if(disassember_ctor (&Dasm) == ERR_CTOR)
    {
        return ERR_DASM;
    }

    read_files (&Dasm);

    file_out_ctor (&Dasm);

    disassember_dtor (&Dasm);

    return 0;
}

//-----------------------------------------------------------------------------

void handle_cmds (int cmd_d, double arg_d, int *ipp, Processor *Cpu)
{
    int ip = *ipp;

    double f1 = -1;
    double f2 = -1;

    #define CMD_DEF(cmd, code, ...) \
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

    stack_dumps (&Cpu->Stk, Cpu->Info.log_file);

    *ipp = ip;
}

//-----------------------------------------------------------------------------

int main ()
{
    disassembling ();

    return 0;
}

//-----------------------------------------------------------------------------

