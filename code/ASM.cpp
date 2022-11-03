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
    Asm->code_array  = (double*) calloc (CODE_SIZE_INIT,   sizeof (double));
    Asm->label_array = (int*)    calloc (LABEL_SIZE_INIT,  sizeof (int));

    if(Asm->code_array == NULL || Asm->label_array == NULL)
    {
        return ERR_CTOR;
    }

    Asm->code_arr_size  = 2;
    Asm->code_arr_capct = CODE_SIZE_INIT;

    Asm->label_arr_size  = 1;
    Asm->label_arr_capct = LABEL_SIZE_INIT;

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

void asm_pars_ctor (Assembler *Asm)
{
    Asm->Pars.val_dbl          = -1;
    Asm->Pars.val_int          = -1;
    Asm->Pars.reg_sym          =  0;
    Asm->Pars.flag_cmd         =  0;
    Asm->Pars.mask             =  0;
    Asm->Pars.num_readed_codes =  1;
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
    asm_pars_ctor (Asm);

    if(sscanf (Asm->Cur_line.begin_line, ":%d", &Asm->Pars.val_int) == 1)
    {
        parse_label (Asm);
    }

    else
    {
        parse_arg (Asm);

        if(Asm->Pars.flag_cmd) parse_cmd (Asm);
    }
}

//-----------------------------------------------------------------------------

void parse_label (Assembler *Asm)
{
    if(Asm->label_arr_size + LIMIT_DIFRCE > Asm->label_arr_capct)
    {
        Asm->label_arr_capct *= 2;

        Asm->label_array = (int*) recalloc (Asm->label_array,
                                            Asm->label_arr_capct,
                                            Asm->label_arr_size,
                                            sizeof (int)         );
    }

    if(Asm->Pars.val_int > Asm->label_arr_size)
    {
        Asm->label_arr_size = Asm->Pars.val_int;
    }

    Asm->label_array[Asm->Pars.val_int] = Asm->code_arr_size;
}

//-----------------------------------------------------------------------------

void parse_arg (Assembler *Asm)
{
    if(sscanf (Asm->Cur_line.begin_line, "%20s", Asm->Pars.cmd) == 1)
    {
        Asm->Pars.flag_cmd++;
    }

    if(strchr (Asm->Cur_line.begin_line, ':') != NULL)
    {
        if(sscanf (Asm->Cur_line.begin_line, "%*s %d", &Asm->Pars.val_int) == 1)
        {
            write_in_arg (Asm, Asm->Pars.val_int, MASK_IMM);
        }

        else
        {
            Asm->Info.code_sgntr = WRONG_SIGN;
        }
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [r%cx + %d]", &Asm->Pars.reg_sym, &Asm->Pars.val_int) == 2)
    {
        write_in_arg (Asm, Asm->Pars.reg_sym - 'a', 0);
        write_in_arg (Asm, Asm->Pars.val_int , MASK_RAM | MASK_REG | MASK_IMM);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [%d]", &Asm->Pars.val_int) == 1)
    {
        write_in_arg (Asm, Asm->Pars.val_int, MASK_RAM | MASK_IMM);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [r%cx]", &Asm->Pars.reg_sym) == 1)
    {
        write_in_arg (Asm, Asm->Pars.reg_sym - 'a', MASK_RAM | MASK_REG);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s r%cx", &Asm->Pars.reg_sym) == 1)
    {
        write_in_arg (Asm, Asm->Pars.reg_sym - 'a', MASK_REG);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s %lg", &Asm->Pars.val_dbl) == 1)
    {
        write_in_arg (Asm, Asm->Pars.val_dbl, MASK_IMM);
    }
}

//-----------------------------------------------------------------------------

void write_in_arg (Assembler *Asm, double val, int mask)
{
    if(Asm->code_arr_size + LIMIT_DIFRCE > Asm->code_arr_capct)
    {
        Asm->code_arr_capct *= 2;

        Asm->code_array = (double*) recalloc (Asm->code_array,
                                              Asm->code_arr_capct,
                                              Asm->code_arr_size,
                                              sizeof (double)     );
    }

    Asm->code_array[Asm->code_arr_size + 1] = val;
    Asm->Pars.mask |= mask;
    Asm->Pars.num_readed_codes++;
}

//-----------------------------------------------------------------------------

void parse_cmd (Assembler *Asm)
{
    int flag_found = 0;

    char *name_cmd[] =
    {
        #define CMD_DEF(cmd, name, ...) \
            name,

        #include "../include/codegen.h"

        #undef CMD_DEF
    };

    for(int num_cmd = 0; num_cmd < NUM_OF_SUP_CMD; num_cmd++)
    {
        if(stricmp (Asm->Pars.cmd, name_cmd[num_cmd]) == 0)
        {
            Asm->code_array[Asm->code_arr_size] = num_cmd | Asm->Pars.mask;
            Asm->code_arr_size += Asm->Pars.num_readed_codes;
            flag_found++;

            break;
        }
    }

    if(flag_found == 0) Asm->Info.code_sgntr = WRONG_SIGN;
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

    Asm->code_arr_size   = -1;
    Asm->code_arr_capct  = -1;
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

