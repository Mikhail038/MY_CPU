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

    Code.top_number = 0;

    make_array_of_code (Amnt_lines, &Source, &Code);
/*
    //print array of code============================================
    for (int i = 0; i < MaxSize; i++)
    {
        printf ("%0.2x ", Code.ArrCode[i]);

    //    if (i % 4 == 3)
    //    {
    //         printf ("\n");
    //    }
       //if
        // if (ArrCode[i] != 0)
        // {
        //     printf ("%d\n", ArrCode[i]);

        //     for (int j = 0; j < sizeof (*ArrCode); j++)
        //     {
        //         printf ("%0.2x ", ((unsigned char*)&(ArrCode[i]))[j]);
        //     }

        //     printf ("\n");
        // }
    }
    //===============================================================
*/
    free (Source.Buffer);


    FILE* Bin = argc > 2 ? fopen (argv[2], "wb") : fopen ("proc.out", "wb");

    make_bin_file (Bin, &Code);
    //fwrite (ArrCode, sizeof (int), MaxSize, Bin);

    fclose (Bin);

    free (Code.ArrCode);

    return 0;
}
