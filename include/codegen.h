CMD_DEF(HLT,
{
    printf ("");
})

CMD_DEF(PUSH,
{
    stack_push (&Cpu->Stk, arg_d);
    ip++;
})

CMD_DEF(POP,
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

    else
    {
        stack_pop (&Cpu->Stk);
        ip--;
    }

    ip++;
})

CMD_DEF(ADD,
{
    stack_push (&Cpu->Stk, stack_pop (&Cpu->Stk) + stack_pop (&Cpu->Stk));
})

CMD_DEF(SUB,
{
    stack_push (&Cpu->Stk, -(stack_pop (&Cpu->Stk) - stack_pop (&Cpu->Stk)));
})

CMD_DEF(MUL,
{
    stack_push (&Cpu->Stk, stack_pop (&Cpu->Stk) * stack_pop (&Cpu->Stk));
})

CMD_DEF(DIV,
{
    stack_push (&Cpu->Stk, 1 / stack_pop (&Cpu->Stk) * stack_pop (&Cpu->Stk));
})

CMD_DEF(OUT,
{
    printf ("result: %lg\n", stack_pop (&Cpu->Stk));
})

CMD_DEF(DUMP,
{
    stack_dumps (&Cpu->Stk, Cpu->Info.log_file);
})

CMD_DEF(JBE,
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

CMD_DEF(JAE,
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

CMD_DEF(JA,
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

CMD_DEF(JB,
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

CMD_DEF(JE,
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

CMD_DEF(JNE,
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
