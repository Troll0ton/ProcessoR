#include "../include/ASM.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    Assembler Asm = { 0 };

    if(assembler_ctor (&Asm, argv) == E(CTOR))
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
    Asm->Code.array  = (char*) calloc (SZ(CODE_INIT),  sizeof (char));
    Asm->Label.array = (int*)  calloc (SZ(LABEL_INIT), sizeof (int));

    if(Asm->Code.array == NULL)
    {
        printf ("|ERROR - Null pointer, code array|\n");

        return E(CTOR);
    }

    if(Asm->Label.array == NULL)
    {
        printf ("|ERROR - Null pointer, label array|\n");

        return E(CTOR);
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
    Info->code_signature = SIGNATURE;

    Info->file_in   = fopen ((char*) argv[1],     "rb");
    Info->code_file = fopen ("../files/code.bin", "wb");

    DOUBLE_PASS = false;

    if(Info->file_in   == NULL ||
       Info->code_file == NULL   )
    {
        printf ("|ERROR - file opening|\n");

        return E(CTOR);
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

    fwrite (Asm->Code.array, sizeof(char), Asm->Code.size, Asm->Info.code_file);
}

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Asm->Info.double_pass)

void parse_text (Assembler *Asm, Line *Text, File *File_input)
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
    Asm->cur_pos   = SG(CODE_OFFSET);

    if(DOUBLE_PASS) parse_text (Asm, Text, File_input);
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
    if(lscan(":%lg", &Arg->value) == 1)
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
    char cmd_name[LM(MAX_LEN)] = "";

    if(!Arg->flag && lscan("%20s", cmd_name) == 1)
    {
        char *cmd_names[] =
        {
            #define CMD_DEF(cmd, name, code, ...) \
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
                Cmd->code = num_cmd;
                Cmd->flag = true;

                break;
            }
        }

        if(!Cmd->flag) Asm->Info.code_signature = SIGNATURE_DESTROYED;
    }
}

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Asm->Info.double_pass)

void parse_arg (Assembler *Asm, Command *Cmd, Argument *Arg)
{
    if(Cmd->flag)
    {
        char unexpected_line[LM(MAX_LEN)] = "";

        if(strchr (Asm->Cur_line.begin_line, ':') != NULL)
        {
            if(lscan("%*s %lg", &Arg->value) == 1)
            {
                Cmd->mask |= MASK_IMM;

                search_label (Asm, Arg);
            }

            else
            {
                Asm->Info.code_signature = SIGNATURE_DESTROYED;
            }
        }

        #define PARSE_ARG(num, name_msk, format, ...) \
        else if(lscan(format, __VA_ARGS__) == num)    \
        {                                             \
            Cmd->mask |= name_msk;                    \
        }                                             \

        //-----------------------------------------------------------------------------

        #include "../include/asm_codegen.h"

        //-----------------------------------------------------------------------------

        #undef PARSE_ARG

        else if(lscan("%*s %20s", unexpected_line) == 1)
        {
            printf ("ERROR - UNEXPECTED LINE!\n");

            Asm->Info.code_signature = SIGNATURE_DESTROYED;
        }
    }
}

#undef DOUBLE_PASS

//-----------------------------------------------------------------------------

#define DOUBLE_PASS (Asm->Info.double_pass)

void search_label (Assembler *Asm, Argument *Arg)
{
    if(Arg->value > Asm->Label.size || Asm->Label.array[(int) Arg->value] <= 0)
    {
        Arg->value = NOT_FOUND;

        DOUBLE_PASS = true;
    }

    else
    {
        Arg->value = Asm->Label.array[(int) Arg->value];
    }
}

#undef DOUBLE_PASS

//-----------------------------------------------------------------------------

void write_in_code (Assembler *Asm, Command Cmd, Argument Arg)
{
    if(Asm->cur_pos + LM(SIZE_DIFF) > Asm->Code.capacity)
    {
        Asm->Code.capacity *= PAR_INCREASE;

        Asm->Code.array = (char*) recalloc (Asm->Code.array,
                                            Asm->Code.capacity,
                                            Asm->cur_pos,
                                            sizeof (char)      );
    }

    if(!Cmd.flag && Arg.flag)
    {
        write_in_label (Asm, Arg);
    }

    else if(Cmd.flag)
    {
        Asm->Code.array[Asm->cur_pos] = (char) (Cmd.code |= Cmd.mask);
        Asm->cur_pos += CMD_OFFSET;

        write_in_arg (Asm, Cmd, Arg);
    }

    *(elem_t*)(Asm->Code.array) = Asm->Code.size;
    *(elem_t*)(Asm->Code.array + ARG_OFFSET) = Asm->Info.code_signature;
}

