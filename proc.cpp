#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    StackEndCanaryIsDead    = 128,
    BirthFileNull           = 256,
    BirthFuncNull           = 256,
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
    char name[60];
} StructError;

static const StructError ArrStructErr[128] =
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

void cpu_constructor (FILE* Bin, StructCPU* CPU)
{
    check_passport (Bin, CPU);

    read_array_of_code (Bin, CPU);

    CPU->Regs = (double*) calloc (5, sizeof (*CPU->RAM));
    CPU->RAM  = (double*) calloc (10, sizeof (*CPU->RAM));

    return;
}


int check_passport (FILE* Bin, StructCPU* CPU)
{
    MCA (Bin  != NULL, -1);
    MCA (CPU != NULL, -2);

    // fscanf (Bin, "%c", Code->sygnature);
    // fscanf (Bin, "%c", Code->version);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[0]);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[1]);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[2]);
    // fscanf (Bin, "%c", ((unsigned char*) &Code->size)[3]);

    unsigned char sygnature;
    unsigned char version;

    fread (&sygnature, 1, sizeof (sygnature), Bin);
    fread (&version,   1, sizeof (version),   Bin);
    fread (&CPU->size, 1, sizeof (CPU->size),      Bin);

    // printf ("%0.2x %0.2x   %0.2x %0.2x %0.2x %0.2x (%d)\n", Code->sygnature, Code->version,
    //  ((unsigned char*) &Code->size)[0],  ((unsigned char*) &Code->size)[1],
    //  ((unsigned char*) &Code->size)[2],  ((unsigned char*) &Code->size)[3], Code->size);

    return 0;
}

int read_array_of_code (FILE* Bin, StructCPU* CPU)
{
    MCA (Bin  != NULL, -1);
    MCA (CPU != NULL, -2);

    CPU->Array = (unsigned char*) calloc (CPU->size * 2, sizeof (*CPU->Array));
    fread (CPU->Array, CPU->size, sizeof (*CPU->Array), Bin);

    //printf ("%0.2x %d", Code->ArrCode[Code->ip], Code->ip);

    return 0;
}

int execute_code (StructCPU* CPU)
{
    for (; CPU->ip < CPU->size;)
    {
        execute_command (CPU);
        //printf ("done\n");
    }

    return 0;
}

#define DEF_CMD(name,num,...) \
case num: \
    __VA_ARGS__

int execute_command (StructCPU* CPU)
{
    for (int i = 0;  i < AmntCommands; i++)
    {
        //printf ("'%0.2x' '%0.2x' || '%0.2x' || '%0.2x' || '%0.2x'\n", ArrCommands[i].num, Code->ArrCode[Code->ip], Code->ArrCode[Code->ip] - 128, Code->ArrCode[Code->ip] - 256, Code->ArrCode[Code->ip]- 256 - 128 );

        if ((ArrCommands[i].num == CPU->Array[CPU->ip]) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 32) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 64) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 32 - 64) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 128) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 128 - 32) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 128 - 64) ||
        (ArrCommands[i].num == CPU->Array[CPU->ip] - 64 - 128 - 32))
        {
            //printf ("!\n");

            //printf ("%s\n", ArrCommands[i].name);

            if (    strcmp (ArrCommands[i].name, "push") == 0)
            {
                //add_to_array (Array, ArrCommands[i].name);
                run_push_or_pop (CPU, ArrCommands[i].name);
                return 0;
            }
            else if (strcmp (ArrCommands[i].name, "pop" ) == 0)
            {
                //add_to_array (Array, ArrCommands[i].name);
                run_jump (CPU, ArrCommands[i].name);
                return 0;
            }
            else if (strcmp (ArrCommands[i].name, "jump") == 0)
            {
                //add_to_array (Array, ArrCommands[i].name);
                run_jump (CPU, ArrCommands[i].name);
                return 0;
            }
            else
            {
                switch (ArrCommands[i].num)
                {
                #include "simple_commands.h"
                    default:
                        assert (0);
                        break;
                }
                return 0;

            }
        }
    }

    return 0;
}

#undef DEF_CMD(name,num,...)

void cpu_destructor (StructCPU* CPU)
{
    return;
}


//=================================================================
void run_push_or_pop (StructCPU* CPU, const char* line)
{

}

void run_jump (StructCPU* CPU, const char* line)
{

}

//=================================================================
