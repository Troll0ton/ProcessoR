#include "../include/CPU.h"

//-----------------------------------------------------------------------------

void processor ()
{
    int regs[5] = {99, 98, 97, 96, 95};
    double ram[3] = {999,999,999};

    //-----------------------------------------------------------------------------

    Stack stk1;
    stack_ctor (&stk1, 2);

    FILE *code_file_  = fopen ("../files/code.txt", "rb");
    FILE *label_file_ = fopen ("../files/labels.txt", "rb");

    int *labels = NULL;

    double *code = NULL;

    fseek (label_file_, 0, SEEK_SET);

    read_label_file (label_file_, &labels);

    read_code_file (code_file_, &code);

    calculator (&stk1, code, regs, ram, labels);

    stack_dtor (&stk1);

    fclose (code_file_);
    fclose (label_file_);

    free (labels);
    free (code);
}

//-----------------------------------------------------------------------------

void code_dump (double *code, int size)
{
    printf ("\n________________________CODE_DUMP__________________________\n\n");

    for(int i = 0; i <= size; i++)
    {
        int num_nul = 0;

        int pow = 1;

        while(i / pow != 0)
        {
            pow *= 10;
            num_nul++;
        }

        if(num_nul == 0)num_nul++;

        for(int j = 0; j < 5 - num_nul; j++)
        {
            printf ("0");
        }

        printf ("%d || %lg\n", i, code[i]);
    }

    printf ("___________________________________________________________\n\n");
}

//-----------------------------------------------------------------------------

void label_dump (int *label, int size)
{
    printf ("\n________________________LABEL_DUMP__________________________\n\n");

    for(int i = 0; i <= size; i++)
    {
        int num_nul = 0;

        int pow = 1;

        while(i / pow != 0)
        {
            pow *= 10;
            num_nul++;
        }

        if(num_nul == 0)num_nul++;

        for(int j = 0; j < 5 - num_nul; j++)
        {
            printf ("0");
        }

        printf ("%d || %d\n", i, label[i]);
    }

    printf ("___________________________________________________________\n\n");
}

//-----------------------------------------------------------------------------

void read_label_file (FILE *label_file__, int **labels_)
{
    int res_lab = -1;

    fread (&res_lab, sizeof(int), 1, label_file__);

    *labels_ = (int*) calloc (res_lab + 1, sizeof (int));

    (*labels_)[0] = res_lab;

    int val1 = -1;

    for(int i = 1; i <= res_lab; i++)
    {
        fread (&val1, sizeof(int), 1, label_file__);
        (*labels_)[i] = val1;
    }

    label_dump ((*labels_), res_lab);
}

//-----------------------------------------------------------------------------

void read_code_file (FILE *code_file_, double **code_)
{
    int res_sum    = -1;
    int32_t code_sgntr = -1;

    fread (&res_sum, sizeof(int), 1, code_file_);
    fread (&code_sgntr, sizeof(int32_t), 1, code_file_);

    *code_ = (double*) calloc (res_sum + 1, sizeof (double));

    if(code_sgntr == Cor_signature)
    {
        fill_code_array (code_file_, res_sum, *code_);

        code_dump ((*code_), res_sum);
    }

    else
    {
        printf ("|Wrong signature!|\n");
    }
}

//-----------------------------------------------------------------------------

void calculator (Stack *stk_, double *code_, int *regs_, double *ram_, int *labels_)
{
    for(int ip = 1; ip <= (int) code_[0]; ip++)
    {
        int cmd_d = code_[ip];
        double arg_d = -1;

        if(cmd_d & ARG_REG) arg_d = regs_[(int) code_[ip + 1]];

        else if(cmd_d & ARG_RAM) arg_d = ram_[(int) code_[ip + 1]];

        else arg_d = code_[ip + 1];

        switch(cmd_d)
        {
            case CMD_PUSH_:
                stack_push (stk_, arg_d);
                ip++;
                break;
            case CMD_RG_PUSH_:
                stack_push (stk_, arg_d);
                ip++;
                break;
            case CMD_RM_PUSH_:
                stack_push (stk_, arg_d);
                ip++;
                break;
            case CMD_ADD_:
                stack_push (stk_, stack_pop (stk_) + stack_pop (stk_));
                break;
            case CMD_SUB_:
                stack_push (stk_, -(stack_pop (stk_) - stack_pop (stk_)));
                break;
            case CMD_MUL_:
                stack_push (stk_, stack_pop (stk_) * stack_pop (stk_));
                break;
            case CMD_DIV_:
                stack_push (stk_, 1 / stack_pop (stk_) * stack_pop (stk_));
                break;
            case CMD_HLT_:
                break;
            case CMD_OUT_:
                printf ("result: %lg\n", stack_pop (stk_));
                break;
            case CMD_DUMP_:
                printf ("|dump|\n");
                break;
            case CMD_JUMP_:
                if(arg_d > 0)
                {
                    int pos_ch = arg_d;
                    code_[ip + 1] = -1000;
                    ip = labels_[pos_ch] - 1;
                }
                else ip++;
                break;
            default:
                printf ("?%d \n", cmd_d);
                break;
        }

        stack_dump_ (stk_);
    }
}

//-----------------------------------------------------------------------------

void fill_code_array (FILE *code_file_, int res_sum_, double *code_)
{
    int cmd = -1;

    code_[0] = res_sum_;

    for(int ib = 1; ib <= res_sum_; ib++)
    {
        fread (&cmd, sizeof(int), 1, code_file_);

        code_[ib] = (double) cmd;

        for(int num_cmd = 0; num_cmd < Num_sup_cmd; num_cmd++)
        {
            if(Cmd_cpu[num_cmd].num == cmd && Cmd_cpu[num_cmd].par == 1)
            {
                ib++;
                double val = 0;
                fread (&val, sizeof(double), 1, code_file_);

                code_[ib] = val;
            }

            else if(Cmd_cpu[num_cmd].num == cmd && Cmd_cpu[num_cmd].par > 1)
            {
                ib++;
                int val2 = 0;
                fread (&val2, sizeof(int), 1, code_file_);

                code_[ib] = (double) val2;
            }
        }
    }
}

//-----------------------------------------------------------------------------

