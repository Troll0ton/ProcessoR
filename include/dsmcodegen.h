CMD_(CMD_PUSH_, arg_d, ip,
{
    fprintf (file_out, "push %lg\n", arg_d);
    ip++;
})

CMD_(CMD_RG_PUSH_, arg_d, ip,
{
    fprintf (file_out, "push r%cx\n", (int) arg_d + 97);
    ip++;
})

CMD_(CMD_RM_PUSH_, arg_d, ip,
{
    fprintf (file_out, "push [%d]\n", (int) arg_d);
    ip++;
})

CMD_(CMD_ADD_, arg_d, ip,
{
    fprintf (file_out, "add\n");
})

CMD_(CMD_SUB_, arg_d, ip,
{
    fprintf (file_out, "sub\n");
})

CMD_(CMD_MUL_, arg_d, ip,
{
    fprintf (file_out, "mul\n");
})

CMD_(CMD_DIV_, arg_d, ip,
{
    fprintf (file_out, "div\n");
})

CMD_(CMD_HLT_, arg_d, ip,
{
    fprintf (file_out, "hlt\n");
})

CMD_(CMD_OUT_, arg_d, ip,
{
    fprintf (file_out, "out\n");
})

CMD_(CMD_DUMP_, arg_d, ip,
{
    fprintf (file_out, "dump\n");
})

CMD_(CMD_JB_, arg_d, ip,
{
    fprintf (file_out, "jb %d:\n", (int) arg_d);
})

CMD_(CMD_JBE_, arg_d, ip,
{
    fprintf (file_out, "jbe %d:\n", (int) arg_d);
})

CMD_(CMD_JA_, arg_d, ip,
{
    fprintf (file_out, "ja %d:\n", (int) arg_d);
})

CMD_(CMD_JAE_, arg_d, ip,
{
    fprintf (file_out, "jae %d:\n", (int) arg_d);
})

CMD_(CMD_JE_, arg_d, ip,
{
    fprintf (file_out, "je %d:\n", (int) arg_d);
})

CMD_(CMD_JNE_, arg_d, ip,
{
    fprintf (file_out, "jne %d:\n", (int) arg_d);
})
