#include "../include/ASM.h"
#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    if(assembling (argv) == ERR_ASM)
    {
        printf ("-------ERROR ASM-------\n");

        return ERR_ASM;
    }

    if(processor () == ERR_CPU)
    {
        printf ("-------ERROR CPU-------\n");

        return ERR_CPU;
    }

    return 0;
}

//-----------------------------------------------------------------------------

