#include "../include/DASM.h"

//-----------------------------------------------------------------------------

int main ()
{
    disassembling ();

    return 0;
}

//-----------------------------------------------------------------------------

int disassembling ()
{
    Disassember Dasm = { 0 };

    if(disassember_ctor (&Dasm, file_writer) == ERR_CTOR)
    {
        return ERR_DASM;
    }

    read_files (&Dasm);

    handle_cmds (&Dasm);

    disassember_dtor (&Dasm);

    return 0;
}

//-----------------------------------------------------------------------------

void file_writer (int cmd_d, double arg_d, int *ipp, Processor *Cpu)
{
    int ip = *ipp;

    #define CMD_DEF(cmd, name, ...)                    \
        case cmd:                                      \
            fprintf (Cpu->Info.file_out, "%s ", name); \
            break;

    switch (cmd_d & MASK_CMD)
    {
        #include "../include/codegen.h"

        default:
            fprintf (Cpu->Info.file_out, "?%d ", cmd_d);
            break;
    }

    #undef CMD_DEF

    if(cmd_d & MASK_RAM &&
       cmd_d & MASK_IMM &&
       cmd_d & MASK_REG   )
    {
        fprintf (Cpu->Info.file_out, "[r%cx + %d]", (int) Cpu->code[ip + 1] + 'a', (int) Cpu->code[ip + 2]);
        ip += 2;
    }

    else if(cmd_d & MASK_REG &&
            cmd_d & MASK_RAM   )
    {
        fprintf (Cpu->Info.file_out, "[r%cx]", (int) Cpu->code[ip + 1] + 'a');
        ip += 1;
    }

    else if(cmd_d & MASK_IMM &&
            cmd_d & MASK_RAM   )
    {
        fprintf (Cpu->Info.file_out, "[%d]", (int) Cpu->code[ip + 1]);
        ip += 1;
    }

    else if(cmd_d & MASK_IMM)
    {
        fprintf (Cpu->Info.file_out, "%d", (int) Cpu->code[ip + 1]);
        ip += 1;
    }

    else if(cmd_d & MASK_REG)
    {
        fprintf (Cpu->Info.file_out, "r%cx", (int) Cpu->code[ip + 1] + 'a');
        ip += 1;
    }

    fprintf (Cpu->Info.file_out, "\n");

    *ipp = ip;
}

//-----------------------------------------------------------------------------




