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

    assembler_dtor (&Asm);

    return 0;
}

//-----------------------------------------------------------------------------

int assembler_ctor (Assembler *Asm, char *argv[])
{
    Asm->Code.array  = (double*) calloc (SZ(CODE_INIT),  sizeof (double));
    Asm->Label.array = (int*)    calloc (SZ(LABEL_INIT), sizeof (int));

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

    Asm->Code.size      = SG(CODE_OFFSET);
    Asm->Code.capacity  = SZ(CODE_INIT);

    Asm->Label.size     = SG(LABEL_OFFSET);
    Asm->Label.capacity = SZ(LABEL_INIT);

    Asm->Code.dmp_file  = fopen ("../dump/code_asm_dump.txt",  "w+");
    Asm->Label.dmp_file = fopen ("../dump/label_asm_dump.txt", "w+");

    Asm->cur_pos = SG(CODE_OFFSET);
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

    handle_text (Asm, Text, File_input);

    write_res_sums (Asm);

    clear_mem (Text, File_input);

    fwrite (Asm->Code.array, sizeof(double), Asm->Code.size, Asm->Info.code_file);
}

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Asm->Info.double_pass)

void handle_text (Assembler *Asm, Line *Text, File *File_input)
{
    DOUBLE_PASS = false;

    for(int i = 0; i < File_input->num_of_lines; i++)
    {
        Command Cmd  = { 0 };
        Argument Arg = { 0 };

        Asm->Cur_line = Text[i];

        parse_line (Asm, &Cmd, &Arg);

        write_in_code (Asm, Cmd, Arg);
    }
    Asm->Code.size = Asm->cur_pos;
    Asm->cur_pos = SG(CODE_OFFSET);

    if(DOUBLE_PASS) handle_text (Asm, Text, File_input);
}

#undef DOUBLE_PASS

//-----------------------------------------------------------------------------

void parse_line (Assembler *Asm, Command *Cmd, Argument *Arg)
{
    parse_label (Asm, Arg);

    parse_cmd   (Asm, Cmd, Arg);

    parse_arg   (Asm, Cmd, Arg);
}

//-----------------------------------------------------------------------------

void parse_label (Assembler *Asm, Argument *Arg)
{
    if(sscanf (Asm->Cur_line.begin_line, ":%lg", &Arg->value) == 1)
    {
        if(Arg->value > Asm->Label.size)
        {
            Asm->Label.size = (int) Arg->value;
        }

        Arg->flag = true;
    }
}

//-----------------------------------------------------------------------------

void parse_cmd (Assembler *Asm, Command *Cmd, Argument *Arg)
{
    char cmd_name[L(MAX_LEN)] = "";

    if(!Arg->flag && sscanf (Asm->Cur_line.begin_line, "%20s", cmd_name) == 1)
    {
        char *cmd_names[] =
        {
            #define CMD_DEF(cmd, name, code, ...)\
            name,

            //-----------------------------------------------------------------------------

            #include "../include/codegen.h"

            //-----------------------------------------------------------------------------

            #undef CMD_DEF
        };

        for(int num_cmd = 0; num_cmd < N(SUPPORTED_CMD); num_cmd++)
        {
            if(stricmp (cmd_name, cmd_names[num_cmd]) == 0)
            {
                Cmd->number = num_cmd;
                Cmd->flag = true;

                break;
            }

        }

        if(!Cmd->flag) Asm->Info.code_sgntr = SIGNATURE_DESTROYED;
    }
}

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Asm->Info.double_pass)

void parse_arg (Assembler *Asm, Command *Cmd, Argument *Arg)
{
    if(Cmd->flag)
    {
        if(strchr (Asm->Cur_line.begin_line, ':') != NULL)
        {
            if(sscanf (Asm->Cur_line.begin_line, "%*s %lg", &Arg->value) == 1)
            {
                Cmd->mask |= MASK_IMM;

                if(Arg->value > Asm->Label.size || Asm->Label.array[(int) Arg->value] <= 0)
                {
                    Arg->value = -1;

                    DOUBLE_PASS = true;
                }

                else
                {
                    Arg->value = Asm->Label.array[(int) Arg->value];
                }
            }

            else
            {
                Asm->Info.code_sgntr = SIGNATURE_DESTROYED;
            }
        }

        #define PARSE_ARG(num, name_msk, format, ...)                                \
        else if(sscanf (Asm->Cur_line.begin_line, format, __VA_ARGS__) == num)      \
        {                                                                            \
            Cmd->mask |= name_msk;                                                   \
        }                                                                            \

        //-----------------------------------------------------------------------------

        #include "../include/asm_codegen.h"

        //-----------------------------------------------------------------------------

        #undef PARSE_ARG
    }
}

#undef DOUBLE_PASS

//-----------------------------------------------------------------------------

void write_in_code (Assembler *Asm, Command Cmd, Argument Arg)
{
    if(!Cmd.flag)
    {
        if(Arg.flag)
        {
            if(Asm->Label.size + L(SIZE_DIFF) > Asm->Label.capacity)
            {
                Asm->Label.capacity *= 2;

                Asm->Label.array = (int*) recalloc (Asm->Label.array,
                                                    Asm->Label.capacity,
                                                    Asm->Label.size,
                                                    sizeof (int)         );
            }

            if(Arg.value > Asm->Label.size)
            {
                Asm->Label.size = (int) Arg.value;
            }

            Asm->Label.array[(int) Arg.value] = Asm->cur_pos;
        }
    }

    else
    {
        Asm->Code.array[Asm->cur_pos++] = Cmd.number |= Cmd.mask;

        if(Asm->cur_pos + L(SIZE_DIFF) > Asm->Code.capacity)
        {
            Asm->Code.capacity *= 2;

            Asm->Code.array = (double*) recalloc (Asm->Code.array,
                                                  Asm->Code.capacity,
                                                  Asm->cur_pos,
                                                  sizeof (double)    );
        }

        if(Cmd.number & MASK_REG)
        {
            Asm->Code.array[Asm->cur_pos++] = Arg.reg_sym - 'a';
        }

        if(Cmd.number & MASK_IMM)
        {
            Asm->Code.array[Asm->cur_pos++] = Arg.value;
        }
    }

    Asm->Code.array[0] = Asm->Code.size;
    Asm->Code.array[1] = Asm->Info.code_sgntr;

    asm_dump (Asm);
}

//-----------------------------------------------------------------------------

void assembler_dtor (Assembler *Asm)
{
    free (Asm->Code.array);
    free (Asm->Label.array);

    Asm->Code.size      = -1;
    Asm->Code.capacity  = -1;
    Asm->Label.size     = -1;
    Asm->Label.capacity = -1;

    fclose (Asm->Code.dmp_file);
    fclose (Asm->Label.dmp_file);

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
    fprintf (Asm->Code.dmp_file, "_______________________________________________________________________________\n"
                                 "size: %d sign: %x\n",
                                 (int) Asm->Code.array[0], (int) Asm->Code.array[1]);

    for(int i = 2; i < Asm->Code.size; i++)
    {
        fprintf (Asm->Code.dmp_file, "%06d || %lg\n", i, Asm->Code.array[i]);
    }

    for(int i = 0; i <= Asm->Label.size; i++)
    {
        fprintf (Asm->Label.dmp_file, "_______________________________________________________________________________\n"
                                      "%06d || %d\n",
                                      i, Asm->Label.array[i]);
    }
}

//-----------------------------------------------------------------------------



