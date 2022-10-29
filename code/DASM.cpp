#include "../include/DASM.h"

//-----------------------------------------------------------------------------

void disassembler ()
{
    FILE *code_file_  = fopen ("../files/code.bin",     "rb");
    FILE *label_file_ = fopen ("../files/labels.bin",   "rb");

    int *labels = NULL;

    double *code = NULL;

    fseek (label_file_, 0, SEEK_SET);

    read_label_file (label_file_, &labels);

    read_code_file (code_file_, &code);

    file_maker (code, labels);

    fclose (code_file_);
    fclose (label_file_);

    free (labels);
    free (code);
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

        code_dump ((*code_), res_sum, code_sgntr);
    }

    else
    {
        printf ("|Wrong signature!|\n");
    }
}

//-----------------------------------------------------------------------------

void file_maker (double *code_, int *labels_)
{
    FILE *file_out = fopen ("../files/file_out.txt", "w+");

    for(int ip = 1; ip <= (int) code_[0]; ip++)
    {
        int cmd_d    = code_[ip];
        double arg_d = 0;

        arg_d = code_[ip + 1];

        label_input (ip, labels_, file_out);

        handle_cmds (cmd_d, arg_d, &ip, file_out);
    }

    fclose (file_out);
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

        for(int num_cmd = 0; num_cmd < Num_sup_cmd + Num_sup_jmps; num_cmd++)
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

bool is_equal (double a, double b)
{
    const double EPS = 1e-1;

    return (a - b < EPS && a - b > -EPS);
}


//-----------------------------------------------------------------------------

void handle_cmds (int cmd_d, double arg_d, int *ipp, FILE *file_out)
{
    int ip = *ipp;

    #define CMD_(cmd, code) \
        case cmd:           \
            code            \
            break;

    switch (cmd_d)
    {
        #include "../include/dsmcodegen.h"

        default:
            fprintf (file_out, "| %d - ?\n", cmd_d);
            break;
    }

    #undef CMD_

    *ipp = ip;
}

//-----------------------------------------------------------------------------

void label_input (int ip, int *labels, FILE *file_out)
{
    int num_of_lbls = labels[0];

    for(int i = 1; i <= num_of_lbls; i++)
    {
        if(labels[i] + 1 == ip && labels[i] > 0)
        {
            fprintf (file_out, ":%d\n", i);
        }
    }
}

//-----------------------------------------------------------------------------

void code_dump (double *code, int size, int32_t code_sgntr)
{
    FILE *code_dmp_file  = fopen ("../dump/code_dump.txt", "w+");

    fprintf (code_dmp_file,
             "\n________________________CODE_DUMP__________________________\n\n"
             "|RES SUM|   - %lg\n"
             "|Signature| - %x\n", code[0], code_sgntr);

    for(int i = 1; i <= size; i++)
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
            fprintf (code_dmp_file, "0");
        }

        fprintf (code_dmp_file, "%d || %lg\n", i, code[i]);
    }

    fprintf (code_dmp_file, "___________________________________________________________\n\n");

    fclose (code_dmp_file);
}

//-----------------------------------------------------------------------------

void label_dump (int *label, int size)
{
    FILE *label_dmp_file  = fopen ("../dump/label_dump.txt", "w+");

    fprintf (label_dmp_file,
             "\n________________________LABEL_DUMP__________________________\n\n"
             "|RES SUM|   - %d\n", label[0]);

    for(int i = 1; i <= size; i++)
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
            fprintf (label_dmp_file, "0");
        }

        fprintf (label_dmp_file, "%d || %d\n", i, label[i]);
    }

    fprintf (label_dmp_file, "____________________________________________________________\n\n");

    fclose (label_dmp_file);
}

//-----------------------------------------------------------------------------

int main ()
{
    disassembler ();

    return 0;
}

//-----------------------------------------------------------------------------

