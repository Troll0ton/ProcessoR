#include "../include/ASM.h"

//-----------------------------------------------------------------------------

int assembling (char *argv[])
{
    Asm_info_ *Asm_info = (Asm_info_*) calloc (1, sizeof (Asm_info_));
    // if ( == NULL)
    Asm_info_ctor (Asm_info);

    if(open_files (Asm_info, argv) == Failed_open)
    {
        return Error_asm;
    }

    files_ctor (Asm_info);

    close_files (Asm_info);

    free (Asm_info);

    return 0;
}

//-----------------------------------------------------------------------------

void Asm_info_ctor (Asm_info_ *data)
{
    data->res_sum = 0;
    data->num_of_labels = 0;

    data->code_sgntr = Cor_signature;
}

//-----------------------------------------------------------------------------

void files_ctor (Asm_info_ *data)
{
    File *File_input = file_reader (data->file_in);
    Line *Text = lines_separator (File_input);

    fseek (data->code_file, sizeof(int) + sizeof(int32_t), SEEK_SET);

    for (int i = 0; i < File_input->num_of_lines; i++)
    {
        data->Cur_line = Text[i];

        char cmd[Max_cmd_size] = "";

        sscanf (data->Cur_line.begin_line, "%s", cmd);

        handle_lines (data, cmd);
    }

    write_res_sums (data);

    clear_mem (Text, File_input);
}

//-----------------------------------------------------------------------------
// parse

void handle_label (Asm_info_ *data)
{
    int label  = -1;
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

// :14124
// jmp 14:

void handle_jump (Asm_info_ *data, char *cmd)
{
    int val = -1;
    int cur_len = -1;

    for(int i = Jump_num; i < Jump_num + Num_sup_jmps; i++)
    {
        if(stricmp (cmd, Cmd_asm[i].name) == 0)
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
// divorce ??

void label_utility (Asm_info_ *data, char *cmd)
{
    if(cmd[0] == ':')
    {
        handle_label (data);
    }

    else
    {
        handle_jump (data, cmd);
    }
}

//-----------------------------------------------------------------------------

bool found_label (Asm_info_ *data)
{
    return (strchr (data->Cur_line.begin_line, ':') != NULL);   // ??
}

//-----------------------------------------------------------------------------

bool found_arg_funct (char *cmd)
{
    return (stricmp (cmd, "push") == 0);   // ??
}

//-----------------------------------------------------------------------------

void handle_arg_functs (Asm_info_ *data)
{
    char arg[Arg_max_len] = "";

    // 5??
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

void handle_regs (Asm_info_ *data, char *arg)
{
    if(arg[0] == 'r' && arg[2] == 'x')
    {
        int num_of_reg = arg[1] - 'a';

        fwrite (&Cmd_asm[CMD_RG_PUSH].num, sizeof(int), 1, data->code_file);
        fwrite (&num_of_reg, sizeof(int), 1, data->code_file);
    }
}

//-----------------------------------------------------------------------------

void handle_ram_args (Asm_info_ *data, char *arg)
{
    if(arg[0] == '[')
    {
        int num_of_rmarg = -1;

        if(sscanf (data->Cur_line.begin_line + Push_len, "%d", &num_of_rmarg))
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

void handle_com_functs (Asm_info_ *data, char *cmd)
{
    int flag_found = 0;

    for(int num_cmd = 0; num_cmd < Num_sup_cmd; num_cmd++)
    {
        if(stricmp (cmd, Cmd_asm[num_cmd].name) == 0)
        {
            fwrite (&Cmd_asm[num_cmd].num, sizeof(int), 1, data->code_file);
            data->res_sum++;
            flag_found++;

            break;
        }
    }

    int cmd_len = strlen (cmd);
    int num_of_sym = 0;

    for(int i = 0; i < cmd_len; i++)
    {
        if(cmd[i] != '_')
        {
            num_of_sym++;

            break;
        }
    }

    if(num_of_sym == 0) flag_found++;

    if(flag_found == 0) data->code_sgntr = Incor_signature;
}

//-----------------------------------------------------------------------------

void write_res_sums (Asm_info_ *data)
{
    fseek  (data->code_file, 0, SEEK_SET);
    fwrite (&data->res_sum, sizeof(int), 1, data->code_file);
    fwrite (&data->code_sgntr, sizeof(int32_t), 1, data->code_file);

    fseek  (data->label_file, 0, SEEK_SET);
    fwrite (&data->num_of_labels, sizeof(int), 1, data->label_file);
}

//-----------------------------------------------------------------------------

void close_files (Asm_info_ *data)
{
    fclose (data->file_in);
    fclose (data->code_file);
    fclose (data->label_file);
}

//-----------------------------------------------------------------------------

int open_files (Asm_info_ *data, char *argv[])
{
    data->file_in    = fopen ((char*) argv[1],       "rb");
    data->code_file  = fopen ("../files/code.bin",   "wb");
    data->label_file = fopen ("../files/labels.bin", "wb");

    if(data->file_in    == NULL ||
       data->code_file  == NULL ||
       data->label_file == NULL   ) return Failed_open;

    return 0;
}

//-----------------------------------------------------------------------------

void handle_lines (Asm_info_ *data, char *cmd)
{
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
        // cmd
        handle_com_functs (data, cmd);
    }
}

//-----------------------------------------------------------------------------


