#include "../include/DASM.h"

//-----------------------------------------------------------------------------

int main ()
{
    Disassember Dasm = { 0 };

    if(disassember_ctor (&Dasm) == E(CTOR))
    {
        return E(DASM);
    }

    read_code_file (&Dasm);

    disassembling (&Dasm);

    disassember_dtor (&Dasm);

    return 0;
}

//-----------------------------------------------------------------------------

