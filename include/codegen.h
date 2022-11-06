CMD_DEF(HLT, "hlt",
{
    stack_push (&Cpu->Stk, STOP);
})

CMD_DEF(PUSH, "push",
{
    stack_push (&Cpu->Stk, curr_arg);
    curr_pos++;
})

CMD_DEF(POP, "pop",
{
    if((int) Cpu->code[curr_pos] & MASK_RAM && (int) Cpu->code[curr_pos] & MASK_REG)
    {
        Cpu->ram[(int) Cpu->regs[(int) Cpu->code[curr_pos + 1]]] = stack_pop (&Cpu->Stk);
    }

    else if((int) Cpu->code[curr_pos] & MASK_REG)
    {
        Cpu->regs[(int) Cpu->code[curr_pos + 1]] = stack_pop (&Cpu->Stk);
    }

    else if((int) Cpu->code[curr_pos] & MASK_RAM)
    {
        Cpu->ram[(int) Cpu->code[curr_pos + 1]]  = stack_pop (&Cpu->Stk);
    }

    else
    {
        stack_pop (&Cpu->Stk);
        curr_pos--;
    }

    curr_pos++;
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
    double second_number = stack_pop (&Cpu->Stk);
    double first_number  = stack_pop (&Cpu->Stk);

    if(first_number <= second_number)
    {
        int pos_ch = curr_arg;
        curr_pos = pos_ch - 2;
    }

    else curr_pos++;

    stack_push (&Cpu->Stk, first_number);
    stack_push (&Cpu->Stk, second_number);
})

CMD_DEF(JAE, "jae",
{
    double second_number = stack_pop (&Cpu->Stk);
    double first_number  = stack_pop (&Cpu->Stk);
    if(first_number >= second_number)
    {
        int pos_ch = curr_arg;
        curr_pos = pos_ch - 2;
    }
    else curr_pos++;
    stack_push (&Cpu->Stk, first_number);
    stack_push (&Cpu->Stk, second_number);
})

CMD_DEF(JA, "ja",
{
    double second_number = stack_pop (&Cpu->Stk);
    double first_number  = stack_pop (&Cpu->Stk);
    if(first_number > second_number)
    {
        int pos_ch = curr_arg;
        curr_pos = pos_ch- 2;
    }
    else curr_pos++;
    stack_push (&Cpu->Stk, first_number);
    stack_push (&Cpu->Stk, second_number);
})

CMD_DEF(JB, "jb",
{
    double second_number = stack_pop (&Cpu->Stk);
    double first_number  = stack_pop (&Cpu->Stk);
    if(first_number < second_number)
    {
        int pos_ch = curr_arg;
        curr_pos = pos_ch - 2;
    }
    else curr_pos++;
    stack_push (&Cpu->Stk, first_number);
    stack_push (&Cpu->Stk, second_number);
})

CMD_DEF(JE, "je",
{
    double second_number = stack_pop (&Cpu->Stk);
    double first_number  = stack_pop (&Cpu->Stk);
    if(is_equal(first_number,second_number))
    {
        int pos_ch = curr_arg;
        curr_pos = pos_ch - 2;
    }
    else curr_pos++;
    stack_push (&Cpu->Stk, first_number);
    stack_push (&Cpu->Stk, second_number);
})

CMD_DEF(JNE, "jne",
{
    double second_number = stack_pop (&Cpu->Stk);
    double first_number  = stack_pop (&Cpu->Stk);
    if(!is_equal(first_number,second_number))
    {
        int pos_ch = curr_arg;
        curr_pos = pos_ch - 2;
    }
    else curr_pos++;
    stack_push (&Cpu->Stk, first_number);
    stack_push (&Cpu->Stk, second_number);
})
