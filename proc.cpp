#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "proc.h"

/*
typedef struct
{
    char* name = NULL;
    char* file = NULL;
    char* func = NULL;
    int   line;
} StructInfo;

typedef struct
{
    CanaryType          front_canary    = 0;
    unsigned int    hash            = 0;
    unsigned int    hash_data       = 0;
    int             size            = 0;
    int             capacity        = 0;
    StackDataType*  data            = NULL;
    CanaryType          canary          = 0;
    // int             canary_size     = 0;
    int             err             = 0;
    StructInfo*     birth           = NULL;
    StructInfo*     source          = NULL;
    CanaryType          end_canary      = 0;
} StructStack;

StructFunctions ArrFunctions[AmntCommands] =
{
    {push, 1, push_proc, "push"},
    {pop,  0, pop_proc,  "pop"},
    {add,  0, add_proc,  "add"},
    {sub,  0, sub_proc,  "sub"},
    {mul,  0, mul_proc,  "mul"},
    {dvd,  0, div_proc,  "div"},
    {inp,  0, inp_proc,  "inp"},
    {out,  0, out_proc,  "out"},
    {dump, 0, dump_proc, "dump"},
    {hlt,  0, hlt_proc,  "hlt"}
};
*/
typedef struct
{
    ECommandNums  num;
    char          name[10];
    int           args;
} StructCommands;

static const StructCommands ArrCommands[AmntCommands] =
{
    {push, "push", 1},
    {pop,  "pop",  0},
    {add,  "add",  0},
    {sub,  "sub",  0},
    {mul,  "mul",  0},
    {dvd,  "div",  0},
    {inp,  "inp",  0},
    {out,  "out",  0},
    {dump, "dump", 0},
    {dup, "dup",   0},
    {jump, "jump", 1},
    {hlt,  "hlt",  0}
};

enum EErrors
{
    UnknownError            = 1,
    Capacity                = 2,
    StackNullPtr            = 4,
    SizeMoreCapacity        = 8,
    StackFrontCanaryIsDead  = 16,
    StackEndCanaryIsDead    = 32,
    BirthFileNull           = 64,
    BirthFuncNull           = 128,
    SourceFileNull          = 256,
    SourceFuncNull          = 512,
    DataFrontCanaryIsDead   = 1024,
    DataEndCanaryIsDead     = 2048,
    WrongStackHash          = 4096,
    WrongDataHash           = 8192
};

typedef struct
{
    EErrors  num;
    char name[60]; //TODO read about svariable array
} StructError;

static const StructError ArrStructErr[32] =
{
    {UnknownError,           "Unknown Error"},
    {Capacity,               "Capacity < 0"},
    {StackNullPtr,           "Stack pointer is NULL"},
    {SizeMoreCapacity,       "Stack size is more than capacity"},
    {StackFrontCanaryIsDead, "Front canary is dead. Stack is corrupted"},
    {StackEndCanaryIsDead,   "End canary is dead. Stack is corrupted"},
    {BirthFileNull,          "No info about file where stack was declared"},
    {BirthFuncNull,          "No info about function where stack was declared"},
    {SourceFileNull,         "No info about file where check failed"},
    {SourceFuncNull,         "No info about function where check failed"},
    {DataFrontCanaryIsDead,  "Front canary is dead. Stack data is corrupted"},
    {DataEndCanaryIsDead,    "End canary is dead. Stack data is corrupted"},
    {WrongStackHash,         "Hash-check failed, stack is corrupted"},
    {WrongDataHash,          "Hash-check failed, data is corrupted"}
};


int check_passport (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    // fscanf (Bin, "%c", Code->sygnature);
    // fscanf (Bin, "%c", Code->version);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[0]);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[1]);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[2]);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[3]);


    fread (&Code->sygnature, 1, sizeof (Code->sygnature), Bin);
    fread (&Code->version,   1, sizeof (Code->version),   Bin);
    fread (&Code->size,      1, sizeof (Code->size),      Bin);

    // printf ("%0.2x %0.2x   %0.2x %0.2x %0.2x %0.2x (%d)\n", Code->sygnature, Code->version,
    //  ((unsigned char*) &Code->size)[0],  ((unsigned char*) &Code->size)[1],
    //  ((unsigned char*) &Code->size)[2],  ((unsigned char*) &Code->size)[3], Code->size);

    return 0;
}

int read_array_of_code (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    Code->ArrCode = (unsigned char*) calloc (Code->size * 2, sizeof (*Code->ArrCode));
    fread (Code->ArrCode, Code->size, sizeof (*Code->ArrCode), Bin);

    //printf ("%0.2x %d", Code->ArrCode[Code->ip], Code->ip);

    return 0;
}

void run_code ( StructStack* stack, StructMachineCode* Code)
{
    for (; Code->ip++ ; Code->ip < Code->size)
    {

    }

    return;
}

//=================================================================

//=================================================================
