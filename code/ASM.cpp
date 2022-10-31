#include "../include/ASM.h"

//-----------------------------------------------------------------------------

int assembling (char *argv[])
{
    Assembler Asm = { 0 };

    if(assembler_ctor (&Asm, argv) == ERR_CTOR)
    {
        return ERR_ASM;
    }

    fill_asm_arrays (&Asm);

    files_ctor (&Asm);

    assembler_dtor (&Asm);

    return 0;
}

//-----------------------------------------------------------------------------

int assembler_ctor (Assembler *Asm, char *argv[])
{
    Asm->code_array  = (double*) calloc (100, sizeof (double));
    Asm->label_array = (int*)    calloc (50,  sizeof (int));

    if(Asm->code_array == NULL || Asm->label_array == NULL)
    {
        return ERR_CTOR;
    }

    Asm->code_arr_size  = 2;
    Asm->code_arr_capct = 100;

    Asm->label_arr_size  = 1;
    Asm->label_arr_capct = 50;

    Asm->Info     = { 0 };
    Asm->Cur_line = { 0 };

    return (asm_info_ctor (&(Asm->Info), argv));
}

//-----------------------------------------------------------------------------

int asm_info_ctor (Asm_info *Info, char *argv[])
{
    Info->code_sgntr = CORCT_SIGN;

    Info->file_in    = fopen ((char*) argv[1],       "rb");
    Info->code_file  = fopen ("../files/code.bin",   "wb");
    Info->label_file = fopen ("../files/labels.bin", "wb");

    if(Info->file_in    == NULL ||
       Info->code_file  == NULL ||
       Info->label_file == NULL   )
    {
        return ERR_CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void asm_opts_ctor (Assembler *Asm)
{
    Asm->Opts.val_dbl = -1;
    Asm->Opts.val_int = -1;
    Asm->Opts.reg_sym =  0;
    Asm->Opts.mask    =  0;
    Asm->Opts.type    =  1;
}

//-----------------------------------------------------------------------------

void fill_asm_arrays (Assembler *Asm)
{
    File *File_input = file_reader (Asm->Info.file_in);
    Line *Text = lines_separator (File_input);

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        Asm->Cur_line = Text[i];

        handle_line (Asm);
    }

    write_res_sums (Asm);

    clear_mem (Text, File_input);
}

//-----------------------------------------------------------------------------

void handle_line (Assembler *Asm)
{
    asm_opts_ctor (Asm);

    if(sscanf (Asm->Cur_line.begin_line, ":%d", &Asm->Opts.val_int) == 1)
    {
        parse_label (Asm);
    }

    else if(line_empty (Asm))
    {
        parse_arg (Asm);

        parse_cmd (Asm);
    }
}

//-----------------------------------------------------------------------------

void parse_label (Assembler *Asm)
{
    if(Asm->Opts.val_int > Asm->label_arr_size)
    {
        Asm->label_arr_size = Asm->Opts.val_int;
    }

    Asm->label_array[Asm->Opts.val_int] = Asm->code_arr_size;
}

//-----------------------------------------------------------------------------

void parse_arg (Assembler *Asm)
{
    if(strchr (Asm->Cur_line.begin_line, ':') != NULL)
    {
        if(sscanf (Asm->Cur_line.begin_line, "%20s %d", Asm->Opts.cmd, &Asm->Opts.val_int) == 2)
        {
            Asm->code_array[Asm->code_arr_size + 1] = Asm->Opts.val_int;
            Asm->Opts.mask |= MASK_IMM;
            Asm->Opts.type++;
        }

        else
        {
            Asm->Info.code_sgntr = WRONG_SIGN;
        }
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%20s [%d]", Asm->Opts.cmd, &Asm->Opts.val_int) == 2)
    {
        Asm->code_array[Asm->code_arr_size + 1] = Asm->Opts.val_int;
        Asm->Opts.mask |= MASK_RAM;
        Asm->Opts.type++;
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [r%cx + %d]", &Asm->Opts.reg_sym, &Asm->Opts.val_int) == 2)
    {
        Asm->code_array[Asm->code_arr_size + 1] = Asm->Opts.reg_sym - 'a';
        Asm->code_array[Asm->code_arr_size + 2] = Asm->Opts.val_int;
        Asm->Opts.mask |= (MASK_RAM | MASK_REG);
        Asm->Opts.type+=2;
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s r%cx", &Asm->Opts.reg_sym) == 1)
    {
        Asm->code_array[Asm->code_arr_size + 1] = Asm->Opts.reg_sym - 'a';
        Asm->Opts.mask |= MASK_REG;
        Asm->Opts.type++;
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s %lg", &Asm->Opts.val_dbl) == 1)
    {
        Asm->code_array[Asm->code_arr_size + 1] = Asm->Opts.val_dbl;
        Asm->Opts.mask |= MASK_IMM;
        Asm->Opts.type++;
    }
}

//-----------------------------------------------------------------------------

void parse_cmd (Assembler *Asm)
{
    int flag_found = 0;

    #include "../include/cmd_names.h"

    for(int num_cmd = 0; num_cmd < NUM_SUP_CMD; num_cmd++)
    {
        if(stricmp (Asm->Opts.cmd, name_cmd[num_cmd]) == 0)
        {
            Asm->code_array[Asm->code_arr_size] = num_cmd | Asm->Opts.mask;
            Asm->code_arr_size += Asm->Opts.type;
            flag_found++;

            break;
        }
    }

    if(flag_found == 0) Asm->Info.code_sgntr = WRONG_SIGN;
}

//-----------------------------------------------------------------------------

int line_empty (Assembler *Asm)
{
    char c[MAX_LEN] = "";
    sscanf (Asm->Cur_line.begin_line, "%s", c);

    int cmd_len = strlen (c);
    int num_of_sym = 0;

    for(int i = 0; i < cmd_len; i++)
    {
        if(c[i] != '_')
        {
            num_of_sym++;

            break;
        }
    }

    return num_of_sym;
}

//-----------------------------------------------------------------------------

void files_ctor (Assembler *Asm)
{
    fwrite (Asm->code_array,  sizeof(double), Asm->code_arr_size,  Asm->Info.code_file);
    fwrite (Asm->label_array, sizeof(int),    Asm->label_arr_size + 1, Asm->Info.label_file);
}

//-----------------------------------------------------------------------------

void assembler_dtor (Assembler *Asm)
{
    free (Asm->code_array);
    free (Asm->label_array);

    Asm->code_arr_size  = -1;
    Asm->code_arr_capct = -1;

    Asm->label_arr_size  = -1;
    Asm->label_arr_capct = -1;

    Asm->Info.code_sgntr = WRONG_SIGN;

    asm_info_dtor (&(Asm->Info));
}

//-----------------------------------------------------------------------------

void asm_info_dtor (Asm_info *Info)
{
    Info->code_sgntr = WRONG_SIGN;
    fclose (Info->file_in);
    fclose (Info->code_file);
    fclose (Info->label_file);
}

//-----------------------------------------------------------------------------

void write_res_sums (Assembler *Asm)
{
    Asm->code_array[0]  = Asm->code_arr_size;
    Asm->code_array[1]  = Asm->Info.code_sgntr;
    Asm->label_array[0] = Asm->label_arr_size;
}

//-----------------------------------------------------------------------------






