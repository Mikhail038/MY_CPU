#include <stdio.h>
#include <stdlib.h>

#include "proc.h"
#include <string.h>
//TODO rename to proc_main

int main (int argc, char** argv)
{
    if ((argc > 1) && (strcmp (argv[1], "-g") == 0))
    {
        DBG = 1;
    }

    FILE* Bin = argc > 2 ? fopen (argv[2], "r") : fopen ("code.mc", "r");
    MCA (Bin != NULL, 1);// TODO assert: file.cpp:line

    StructCPU CPU = {};

    cpu_constructor (Bin, &CPU);

    fclose (Bin);


    execute_code (&CPU);

    cpu_destructor (&CPU);

    return 0;
}
