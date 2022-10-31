#include "../include/ASM.h"
#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    if(assembling (argv) == ERR_ASM)
    {
        printf ("-------ERROR ASM-------");

        return ERR_ASM;
    }

    processor ();

    return 0;
}

//-----------------------------------------------------------------------------

