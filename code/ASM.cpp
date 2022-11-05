#include "../include/ASM.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    Assembler Asm = { 0 };

    if(assembler_ctor (&Asm, argv) == ERR_CTOR)
    {
        printf ("----ERROR ASM----");
    }

    assembling (&Asm);

    asm_dump (&Asm);

    assembler_dtor (&Asm);

    return 0;
}

//-----------------------------------------------------------------------------

int assembler_ctor (Assembler *Asm, char *argv[])
{
    Asm->code_array  = (double*) calloc (CODE_SIZE_INIT,  sizeof (double));
    Asm->label_array = (int*)    calloc (LABEL_SIZE_INIT, sizeof (int));

    if(Asm->code_array == NULL || Asm->label_array == NULL)
    {
        return ERR_CTOR;
    }

    Asm->code_arr_size  = NUM_FRST_EL_CD;
    Asm->cur_pos        = NUM_FRST_EL_CD;
    Asm->code_arr_capct = CODE_SIZE_INIT;
    Asm->offset         = 0;

    Asm->label_arr_size  = NUM_FRST_EL_LB;
    Asm->label_arr_capct = LABEL_SIZE_INIT;

    Asm->Info     = { 0 };
    Asm->Cur_line = { 0 };

    return asm_info_ctor (&(Asm->Info), argv);
}

//-----------------------------------------------------------------------------

int asm_info_ctor (Asm_info *Info, char *argv[])
{
    Info->code_sgntr = CORCT_SIGN;

    Info->file_in    = fopen ((char*) argv[1],       "rb");
    Info->code_file  = fopen ("../files/code.bin",   "wb");
    Info->dbl_pass   = false;

    if(Info->file_in    == NULL ||
       Info->code_file  == NULL   )
    {
        return ERR_CTOR;
    }

    return 0;
}

//-----------------------------------------------------------------------------

void assembling (Assembler *Asm)
{
    File *File_input = file_reader (Asm->Info.file_in);
    Line *Text = lines_separator (File_input);

    handle_text (Asm, Text, File_input);

    write_res_sums (Asm);

    clear_mem (Text, File_input);

    fwrite (Asm->code_array, sizeof(double), Asm->code_arr_size, Asm->Info.code_file);
}

//-----------------------------------------------------------------------------

void handle_text (Assembler *Asm, Line *Text, File *File_input)
{
    Asm->Info.dbl_pass = false;

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        Asm->Cur_line = Text[i];

        handle_line (Asm);
    }

    Asm->code_arr_size = Asm->cur_pos;
    Asm->cur_pos = 0;

    if(Asm->Info.dbl_pass) handle_text (Asm, Text, File_input);
}

//-----------------------------------------------------------------------------

void handle_line (Assembler *Asm)
{
    int label;

    Command Cmd = { 0 };

    if(!Asm->Info.dbl_pass && sscanf (Asm->Cur_line.begin_line, ":%d", &label) == 1)
    {
        parse_label (Asm, label);
    }

    else
    {
        parse_arg (Asm, &Cmd);

        if(Cmd.flag_cmd) parse_cmd (Asm, Cmd);
    }
}

//-----------------------------------------------------------------------------

void parse_label (Assembler *Asm, int label)
{
    if(Asm->label_arr_size + LIMIT_DIFRCE > Asm->label_arr_capct)
    {
        Asm->label_arr_capct *= 2;

        Asm->label_array = (int*) recalloc (Asm->label_array,
                                            Asm->label_arr_capct,
                                            Asm->label_arr_size,
                                            sizeof (int)         );
    }

    if(label > Asm->label_arr_size)
    {
        Asm->label_arr_size = label;
    }

    Asm->label_array[label] = Asm->cur_pos;
}

//-----------------------------------------------------------------------------