//-----------------------------------------------------------------------------

void write_in_label (Assembler *Asm, Argument Arg)
{
    if(Asm->Label.size + LM(SIZE_DIFF) > Asm->Label.capacity)
    {
        Asm->Label.capacity *= 2;

        Asm->Label.array = (int*) recalloc (Asm->Label.array,
                                            Asm->Label.capacity,
                                            Asm->Label.size,
                                            sizeof (int)        );
    }

    if(Arg.value > Asm->Label.size)
    {
        Asm->Label.size = (int) Arg.value;
    }

    Asm->Label.array[(int) Arg.value] = Asm->cur_pos;
}

//-----------------------------------------------------------------------------

void write_in_arg (Assembler *Asm, Command Cmd, Argument Arg)
{
    if(Cmd.code & MASK_REG)
    {
        *(elem_t*)(Asm->Code.array + Asm->cur_pos) = (elem_t) (Arg.reg_sym - 'a');

        Asm->cur_pos += ARG_OFFSET;
    }

    if(Cmd.code & MASK_IMM)
    {
        *(elem_t*)(Asm->Code.array + Asm->cur_pos) = (elem_t) Arg.value;

        Asm->cur_pos += ARG_OFFSET;
    }
}

//-----------------------------------------------------------------------------

void assembler_dtor (Assembler *Asm)
{
    free (Asm->Code.array);
    free (Asm->Label.array);

    Asm->Code.size      = DELETED_PAR;
    Asm->Code.capacity  = DELETED_PAR;
    Asm->Label.size     = DELETED_PAR;
    Asm->Label.capacity = DELETED_PAR;

    fclose (Asm->Code.dmp_file);
    fclose (Asm->Label.dmp_file);

    asm_info_dtor (&Asm->Info);
}

//-----------------------------------------------------------------------------

void asm_info_dtor (Asm_info *Info)
{
    Info->code_signature = SIGNATURE_DESTROYED;

    fclose (Info->file_in);
    fclose (Info->code_file);
}

//-----------------------------------------------------------------------------

void write_res_sums (Assembler *Asm)
{
    *(elem_t*)(Asm->Code.array + 0)          = Asm->Code.size;
    *(elem_t*)(Asm->Code.array + ARG_OFFSET) = Asm->Info.code_signature;

    Asm->Label.array[0] = Asm->Label.size;
}

//-----------------------------------------------------------------------------

void asm_dump (Assembler *Asm)
{
    FILE *code_dmp_file = fopen ("../dump/code_asm_dump.txt", "w+");

    int i = 1;

    fprintf (code_dmp_file, "%d - size, %x - signature\n",
                            (int) *(elem_t*)(Asm->Code.array + 0),
                            (int) *(elem_t*)(Asm->Code.array + ARG_OFFSET));

    for(int curr_pos = SG(CODE_OFFSET); curr_pos < Asm->Code.size; curr_pos++)
    {
        char curr_cmd = *(Asm->Code.array + curr_pos);
        int  offset   = 0;

        fprintf (code_dmp_file, "%06d - logic pos, %06d - phys pos || %d\n",
                                i++, curr_pos, (int) Asm->Code.array[curr_pos]);

        if(curr_cmd & MASK_REG)
        {
            fprintf (code_dmp_file, "%06d - logic pos, %06d - phys pos || %lg\n",
                                    i++, curr_pos + CMD_OFFSET,
                                    *(elem_t*)(Asm->Code.array + curr_pos + CMD_OFFSET));

            offset += ARG_OFFSET;
        }

        if(curr_cmd & MASK_IMM)
        {
            fprintf (code_dmp_file, "%06d - logic pos, %06d - phys pos || %lg\n",
                                    i++, curr_pos + CMD_OFFSET + offset,
                                    *(elem_t*)(Asm->Code.array + curr_pos + CMD_OFFSET + offset));

            offset += ARG_OFFSET;
        }

        curr_pos += offset;
    }

    fclose (code_dmp_file);
}

//-----------------------------------------------------------------------------


