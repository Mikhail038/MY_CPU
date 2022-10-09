#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "proc.h"


int main (int argc, char** argv)
{
    FILE* Bin = argc > 1 ? fopen (argv[1], "r") : fopen ("proc.out", "r");
    MCA (Bin != NULL, 1);

    StructMachineCode Code = {};

    check_passport (Bin, &Code);

    read_array_of_code (Bin, &Code);

    fclose (Bin);

    free (Code.ArrCode);

    return 0;
}
