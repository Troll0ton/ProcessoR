CMD_(CMD_PUSH_,
{
    fprintf (file_out, "push %lg\n", arg_d);
    ip++;
})

CMD_(CMD_RG_PUSH_,
{
    fprintf (file_out, "push r%cx\n", (int) arg_d + 97);
    ip++;
})

CMD_(CMD_RM_PUSH_,
{
    fprintf (file_out, "push [%d]\n", (int) arg_d);
    ip++;
})

CMD_(CMD_ADD_,
{
    fprintf (file_out, "add\n");
})

CMD_(CMD_SUB_,
{
    fprintf (file_out, "sub\n");
})

CMD_(CMD_MUL_,
{
    fprintf (file_out, "mul\n");
})

CMD_(CMD_DIV_,
{
    fprintf (file_out, "div\n");
})

CMD_(CMD_HLT_,
{
    fprintf (file_out, "hlt\n");
})

CMD_(CMD_OUT_,
{
    fprintf (file_out, "out\n");
})

CMD_(CMD_DUMP_,
{
    fprintf (file_out, "dump\n");
})

CMD_(CMD_JB_,
{
    fprintf (file_out, "jb %d:\n", (int) arg_d);
})

CMD_(CMD_JBE_,
{
    fprintf (file_out, "jbe %d:\n", (int) arg_d);
})

CMD_(CMD_JA_,
{
    fprintf (file_out, "ja %d:\n", (int) arg_d);
})

CMD_(CMD_JAE_,
{
    fprintf (file_out, "jae %d:\n", (int) arg_d);
})

CMD_(CMD_JE_,
{
    fprintf (file_out, "je %d:\n", (int) arg_d);
})

CMD_(CMD_JNE_,
{
    fprintf (file_out, "jne %d:\n", (int) arg_d);
})
