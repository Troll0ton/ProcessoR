#include "../include/DASM.h"

//-----------------------------------------------------------------------------

int main ()
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

    return 0;
}

//-----------------------------------------------------------------------------

void file_writer (int curr_cmd, double curr_arg, int *curr_ptr, Processor *Cpu)
{
    int cur_pos = *curr_ptr;

    #define CMD_DEF(cmd, name, ...)                    \
        case cmd:                                      \
            fprintf (Cpu->Info.file_out, "%s ", name); \
            break;

    switch (curr_cmd & MASK_CMD)
    {
        #include "../include/codegen.h"

        default:
            fprintf (Cpu->Info.file_out, "?%d ", curr_cmd);
            break;
    }

    #undef CMD_DEF

    if(curr_cmd & MASK_RAM &&
       curr_cmd & MASK_IMM &&
       curr_cmd & MASK_REG   )
    {
        fprintf (Cpu->Info.file_out, "[r%cx + %d]", (int) Cpu->code[cur_pos + 1] + 'a', (int) Cpu->code[cur_pos + 2]);
        cur_pos += 2;
    }

    else if(curr_cmd & MASK_REG &&
            curr_cmd & MASK_RAM   )
    {
        fprintf (Cpu->Info.file_out, "[r%cx]", (int) Cpu->code[cur_pos + 1] + 'a');
        cur_pos += 1;
    }

    else if(curr_cmd & MASK_IMM &&
            curr_cmd & MASK_RAM   )
    {
        fprintf (Cpu->Info.file_out, "[%d]", (int) Cpu->code[cur_pos + 1]);
        cur_pos += 1;
    }

    else if(curr_cmd & MASK_IMM)
    {
        fprintf (Cpu->Info.file_out, "%d", (int) Cpu->code[cur_pos + 1]);
        cur_pos += 1;
    }

    else if(curr_cmd & MASK_REG)
    {
        fprintf (Cpu->Info.file_out, "r%cx", (int) Cpu->code[cur_pos + 1] + 'a');
        cur_pos += 1;
    }

    fprintf (Cpu->Info.file_out, "\n");

    *curr_ptr = cur_pos;
}

//-----------------------------------------------------------------------------




