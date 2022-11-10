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
    Asm->Code.array  = (char*) calloc (CODE_SIZE_INIT,  sizeof (char));
    Asm->Label.array = (int*)  calloc (LABEL_SIZE_INIT, sizeof (int));

    if(Asm->Code.array == NULL)
    {
        printf ("|ERROR - Null pointer, code array|\n");
        return ERR_CTOR;
    }

    if(Asm->Label.array == NULL)
    {
        printf ("|ERROR - Null pointer, label array|\n");
        return ERR_CTOR;
    }

    Asm->Code.size      = NUM_FRST_EL_CD;
    Asm->Code.capacity  = CODE_SIZE_INIT;

    Asm->Label.size     = NUM_FRST_EL_LB;
    Asm->Label.capacity = LABEL_SIZE_INIT;

    Asm->cur_pos = NUM_FRST_EL_CD;
    Asm->offset  = 0;

    Asm->Info     = { 0 };
    Asm->Cur_line = { 0 };

    return asm_info_ctor (&Asm->Info, argv);
}

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Info->double_pass)

int asm_info_ctor (Asm_info *Info, char *argv[])
{
    Info->code_sgntr = SIGNATURE;

    Info->file_in   = fopen ((char*) argv[1],       "rb");
    Info->code_file = fopen ("../files/code.bin",   "wb");

    DOUBLE_PASS = false;

    if(Info->file_in    == NULL ||
       Info->code_file  == NULL   )
    {
        printf ("|ERROR - file opening|\n");

        return ERR_CTOR;
    }

    return 0;
}

#undef DOUBLE_PASS

//-----------------------------------------------------------------------------

void assembling (Assembler *Asm)
{
    File *File_input = file_reader (Asm->Info.file_in);
    Line *Text = lines_separator (File_input);

    parse_text (Asm, Text, File_input);

    write_res_sums (Asm);

    clear_mem (Text, File_input);

    fwrite (Asm->Code.array, sizeof(double), Asm->Code.size, Asm->Info.code_file);
}

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Info->double_pass)

void parse_text (Assembler *Asm, Line *Text, File *File_input)
{
    DOUBLE_PASS = false;

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        Asm->Cur_line = Text[i];

        handle_line (Asm);
    }

    Asm->Code.size = Asm->cur_pos;
    Asm->cur_pos = NUM_FRST_EL_CD;

    if(DOUBLE_PASS) parse_text (Asm, Text, File_input);
}

#undef DOUBLE_PASS

//-----------------------------------------------------------------------------

void parse_line (Assembler *Asm)
{
    Command Cmd  = { 0 };
    Argument Arg = { 0 };

    Cmd.flag = false;
    Arg.flag = false;

    parse_label (Asm, &Arg);

    parse_cmd (Asm, &Cmd);

    parse_arg (Asm, &Cmd);
}

//-----------------------------------------------------------------------------

void parse_label (Assembler *Asm, Argument Arg)
{
    if(sscanf (Asm->Cur_line.begin_line, ":%d", Arg->value) == 1)
    {
        if(Asm->Label.size + LIMIT_DIFFERENCE > Asm->Label.capacity)
        {
            Asm->Label.capacity *= 2;

            Asm->Label.array = (int*) recalloc (Asm->Label.array,
                                                Asm->Label.capacity,
                                                Asm->Label.size,
                                                sizeof (int)         );
        }

        if(Arg->value > Asm->Label.size)
        {
            Asm->Label.size = Arg->value;
        }

        Arg->flag = true;
    }
}

//-----------------------------------------------------------------------------

void parse_cmd (Assembler *Asm, Command *Cmd, Argument Arg)
{
    if(!Arg->flag && sscanf (Asm->Cur_line.begin_line, "%20s", Cmd->name) == 1)
    {
        char *cmd_names[] =
        {
            #define CMD_DEF(cmd, name, code, ...)\
            name,

            #include "../include/codegen.h"

            #undef CMD_DEF
        };

        for(int num_cmd = 0; num_cmd < NUM_OF_SUP_CMD; num_cmd++)
        {
            if(stricmp (Cmd.name, cmd_names[num_cmd]) == 0)
            {
                Cmd->number = num_cmd;
                Cmd->flag = true;

                break;
            }

        }

        if(!Cmd->flag) Asm->Info.code_sgntr = SIGNATURE_DESTROYED;
    }

    //Asm->Code.array[Asm->cur_pos] = num_cmd | Cmd.mask;
    //Asm->cur_pos += (Asm->offset + BASIC_OFFSET);
}

