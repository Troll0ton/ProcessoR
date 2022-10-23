#include "../include/ASM.h"
#include "../include/lines_handle.h"
#include "../include/input_output.h"

//-----------------------------------------------------------------------------

void assembler ()
{
    FILE *file_in    = fopen ("../files/file1.txt",  "rb");
    FILE *code_file  = fopen ("../files/code.txt",   "wb");
    FILE *label_file = fopen ("../files/labels.txt", "wb");

    struct File *File_input = file_reader (file_in);
    struct Line *Text       = lines_separator (File_input);

    int res_sum = 0;
    int num_of_labels = 0;
    double val = 0;

    fseek (code_file, sizeof(int), SEEK_SET);

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        char cmd[100] = "";

        sscanf (Text[i].begin_line, "%s", cmd);

        //-----------------------------------------------------------------------------

        if(strchr (Text[i].begin_line, ':') != NULL)
        {
            if(stricmp (cmd, "jump") == 0)
            {
                fwrite (&Cmd_asm[CMD_JUMP].num, sizeof(int), 1, code_file);

                int ind = -1;

                if(sscanf (Text[i].begin_line + 5, "%d:", &ind) != 0)
                {
                    fwrite (&ind, sizeof(int), 1, code_file);
                    res_sum += 2;
                }

                else
                {
                    printf ("ERROR");
                }
            }

            else
            {
                int label     = -1;
                int label_adr = res_sum;

                if(sscanf (Text[i].begin_line, ":%d", &label) != 0)
                {
                    if(label > num_of_labels)num_of_labels = label;

                    fseek  (label_file, sizeof(int) * label, SEEK_SET);
                    fwrite (&label_adr, sizeof(int), 1, label_file);
                }

                else
                {
                    printf ("ERROR");
                }
            }
        }

        //-----------------------------------------------------------------------------

        else if(stricmp (cmd, "push") == 0)
        {
            char reg[4] = "";

            if(sscanf (Text[i].begin_line + 5, "%lg", &val))
            {
                fwrite (&Cmd_asm[CMD_PUSH].num, sizeof(int), 1, code_file);
                fwrite (&val, sizeof(double), 1, code_file);
            }

            else
            {
                sscanf (Text[i].begin_line + 5, "%s", reg);

                if(reg[0] == 'r' && reg[2] == 'x')
                {
                    int num_of_reg = reg[1] - 97;

                    fwrite (&Cmd_asm[CMD_RPUSH].num, sizeof(int), 1, code_file);
                    fwrite (&num_of_reg, sizeof(int), 1, code_file);
                }
            }

            res_sum++;
        }

        //-----------------------------------------------------------------------------

        else
        {
            for(int num_cmd = 0; num_cmd < num_sup_cmd; num_cmd++)
            {
                fwrite (&Cmd_asm[num_cmd].num, sizeof(int), 1, code_file);
                res_sum++;

                if(Cmd_asm[num_cmd].par)
                {
                    sscanf (Text[i].begin_line + 5, "%lg", &val);
                    fwrite (&val, sizeof(double), 1, code_file);
                    res_sum++;
                }
            }
        }

        //-----------------------------------------------------------------------------

    }

    fseek  (code_file, 0, SEEK_SET);
    fwrite (&res_sum, sizeof(int), 1, code_file);

    fseek  (label_file, 0, SEEK_SET);
    fwrite (&num_of_labels, sizeof(int), 1, label_file);

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



