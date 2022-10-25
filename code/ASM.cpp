#include "../include/ASM.h"

//-----------------------------------------------------------------------------

void assembler ()
{
    Asm_data_ *Asm_data = (Asm_data_*) calloc (1, sizeof (Asm_data_));
    Asm_data_ctor (Asm_data);

    files_ctor (Asm_data);

    fclose (Asm_data->file_in);
    fclose (Asm_data->code_file);
    fclose (Asm_data->label_file);

    free (Asm_data);
}

//-----------------------------------------------------------------------------

void Asm_data_ctor (Asm_data_ *data)
{
    data->file_in    = fopen ("../files/file4.txt",  "rb");
    data->code_file  = fopen ("../files/code.bin",   "wb");
    data->label_file = fopen ("../files/labels.bin", "wb");

    data->res_sum = 0;
    data->num_of_labels = 0;

    data->code_sgntr = Cor_signature;
}

//-----------------------------------------------------------------------------

void files_ctor (Asm_data_ *data)
{
    File *File_input = file_reader (data->file_in);
    Line *Text = lines_separator (File_input);

    fseek (data->code_file, sizeof(int) + sizeof(int32_t), SEEK_SET);

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        data->Cur_line = Text[i];

        char cmd[40] = "";

        sscanf (data->Cur_line.begin_line, "%s", cmd);

        if(found_label (data))
        {
            label_utility (data, cmd);
        }

        else if(found_arg_funct (cmd))
        {
            handle_arg_functs (data);
        }

        else
        {
            handle_com_functs (data, cmd);
        }
    }

    write_res_sums (data);

    clear_mem (Text, File_input);
}

//-----------------------------------------------------------------------------

void handle_label (Asm_data_ *data)
{
    int label = -1;
    int label_adr = data->res_sum;

    if(sscanf (data->Cur_line.begin_line, ":%d", &label) != 0)
    {
        if(label > data->num_of_labels)
        {
            data->num_of_labels = label;
        }

        fseek  (data->label_file, sizeof(int) * label, SEEK_SET);
        fwrite (&label_adr, sizeof(int), 1, data->label_file);
    }
}

//-----------------------------------------------------------------------------

void handle_jump (Asm_data_ *data, char *cmd_)
{
    int val     = -1;
    int cur_len = -1;

    for(int i = 10; i < 10 + Num_sup_jmps; i++)
    {
        if(stricmp (cmd_, Cmd_asm[i].name) == 0)
        {
            cur_len = Cmd_asm[i].par;
            fwrite (&Cmd_asm[i].num, sizeof(int), 1, data->code_file);
            break;
        }
    }

    if(sscanf (data->Cur_line.begin_line + cur_len, "%d:", &val) != 0)
    {
        fwrite (&val, sizeof(int), 1, data->code_file);
        data->res_sum += 2;
    }

    else
    {
        data->code_sgntr = Incor_signature;
    }
}

//-----------------------------------------------------------------------------

void label_utility (Asm_data_ *data, char *cmd_)
{
    if(cmd_[0] == ':')
    {
        handle_label (data);
    }

    else
    {
        handle_jump (data, cmd_);
    }
}

//-----------------------------------------------------------------------------

bool found_label (Asm_data_ *data)
{
    if(strchr (data->Cur_line.begin_line, ':') != NULL)
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

bool found_arg_funct (char *cmd_)
{
    if(stricmp (cmd_, "push") == 0)
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

void handle_arg_functs (Asm_data_ *data)
{
    char arg[100] = "";

    double val = -1;

    if(sscanf (data->Cur_line.begin_line + 5, "%lg", &val))
    {
        fwrite (&Cmd_asm[CMD_PUSH].num, sizeof(int), 1, data->code_file);
        fwrite (&val, sizeof(double), 1, data->code_file);
    }

    else
    {
        sscanf (data->Cur_line.begin_line + 5, "%s", arg);

        handle_regs (data, arg);

        handle_ram_args (data, arg);
    }

    data->res_sum += 2;
}

//-----------------------------------------------------------------------------

void handle_regs (Asm_data_ *data, char *arg_)
{
    if(arg_[0] == 'r' && arg_[2] == 'x')
    {
        int num_of_reg = arg_[1] - 97;

        fwrite (&Cmd_asm[CMD_RG_PUSH].num, sizeof(int), 1, data->code_file);
        fwrite (&num_of_reg, sizeof(int), 1, data->code_file);
    }
}

//-----------------------------------------------------------------------------

void handle_ram_args (Asm_data_ *data, char *arg_)
{
    if(arg_[0] == '[')
    {
        int num_of_rmarg = -1;

        if(sscanf (data->Cur_line.begin_line + 6, "%d", &num_of_rmarg))
        {
            fwrite (&Cmd_asm[CMD_RM_PUSH].num, sizeof(int), 1, data->code_file);
            fwrite (&num_of_rmarg, sizeof(int), 1, data->code_file);
        }

        else
        {
            data->code_sgntr = Incor_signature;
        }
    }
}

//-----------------------------------------------------------------------------

void handle_com_functs (Asm_data_ *data, char *cmd_)
{
    for(int num_cmd = 0; num_cmd < Num_sup_cmd; num_cmd++)
    {
        if(stricmp (cmd_, Cmd_asm[num_cmd].name) == 0)
        {
            fwrite (&Cmd_asm[num_cmd].num, sizeof(int), 1, data->code_file);
            data->res_sum++;
        }
    }
}

//-----------------------------------------------------------------------------

void write_res_sums (Asm_data_ *data)
{
    fseek  (data->code_file, 0, SEEK_SET);
    fwrite (&data->res_sum, sizeof(int), 1, data->code_file);
    fwrite (&data->code_sgntr, sizeof(int32_t), 1, data->code_file);

    fseek  (data->label_file, 0, SEEK_SET);
    fwrite (&data->num_of_labels, sizeof(int), 1, data->label_file);
}

//-----------------------------------------------------------------------------

