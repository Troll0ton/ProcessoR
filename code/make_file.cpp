#include "../include/ASM.h"
#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    if(assembler (argv) == ERROR_ASM)
    {
        printf ("-------ERROR ASM-------");
        return ERROR_ASM;
    }

    processor ();

    return 0;
}

//-----------------------------------------------------------------------------

