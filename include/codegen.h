case CMD_PUSH_:
    stack_push (stk_, arg_d);
    ip++;
    break;
CMD_(101,
{
    printf ("COLL!3");
})
case CMD_RG_PUSH_:
    stack_push (stk_, arg_d);
    ip++;
    break;
case CMD_RM_PUSH_:
    stack_push (stk_, arg_d);
    ip++;
    break;
case CMD_ADD_:
    stack_push (stk_, stack_pop (stk_) + stack_pop (stk_));
    break;
case CMD_SUB_:
    stack_push (stk_, -(stack_pop (stk_) - stack_pop (stk_)));
    break;
case CMD_MUL_:
    stack_push (stk_, stack_pop (stk_) * stack_pop (stk_));
    break;
case CMD_DIV_:
    stack_push (stk_, 1 / stack_pop (stk_) * stack_pop (stk_));
    break;
case CMD_HLT_:
    break;
case CMD_OUT_:
    printf ("result: %lg\n", stack_pop (stk_));
    break;
case CMD_DUMP_:
    printf ("|dump|\n");
    break;
case CMD_JB_:
    f2 = stack_pop (stk_);
    f1 = stack_pop (stk_);
    if(f1 < f2)
    {
        int pos_ch = arg_d;
        ip = labels_[pos_ch];
    }
    else ip++;
    stack_push (stk_, f1);
    stack_push (stk_, f2);
    break;
case CMD_JBE_:
    f2 = stack_pop (stk_);
    f1 = stack_pop (stk_);
    if(f1 <= f2)
    {
        int pos_ch = arg_d;
        ip = labels_[pos_ch];
    }
    else ip++;
    stack_push (stk_, f1);
    stack_push (stk_, f2);
    break;
case CMD_JA_:
    f2 = stack_pop (stk_);
    f1 = stack_pop (stk_);
    if(f1 > f2)
    {
        int pos_ch = arg_d;
        ip = labels_[pos_ch];
    }
    else ip++;
    stack_push (stk_, f1);
    stack_push (stk_, f2);
    break;
case CMD_JAE_:
    f2 = stack_pop (stk_);
    f1 = stack_pop (stk_);
    if(f1 >= f2)
    {
        int pos_ch = arg_d;
        ip = labels_[pos_ch];
    }
    else ip++;
    stack_push (stk_, f1);
    stack_push (stk_, f2);
    break;
case CMD_JE_:
    f2 = stack_pop (stk_);
    f1 = stack_pop (stk_);
    if(is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = labels_[pos_ch];
    }
    else ip++;
    stack_push (stk_, f1);
    stack_push (stk_, f2);
    break;
case CMD_JNE_:
    f2 = stack_pop (stk_);
    f1 = stack_pop (stk_);
    if(!is_equal(f1,f2))
    {
        int pos_ch = arg_d;
        ip = labels_[pos_ch];
    }
    else ip++;
    stack_push (stk_, f1);
    stack_push (stk_, f2);
    break;
