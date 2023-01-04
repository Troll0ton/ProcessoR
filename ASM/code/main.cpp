#include "../include/ASM.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    Assembler Asm = { 0 };

    if(assembler_ctor (&Asm, argv) == E(CTOR))
    {
        printf ("----ERROR ASM----");
    }

    assembling (&Asm);

    asm_dump (&Asm);

    assembler_dtor (&Asm);

    return 0;
}

//-----------------------------------------------------------------------------

