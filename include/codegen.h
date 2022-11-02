CMD_(CMD_PUSH,
{
    stack_push (&Cpu->Stk, arg_d);
    ip++;
})

CMD_(CMD_POP,
{
    if((int) Cpu->code[ip] & MASK_RAM && (int) Cpu->code[ip] & MASK_REG)
    {
        Cpu->ram[Cpu->regs[(int) Cpu->code[ip + 1]]]  = stack_pop (&Cpu->Stk);
    }

    else if((int) Cpu->code[ip] & MASK_REG)
    {
        Cpu->regs[(int) Cpu->code[ip + 1]] = stack_pop (&Cpu->Stk);
    }

    else if((int) Cpu->code[ip] & MASK_RAM)
    {
        Cpu->ram[(int) Cpu->code[ip + 1]]  = stack_pop (&Cpu->Stk);
    }

    else stack_pop (&Cpu->Stk);

    ip++;
})

CMD_(CMD_ADD,
{
    stack_push (&Cpu->Stk, stack_pop (&Cpu->Stk) + stack_pop (&Cpu->Stk));
})

CMD_(CMD_SUB,
{
    stack_push (&Cpu->Stk, -(stack_pop (&Cpu->Stk) - stack_pop (&Cpu->Stk)));
})

CMD_(CMD_MUL,
{
    stack_push (&Cpu->Stk, stack_pop (&Cpu->Stk) * stack_pop (&Cpu->Stk));
})

CMD_(CMD_DIV,
{
    stack_push (&Cpu->Stk, 1 / stack_pop (&Cpu->Stk) * stack_pop (&Cpu->Stk));
})

CMD_(CMD_HLT,
{
    printf ("");
})

CMD_(CMD_OUT,
{
    printf ("result: %lg\n", stack_pop (&Cpu->Stk));
})

CMD_(CMD_DUMP,
{
    stack_dumps (&Cpu->Stk, Cpu->Info.log_file);
})

CMD_(CMD_JB,
{
    f2 = stack_pop (&Cpu->Stk);
    f1 = stack_pop (&Cpu->Stk);
    if(f1 < f2)
    {
        int pos_ch = arg_d;
        ip = Cpu->labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (&Cpu->Stk, f1);
    stack_push (&Cpu->Stk, f2);
})

CMD_(CMD_JBE,
{
    f2 = stack_pop (&Cpu->Stk);
    f1 = stack_pop (&Cpu->Stk);
    if(f1 <= f2)
    {
        int pos_ch = arg_d;
        ip = Cpu->labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (&Cpu->Stk, f1);
    stack_push (&Cpu->Stk, f2);
})

CMD_(CMD_JA,
{
    f2 = stack_pop (&Cpu->Stk);
    f1 = stack_pop (&Cpu->Stk);
    if(f1 > f2)
    {
        int pos_ch = arg_d;
        ip = Cpu->labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (&Cpu->Stk, f1);
    stack_push (&Cpu->Stk, f2);
})

CMD_(CMD_JAE,
{
    f2 = stack_pop (&Cpu->Stk);
    f1 = stack_pop (&Cpu->Stk);
    if(f1 >= f2)
    {
        int pos_ch = arg_d;
        ip = Cpu->labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (&Cpu->Stk, f1);
    stack_push (&Cpu->Stk, f2);
})

CMD_(CMD_JE,
{
    f2 = stack_pop (&Cpu->Stk);
    f1 = stack_pop (&Cpu->Stk);
    if(is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = Cpu->labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (&Cpu->Stk, f1);
    stack_push (&Cpu->Stk, f2);
})

CMD_(CMD_JNE,
{
    f2 = stack_pop (&Cpu->Stk);
    f1 = stack_pop (&Cpu->Stk);
    if(!is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = Cpu->labels[pos_ch] - 2;
    }
    else ip++;
    stack_push (&Cpu->Stk, f1);
    stack_push (&Cpu->Stk, f2);
})
