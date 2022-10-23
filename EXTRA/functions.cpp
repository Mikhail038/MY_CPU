
#include <stdio.h>

#include "functions.h"
#include "config.h"
#include "MYassert.h"


int check_passport (FILE* Bin, int* Size, int* VramSize)
{
    MCA (Bin  != NULL, StdError);
    MCA (Size != NULL, StdError);
    MCA (VramSize != NULL, StdError);

    unsigned char signature = 0;
    unsigned char version = 0;

    fread (&signature, 1, sizeof (signature),  Bin);
    fread (&version,   1, sizeof (version),    Bin);
    fread (Size,       1, sizeof (*Size),      Bin);
    fread (VramSize,   1, sizeof (*VramSize),  Bin);

    MCA (*Size > 0, StdError);
    MCA (*VramSize >= 0, StdError);

    if (signature != StdSign)
    {
        printf ("Wrong sygnature!\n");
        return StdError;
    }
    if (version != StdVersion)
    {
        printf ("Wrong version!\n");
        return StdError;
    }

    return 0;
}
