#include "../include/dasm.h"

//-----------------------------------------------------------------------------

int main ()
{
    Disassember dasm = { 0 };

    if(disassember_ctor (&dasm) == ERROR_CTOR)
    {
        return ERROR_dasm;
    }

    read_code_file (&dasm);

    disassembling (&dasm);

    disassember_dtor (&dasm);

    return 0;
}

//-----------------------------------------------------------------------------

