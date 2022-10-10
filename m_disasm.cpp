#include <stdio.h>
#include <stdlib.h>
#include "asm.h"
#include "disasm.h"
#include "stack.h"



int main (int argc, char** argv)
{
    FILE* Bin = argc > 1 ? fopen (argv[1], "r") : fopen ("proc.out", "r");
    MCA (Bin != NULL, 1);

    StructMachineCode Code = {};

    check_passport (Bin, &Code);

    read_array_of_code (Bin, &Code);

    fclose (Bin);

    StructDisasm Array = {};

    Array.Buffer = (unsigned char*) calloc (Code.size * 4, sizeof (*Array.Buffer));

    make_text_from_code (&Array, &Code);

    FILE* Text = argc > 2 ? fopen (argv[2], "wb") : fopen ("disassmebled.in", "wb");

    print_text_in_file (Text, &Array);

    fclose (Text);

    free (Code.ArrCode);

    free (Array.Buffer);

    return 0;
}
