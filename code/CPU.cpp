#include "../include/CPU.h"
#include "../include/stack.h"
#include "../include/lines_handle.h"
#include "../include/input_output.h"

//-----------------------------------------------------------------------------

void processor ()
{
    Stack stk1;
    stack_ctor (&stk1, 2);

    FILE *code_file = fopen ("../files/code.txt", "rb");

    int flag_stop = 0;
    double num = 0;
    int cmd = -1;

    while(!flag_stop)
    {
        fread (&cmd, sizeof(int), 1, code_file);

        switch(cmd)
        {
            case CMD_PUSH:
                fread (&num, sizeof(double), 1, code_file);
                stack_push (&stk1, num);
                break;
            case CMD_ADD:
                stack_push (&stk1, stack_pop (&stk1) + stack_pop (&stk1));
                break;
            case CMD_SUB:
                stack_push (&stk1, stack_pop (&stk1) - stack_pop (&stk1));
                break;
            case CMD_MUL:
                stack_push (&stk1, stack_pop (&stk1) * stack_pop (&stk1));
                break;
            case CMD_DIV:
                stack_push (&stk1, stack_pop (&stk1) / stack_pop (&stk1));
                break;
            case CMD_HLT:
                flag_stop++;
                break;
            case CMD_OUT:
                printf ("result: %lg", stack_pop (&stk1));
                break;
            default:
                printf ("????");
                break;
        }

        stack_dump_ (&stk1);
    }

    stack_dtor (&stk1);

    fclose (code_file);
}

//-----------------------------------------------------------------------------

int main()
{
    processor ();

    return 0;
}

//-----------------------------------------------------------------------------
