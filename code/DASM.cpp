#include "../include/DASM.h"
#include "../include/lines_handle.h"
#include "../include/input_output.h"

//-----------------------------------------------------------------------------

void disassembler ()
{
    FILE *code_file = fopen ("../files/code.txt", "rb");
    FILE *file_out = fopen ("../files/file2.txt", "wb");

    double num = 0;
    int cmd = -1;
    int flag_stop = 0;

    while(flag_stop == 0)
    {
        fread (&cmd, sizeof(int), 1, code_file);

        switch(cmd)
        {
            case CMD_PUSH:
                fread (&num, sizeof(double), 1, code_file);
                fprintf (file_out, "push %lg\n", num);
                break;
            case CMD_ADD:
                fprintf (file_out, "add\n");
                break;
            case CMD_SUB:
                fprintf (file_out, "sub\n");
                break;
            case CMD_MUL:
                fprintf (file_out, "mul\n");
                break;
            case CMD_DIV:
                fprintf (file_out, "div\n");
                break;
            case CMD_HLT:
                fprintf (file_out, "hlt\n");
                flag_stop++;
                break;
            case CMD_OUT:
                fprintf (file_out, "out\n");
                break;
            default:
                fprintf (file_out, "????\n");
                break;
        }
    }

    fclose (file_out);
    fclose (code_file);
}

//-----------------------------------------------------------------------------

int main()
{
    disassembler ();

    return 0;
}

//-----------------------------------------------------------------------------