void parse_arg (Assembler *Asm, Command *Cmd)
{
    Cmd->title     = NULL;
    Cmd->flag_cmd = 0;
    Cmd->mask     = 0;
    Asm->offset   = 0;

    int    label                 = 0;
    char   reg_sym               = 0;
    char   cmd_name[CMD_MAX_LEN] = "";
    double arg_val               = 0;

    if(sscanf (Asm->Cur_line.begin_line, "%20s", cmd_name) == 1)
    {
        Cmd->title = cmd_name;
        Cmd->flag_cmd++;
    }

    if(strchr (Asm->Cur_line.begin_line, ':') != NULL)
    {
        if(sscanf (Asm->Cur_line.begin_line, "%*s %d", &label) == 1)
        {
            parse_jmp (Asm, Cmd, label);
        }

        else
        {
            Asm->Info.code_sgntr = WRONG_SIGN;
        }
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [r%cx + %lg]", &reg_sym, &arg_val) == 2)
    {
        write_in_arg (Asm, Cmd, reg_sym - 'a', 0);
        write_in_arg (Asm, Cmd, arg_val, MASK_RAM | MASK_REG | MASK_IMM);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [%lg]", &arg_val) == 1)
    {
        write_in_arg (Asm, Cmd, arg_val, MASK_RAM | MASK_IMM);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s [r%cx]", &reg_sym) == 1)
    {
        write_in_arg (Asm, Cmd, reg_sym - 'a', MASK_RAM | MASK_REG);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s r%cx", &reg_sym) == 1)
    {
        write_in_arg (Asm, Cmd, reg_sym - 'a', MASK_REG);
    }

    else if(sscanf (Asm->Cur_line.begin_line, "%*s %lg", &arg_val) == 1)
    {
        write_in_arg (Asm, Cmd, arg_val, MASK_IMM);
    }
}

//-----------------------------------------------------------------------------

void parse_jmp (Assembler *Asm, Command *Cmd, int label)
{
    if(label <= Asm->label_arr_size && Asm->label_array[label] > 0)
    {
        write_in_arg (Asm, Cmd, Asm->label_array[label], MASK_IMM);
    }

    else
    {
        write_in_arg (Asm, Cmd, -1, MASK_IMM);

        Asm->Info.dbl_pass = true;
    }
}

//-----------------------------------------------------------------------------

void write_in_arg (Assembler *Asm, Command *Cmd, double arg_val, int mask)
{
    if(Asm->cur_pos + LIMIT_DIFRCE > Asm->code_arr_capct)
    {
        Asm->code_arr_capct *= 2;

        Asm->code_array = (double*) recalloc (Asm->code_array,
                                              Asm->code_arr_capct,
                                              Asm->cur_pos,
                                              sizeof (double)     );
    }

    Asm->code_array[Asm->cur_pos + 1] = arg_val;
    Cmd->mask |= mask;
    Asm->offset++;
}

//-----------------------------------------------------------------------------

void parse_cmd (Assembler *Asm, Command Cmd)
{
    int flag_found = 0;

    char *cmd_names[] =
    {
        #define CMD_DEF(cmd, name, code, ...)\
        name,

        #include "../include/codegen.h"

        #undef CMD_DEF
    };

    for(int num_cmd = 0; num_cmd < NUM_OF_SUP_CMD; num_cmd++)
    {
        if(stricmp (Cmd.title, cmd_names[num_cmd]) == 0)
        {
            Asm->code_array[Asm->cur_pos] = num_cmd | Cmd.mask;
            Asm->cur_pos += Asm->offset;
            flag_found++;
            printf  ("a");

            break;
        }
    }


    if(flag_found == 0) Asm->Info.code_sgntr = WRONG_SIGN;
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
}

//-----------------------------------------------------------------------------

void write_res_sums (Assembler *Asm)
{
    Asm->code_array[0]  = Asm->code_arr_size;
    Asm->code_array[1]  = Asm->Info.code_sgntr;
    Asm->label_array[0] = Asm->label_arr_size;
}

//-----------------------------------------------------------------------------

void asm_dump (Assembler *Asm)
{
    FILE *code_dmp_file  = fopen ("../dump/code_asm_dump.txt", "w+");
    FILE *label_dmp_file = fopen ("../dump/label_asm_dump.txt", "w+");

    for(int i = 0; i < Asm->code_array[0]; i++)
    {
        fprintf (code_dmp_file, "%06d || %lg\n", i, Asm->code_array[i]);
    }

    for(int i = 0; i <= Asm->label_array[0]; i++)
    {
        fprintf (label_dmp_file, "%06d || %d\n", i, Asm->label_array[i]);
    }

    fclose  (code_dmp_file);
    fclose  (label_dmp_file);
}

//-----------------------------------------------------------------------------



