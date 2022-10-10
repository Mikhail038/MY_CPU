#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "proc.h"


int main (int argc, char** argv)
{
    FILE* Bin = argc > 1 ? fopen (argv[1], "r") : fopen ("proc.out", "r");
    MCA (Bin != NULL, 1);

    StructMachineCode Code = {};

    StructStack stack = {};

    if (STACKCTOR (&stack, 10) != 0)
    {
        stack_destructor (&stack);
        return 1;
    }
    INIT (stack.birth);


    check_passport (Bin, &Code);

    read_array_of_code (Bin, &Code);

    fclose (Bin);


    run_code (&stack, &Code);

    free (Code.ArrCode);

    stack_destructor (&stack);

    return 0;
}
