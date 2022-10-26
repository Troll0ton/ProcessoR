CMD_(stk, CMD_PUSH_, arg_d, ip,
{
    stack_push (stk, arg_d);
    ip++;
})

CMD_(stk, CMD_RG_PUSH_, arg_d, ip,
{
    stack_push (stk, arg_d);
    ip++;
})

CMD_(stk, CMD_RM_PUSH_, arg_d, ip,
{
    stack_push (stk, arg_d);
    ip++;
})

CMD_(stk, CMD_ADD_, arg_d, ip,
{
    stack_push (stk, stack_pop (stk) + stack_pop (stk));
})

CMD_(stk, CMD_SUB_, arg_d, ip,
{
    stack_push (stk, -(stack_pop (stk) - stack_pop (stk)));
})

CMD_(stk, CMD_MUL_, arg_d, ip,
{
    stack_push (stk, stack_pop (stk) * stack_pop (stk));
})

CMD_(stk, CMD_DIV_, arg_d, ip,
{
    stack_push (stk, 1 / stack_pop (stk) * stack_pop (stk));
})

CMD_(stk, CMD_HLT_, arg_d, ip,
{
    printf ("");
})

CMD_(stk, CMD_OUT_, arg_d, ip,
{
    printf ("result: %lg\n", stack_pop (stk));
})

CMD_(stk, CMD_DUMP_, arg_d, ip,
{
    stack_dumps (stk, file_log);
})

CMD_(stk, CMD_JB_, arg_d, ip,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 < f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch];
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(stk, CMD_JBE_, arg_d, ip,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 <= f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch];
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(stk, CMD_JA_, arg_d, ip,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 > f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch];
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(stk, CMD_JAE_, arg_d, ip,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 >= f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch];
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(stk, CMD_JE_, arg_d, ip,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch];
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(stk, CMD_JNE_, arg_d, ip,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(!is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch];
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})
