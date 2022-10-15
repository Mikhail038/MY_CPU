#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "proc.h"

//extern StructCommands ArrCommands[AmntCommands];

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

static const int REGS_CONST = 5;

static int start = 100;

static int width = 0;
static int height = 0;
static int vram_size = 0;


void cpu_constructor (FILE* Bin, StructCPU* CPU)
{
    check_passport (Bin, CPU);

    read_array_of_code (Bin, CPU);

    CPU->Regs = (double*) calloc (REGS_CONST, sizeof (*CPU->RAM));
    //printf ("ram size %d %d\n", start + vram_size + 20, vram_size);
    CPU->RAM  = (double*) calloc (start + vram_size + 10, sizeof (*CPU->RAM));

    CPU->stack = (StructStack*) calloc (1, sizeof (StructStack));
    CPU->addres_stack = (StructStack*) calloc (1, sizeof (StructStack));

    STACKCTOR (CPU->stack, 20);
    STACKCTOR (CPU->addres_stack, 20);

    return;
}


int check_passport (FILE* Bin, StructCPU* CPU)
{
    MCA (Bin  != NULL, -1);
    MCA (CPU != NULL, -2);

    unsigned char sygnature = 0;
    unsigned char version = 0;

    fread (&sygnature, 1, sizeof (sygnature), Bin);
    fread (&version,   1, sizeof (version),   Bin);
    fread (&CPU->size, 1, sizeof (CPU->size), Bin);
    fread (&vram_size, 1, sizeof (vram_size), Bin);

    MCA (CPU->size > 0, 1);
    MCA (vram_size > 0, 2);


    if (sygnature != 218)
    {
        printf ("Wrong sygnature!\n");
        exit (0);
    }
    if (version != 1)
    {
        printf ("Wrong version!\n");
        exit (0);
    }
    // printf ("%0.2X %0.2X   %0.2X %0.2X %0.2X %0.2X (%d)\n", Code->sygnature, Code->version,
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

    //printf ("%0.2X %d", Code->ArrCode[Code->ip], Code->ip);

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
            return 1;
        }
        else if (i == 1)
        {
            printf ("really halted! ip = %d \n", CPU->ip);
            return 0;
        }

    }

    printf ("overflow\n");
    return 2;
}

#define DEF_CMD(name, e_num, num, f_proc, f_parse, f_reparse) \
case num: \
    f_proc \
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
                #include "../ASM/commands.h"
                default:
                    printf ("default error!\n");
                    exit (0);
            }
        }
    }
    printf ("Unknown Command! marker = %d ip %d\n", marker, CPU->ip);

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

    switch (marker)
    {
        #include "Push.h"
        default:
            printf ("push error\n");
            exit (0);
    }
}
#undef PUSH_CMD

#define POP_CMD(num,...) \
case num: \
    __VA_ARGS__ \
    break;

void run_pop (StructCPU* CPU)
{
    int marker = 0;
    marker = ( CPU->Array[CPU->ip] >> 5 );
    CPU->ip++;

    switch (marker)
    {
        #include "Pop.h"
        default:
            printf ("pop error\n");
            exit (0);
    }
}
#undef POP_CMD
/*
#define JMP_CMD(num,...) \
case num: \
    __VA_ARGS__ \
    break;

void run_jump (StructCPU* CPU)
{
    int marker = 0;
    marker = ( CPU->Array[CPU->ip] >> 5 );
    CPU->ip++;

    switch (marker)
    {
        #include "Jump.h"
        default:
            printf ("jump error\n");
            exit (0);
    }
}
#undef JMP_CMD*/
//=================================================================
