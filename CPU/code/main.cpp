#include "../include/CPU.h"

//-----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
    Processor Cpu = { 0 };

    if(processor_ctor (&Cpu) == E(CTOR))
    {
        return E(CPU);
    }

    read_code_file (&Cpu);

    cpu_dump (&Cpu);

    handle_cmds (&Cpu);

    processor_dtor (&Cpu);

    return 0;
}

//-----------------------------------------------------------------------------

