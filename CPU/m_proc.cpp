#include <stdio.h>
#include <stdlib.h>

#include "proc.h"


int main (int argc, char** argv)
{
    FILE* Bin = argc > 1 ? fopen (argv[1], "r") : fopen ("code.mc", "r");
    MCA (Bin != NULL, 1);

    StructCPU CPU = {};

    cpu_constructor (Bin, &CPU);

    fclose (Bin);


    execute_code (&CPU);

    cpu_destructor (&CPU);

    return 0;
}
