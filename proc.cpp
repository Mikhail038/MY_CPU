#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "proc.h"

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


int check_passport (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    fread (&Code->sygnature, 1, sizeof (Code->sygnature), Bin);
    fread (&Code->version,   1, sizeof (Code->version),   Bin);
    fread (&Code->size,      1, sizeof (Code->size),      Bin);

    return 0;
}

int read_array_of_code (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    Code->ArrCode = (unsigned char*) calloc (Code->size, sizeof (*Code->ArrCode));
    fread(Code->ArrCode, Code->size, sizeof (*Code->ArrCode), Bin);

    return 0;
}

void run_code (StructMachineCode* Code)
{
    StructStack stack = {};

    STACKCTOR (&stack, 10);

    for (; Code->top_number++ ; Code->top_number < Code->size)
    {

    }

    return;
}

//=================================================================

int push_proc (StructStack*, DataType* args)
{

    return 0;
}

int pop_proc (StructStack*, DataType* args)
{

    return 0;
}

int add_proc (StructStack*, DataType* args)
{

    return 0;
}

int sub_proc (StructStack*, DataType* args)
{

    return 0;
}

int mul_proc (StructStack*, DataType* args)
{

    return 0;
}

int div_proc (StructStack*, DataType* args)
{

    return 0;
}

int inp_proc (StructStack*, DataType* args)
{

    return 0;
}

int out_proc (StructStack*, DataType* args)
{

    return 0;
}

int dump_proc (StructStack*, DataType* args)
{

    return 0;
}

int hlt_proc (StructStack*, DataType* args)
{

    return 0;
}

//=================================================================
