#include "stack.h"

//-----------------------------------------------------------------------------

void calculator (double *code)  //code 1 10 20 3 4 2
{
    int flag_stop = 0;

    while(!flag_stop)
    {
        switch(code[ip])
        {
            case CMD_PUSH:
                stack_push (stk, code[ip+1]);
                ip+=2;
                break;
            case CMD_ADD:
                stack_push (stk, stack_pop (stk) + stack_pop (stk));
                ip+=1;
                break;
            case CMD_SUB:
                stack_push (stk, stack_pop (stk) - stack_pop (stk));
                ip+=1;
                break;
            case CMD_MUL:
                stack_push (stk, stack_pop (stk) * stack_pop (stk));
                ip+=1;
                break;
            case CMD_DIV:
                stack_push (stk, stack_pop (stk) / stack_pop (stk));
                ip+=1;
                break;
            case CMD_HLT:
                stack_dtor (stk);
                flag_stop++;
                break;
            case CMD_OUT:
                printf ("result: lg", stack_pop (stk));
                ip+=1;
                break;
            case CMD_DUMP:
                stack_dump_ext (stk);
                ip+=1;
                break;
            default:
                printf ("????", __LINE__, num_of_roots);
                ip+=1;
                break;
        }
    }
}

//-----------------------------------------------------------------------------
