#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "asm.h"


int main (int argc, char** argv)
{
    FILE* Source = argc > 1 ? fopen (argv[1], "r") : fopen ("proc.in", "r");
    MCA (Source != NULL, 1);

    int AmntLines = count_lines_in_file (Source);
    int AmntSymbols = 1 + count_symbols_in_file (Source);
    int MaxSize = 3 * AmntLines;

    MCA (AmntSymbols >= 0, 1);
    char* Buffer = (char*) calloc (AmntSymbols, sizeof (*Buffer));
    MCA (Buffer != NULL, 0);

    fread (Buffer, sizeof (*Buffer), AmntSymbols, Source);
    Buffer[AmntSymbols - 1] = '\0';

    fclose (Source);

    printf ("+%s+\n", Buffer);
    printf ("AmntSymbols %d AmntLines %d\n", AmntSymbols, AmntLines);


    int* ArrCode = (int*) calloc (MaxSize, sizeof (*ArrCode));

    int ArrNumber = 0;

    make_array_of_code ((char*) Buffer, ArrCode, AmntSymbols, AmntLines);

    //print array of code============================================
    for (int i = 0; i < MaxSize; i++)
    {
        if (ArrCode[i] != 0) 
        {
            printf ("%d\n", ArrCode[i]);

            for (int j = 0; j < sizeof (*ArrCode); j++)
            {
                printf ("%0.2x ", ((unsigned char*)&(ArrCode[i]))[j]);
            }

            printf ("\n");
        }
    }
    //===============================================================

    free (Buffer);

    // FILE* Bin = argc > 2 ? fopen (argv[2], "wb") : fopen ("proc.out", "wb");

    //fwrite (ArrCode, sizeof (int), MaxSize, Bin);

    // fclose (Bin);

    free (ArrCode);

    return 0;
}