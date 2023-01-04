
CMD_DEF(HLT, "hlt",
{
    Cpu->F(STOP) = true;
})

CMD_DEF(PUSH, "push",
{
    stack_push (&Cpu->Stk, arg_value);
})

CMD_DEF(POP, "pop",
{
    *curr_arg = stack_pop (&Cpu->Stk);
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
    double value = stack_pop (&Cpu->Stk);

    if(value == POISON_STK)
    {
        printf ("not existed\n");
    }

    else
    {
        printf ("result: %lg\n", value);
    }
})

CMD_DEF(DUMP, "dump",
{
    stack_dumps (&Cpu->Stk, Cpu->Info.file_out);
})

CMD_DEF(JBE, "jbe",
{
    CJMP(first_number <= second_number);
})

CMD_DEF(JAE, "jae",
{
    CJMP(first_number >= second_number);
})

CMD_DEF(JA, "ja",
{
    CJMP(first_number > second_number);
})

CMD_DEF(JB, "jb",
{
    CJMP(first_number < second_number);
})

CMD_DEF(JE, "je",
{
    CJMP(is_equal(first_number, second_number));
})

CMD_DEF(JNE, "jne",
{
    CJMP(!is_equal(first_number, second_number));
})


CMD_DEF(JMP, "jmp",
{
    int pos_ch = arg_value;
    curr_pos = pos_ch - 1;
})

CMD_DEF(CALL, "call",
{
    stack_push (&Cpu->Stk_call, ++curr_pos);

    int pos_ch = arg_value;
    curr_pos   = pos_ch - 1;
})

CMD_DEF(RET, "ret",
{
    curr_pos = stack_pop (&Cpu->Stk_call) - 1;
})

CMD_DEF(SQRT, "sqrt",
{
    double value = stack_pop (&Cpu->Stk);

    if(value >= 0)
    {
        stack_push (&Cpu->Stk, sqrt (value));
    }

    else
    {
        printf ("ERROR - sqrt below zero!\n");
    }
})

