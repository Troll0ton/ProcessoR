CMD_(CMD_PUSH_,
{
    stack_push (stk, arg_d);
    ip++;
})

CMD_(CMD_POP_,
{
    stack_pop (stk);
    ip++;
})

CMD_(CMD_ADD_,
{
    stack_push (stk, stack_pop (stk) + stack_pop (stk));
})

CMD_(CMD_SUB_,
{
    stack_push (stk, -(stack_pop (stk) - stack_pop (stk)));
})

CMD_(CMD_MUL_,
{
    stack_push (stk, stack_pop (stk) * stack_pop (stk));
})

CMD_(CMD_DIV_,
{
    stack_push (stk, 1 / stack_pop (stk) * stack_pop (stk));
})

CMD_(CMD_HLT_,
{
    printf ("");
})

CMD_(CMD_OUT_,
{
    printf ("result: %lg\n", stack_pop (stk));
})

CMD_(CMD_DUMP_,
{
    stack_dumps (stk, file_log);
})

CMD_(CMD_JB_,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 < f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(CMD_JBE_,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 <= f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(CMD_JA_,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 > f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(CMD_JAE_,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(f1 >= f2)
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(CMD_JE_,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})

CMD_(CMD_JNE_,
{
    f2 = stack_pop (stk);
    f1 = stack_pop (stk);
    if(!is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = data.labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (stk, f1);
    stack_push (stk, f2);
})
