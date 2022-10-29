#include "../include/ASM.h"
#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    if(assembling (argv) == Error_asm)
    {
        printf ("-------ERROR ASM-------");
        return Error_asm;
    }

    processor ();

    return 0;
}

//-----------------------------------------------------------------------------

