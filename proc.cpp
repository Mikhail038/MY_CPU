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

    CPU->stack = (StructStack*) calloc (1, sizeof (StructStack));
    CPU->addres_stack = (StructStack*) calloc (1, sizeof (StructStack));

    STACKCTOR (CPU->stack, 10);
    STACKCTOR (CPU->addres_stack, 10);

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
        int i = execute_command (CPU);

        if (i == -1)
        {
            printf ("crush\n");
        }
        else if (i == 1)
        {
            printf ("really halted!\n");
            return 0;
        }

    }

    return 0;
}

#define DEF_CMD(name,num,...) \
case num: \
    __VA_ARGS__ \
    return 0;

int execute_command (StructCPU* CPU)
{
    int marker = 0;

    for (int i = 0;  i < AmntCommands; i++)
    {
        marker = CPU->Array[CPU->ip] & 31;

       // printf ("%0.4d %d %d\n", CPU->ip, ArrCommands[i].num, marker);
        if (ArrCommands[i].num == marker)
        {
            switch (marker)
            {
                #include "commands.h"
                default:
                    printf ("default error!\n");
                    exit (0);
            }
        }
    }
    printf ("Unknown Command! marker = %d\n", marker);

    return -1;
}
#undef DEF_CMD(name,num,...)

void cpu_destructor (StructCPU* CPU)
{
    free (CPU->Array);
    free (CPU->RAM);
    free (CPU->Regs);

    stack_destructor (CPU->stack);
    stack_destructor (CPU->addres_stack);

    free (CPU->stack);
    free (CPU->addres_stack);


    return;
}

//=================================================================

#define PUSH_CMD(num,...) \
case num: \
    __VA_ARGS__ \
    break;

void run_push (StructCPU* CPU)
{
    int marker = 0;
    marker = (CPU->Array[CPU->ip] >> 5);
    CPU->ip++;
    //printf ("%d %d\n",  (int) CPU->Array[CPU->ip], marker);
    //printf ("11\n");
    switch (marker)
    {
        #include "Push.h"
        default:
            printf ("push error\n");
            exit (0);
    }
}
#undef PUSH_CMD(num,...)

#define POP_CMD(num,...) \
case num: \
    __VA_ARGS__ \
    break;

void run_pop (StructCPU* CPU)
{
    int marker = 0;
    marker = ( CPU->Array[CPU->ip] >> 5 );
    CPU->ip++;

    //printf ("%d %d\n",  (int) CPU->Array[CPU->ip], marker);
    //printf ("22\n");

    switch (marker)
    {
        #include "Pop.h"
        default:
            printf ("pop error\n");
            exit (0);
    }
}
#undef POP_CMD(num,...)


void run_jump (StructCPU* CPU)
{

}

//=================================================================
