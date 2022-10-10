#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "asm.h"


int main (int argc, char** argv)
{
    FILE* SourceFile = argc > 1 ? fopen (argv[1], "r") : fopen ("proc.in", "r");
    MCA (SourceFile != NULL, 1);

    StructSource Source = {};

    int Amnt_lines = count_lines_in_file (SourceFile);
    Source.amnt_symbols = 1 + count_symbols_in_file (SourceFile);
    int MaxSize = Amnt_lines + Amnt_lines * 2 * sizeof (double);

    MCA (Amnt_lines >= 0, 1);
    Source.Buffer = (char*) calloc (Source.amnt_symbols, sizeof (*Source.Buffer));
    MCA (Source.Buffer != NULL, 0);

    fread (Source.Buffer, sizeof (*Source.Buffer), Source.amnt_symbols, SourceFile);
    Source.Buffer[Source.amnt_symbols - 1] = '\0';

    fclose (SourceFile);

    //printf ("+%s+\n", Source.Buffer);
    printf ("AmntSymbols %d AmntLines %d\n", Source.amnt_symbols, Amnt_lines);

    StructMachineCode Code = {};

    Code.ArrCode = (unsigned char*) calloc (1, MaxSize);

    Code.ip = 0;

    make_array_of_code (Amnt_lines, &Source, &Code);

    free (Source.Buffer);


    FILE* Bin = argc > 2 ? fopen (argv[2], "wb") : fopen ("proc.out", "wb");

    make_bin_file (Bin, &Code);
    //fwrite (ArrCode, sizeof (int), MaxSize, Bin);

    fclose (Bin);

    free (Code.ArrCode);

    return 0;
}
