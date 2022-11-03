CMD_DEF(HLT, "hlt",
{
    stack_push (&Cpu->Stk, STOP);
})

CMD_DEF(PUSH, "push",
{
    stack_push (&Cpu->Stk, arg_d);
    ip++;
})

CMD_DEF(POP, "pop",
{
    if((int) Cpu->code[ip] & MASK_RAM && (int) Cpu->code[ip] & MASK_REG)
    {
        Cpu->ram[Cpu->regs[(int) Cpu->code[ip + 1]]] = stack_pop (&Cpu->Stk);
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

CMD_DEF(ADD, "add",
{
    stack_push (&Cpu->Stk, stack_pop (&Cpu->Stk) + stack_pop (&Cpu->Stk));
})

CMD_DEF(SUB, "sub",
{
    stack_push (&Cpu->Stk, -(stack_pop (&Cpu->Stk) - stack_pop (&Cpu->Stk)));
})

CMD_DEF(MUL, "mul",
{
    stack_push (&Cpu->Stk, stack_pop (&Cpu->Stk) * stack_pop (&Cpu->Stk));
})

CMD_DEF(DIV, "div",
{
    stack_push (&Cpu->Stk, 1 / stack_pop (&Cpu->Stk) * stack_pop (&Cpu->Stk));
})

CMD_DEF(OUT, "out",
{
    printf ("result: %lg\n", stack_pop (&Cpu->Stk));
})

CMD_DEF(DUMP, "dump",
{
    stack_dumps (&Cpu->Stk, Cpu->Info.file_out);
})

CMD_DEF(JBE, "jbe",
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

CMD_DEF(JAE, "jae",
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

CMD_DEF(JA, "ja",
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

CMD_DEF(JB, "jb",
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

CMD_DEF(JE, "je",
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

CMD_DEF(JNE, "jne",
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