//-----------------------------------------------------------------------------

void parse_arg (Assembler *Asm, Command *Cmd)
{
    Cmd->mask     = 0;
    Asm->offset   = 0;

    int    label   = 0;
    char   reg_sym = 0;
    double arg_val = 0;

    if(sscanf (Asm->Cur_line.begin_line, "%20s", Cmd->name) == 1)
    {
        Cmd->flag_cmd++;
    }

    parse_jmp (Asm, Arg);

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

    else
    {
        // error
    }
}

//-----------------------------------------------------------------------------

void parse_jmp (Assembler *Asm, Command *Cmd, int label)
{
    if(Cmd->flag && strchr (Asm->Cur_line.begin_line, ':') != NULL)
    {
        if(sscanf (Asm->Cur_line.begin_line, "%*s %d", &label) == 1)
        {
            if(label <= Asm->Label.size && Asm->Label.array[label] > 0)
            {
                write_in_arg (Asm, Cmd, Asm->Label.array[label], MASK_IMM);
            }

            else
            {
                write_in_arg (Asm, Cmd, -1, MASK_IMM);

                Asm->Info.double_pass = true;
            }
        }

        else
        {
            Asm->Info.code_sgntr = SIGNATURE_DESTROYED;
        }
    }
}

//-----------------------------------------------------------------------------

void write_in_arg (Assembler *Asm, Command *Cmd, double arg_val, int mask)
{
    if(Asm->cur_pos + LIMIT_DIFFERENCE > Asm->code_arr_capct)
    {
        Asm->code_arr_capct *= 2;

        Asm->Code.array = (double*) recalloc (Asm->Code.array,
                                              Asm->code_arr_capct,
                                              Asm->cur_pos,
                                              sizeof (double)     );
    }

    Asm->Code.array[Asm->cur_pos + 1] = arg_val;
    Cmd->mask |= mask;
    Asm->offset++;
}

//-----------------------------------------------------------------------------

void assembler_dtor (Assembler *Asm)
{
    free (Asm->Code.array);
    free (Asm->Label.array);

    Asm->Code.size   = -1;
    Asm->code_arr_capct  = -1;
    Asm->Label.size  = -1;
    Asm->Label.capacity = -1;

    // SIGNATURE_DESTROYED
    Asm->Info.code_sgntr = SIGNATURE_DESTROYED;

    asm_info_dtor (&Asm->Info);
}

//-----------------------------------------------------------------------------

void asm_info_dtor (Asm_info *Info)
{
    Info->code_sgntr = SIGNATURE_DESTROYED;

    fclose (Info->file_in);
    fclose (Info->code_file);
}

//-----------------------------------------------------------------------------

void write_res_sums (Assembler *Asm)
{
    Asm->Code.array[0]  = Asm->Code.size;
    Asm->Code.array[1]  = Asm->Info.code_sgntr;
    Asm->Label.array[0] = Asm->Label.size;
}

//-----------------------------------------------------------------------------

void asm_dump (Assembler *Asm)
{
    FILE *code_dmp_file  = fopen ("../dump/code_asm_dump.txt",  "w+");
    FILE *label_dmp_file = fopen ("../dump/label_asm_dump.txt", "w+");

    for(int i = 0; i < Asm->Code.size; i++)
    {
        fprintf (code_dmp_file, "%06d || %lg\n", i, Asm->Code.array[i]);
    }

    for(int i = 0; i <= Asm->Label.size; i++)
    {
        fprintf (label_dmp_file, "%06d || %d\n", i, Asm->Label.array[i]);
    }

    fclose  (code_dmp_file);
    fclose  (label_dmp_file);
}

//-----------------------------------------------------------------------------



