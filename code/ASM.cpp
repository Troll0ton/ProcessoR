#include "../include/ASM.h"
#include "../include/lines_handle.h"
#include "../include/input_output.h"

//-----------------------------------------------------------------------------

void assembler ()
{
    FILE *file_in = fopen ("../files/file1.txt", "rb");
    FILE *code_file = fopen ("../files/code.txt", "wb");

    struct File *File_input = file_reader (file_in);
    struct Line *Text = lines_separator   (File_input);

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        char cmd[100] = "";

        sscanf (Text[i].begin_line, "%s", cmd);

        if(stricmp(cmd, "push") == 0)
        {
            double val = 0;
            sscanf  (Text[i].begin_line + 5, "%lg", &val);

            fwrite (&CMD_PUSH, sizeof(int), 1, code_file);
            fwrite (&val, sizeof(double), 1, code_file);
        }

        else if(stricmp(cmd, "hlt") == 0)
        {
            fwrite (&CMD_HLT, sizeof(int), 1, code_file);
        }

        else if(stricmp(cmd, "add") == 0)
        {
            fwrite (&CMD_ADD, sizeof(int), 1, code_file);
        }

        else if(stricmp(cmd, "sub") == 0)
        {
            fwrite (&CMD_SUB, sizeof(int), 1, code_file);
        }

        else if(stricmp(cmd, "mul") == 0)
        {
            fwrite (&CMD_MUL, sizeof(int), 1, code_file);
        }

        else if(stricmp(cmd, "div") == 0)
        {
            fwrite (&CMD_DIV, sizeof(int), 1, code_file);
        }

        else if(stricmp(cmd, "out") == 0)
        {
            fwrite (&CMD_OUT, sizeof(int), 1, code_file);
        }
    }

    fclose (file_in);
    fclose (code_file);
}

//-----------------------------------------------------------------------------

int main()
{
    assembler ();

    return 0;
}

//-----------------------------------------------------------------------------



