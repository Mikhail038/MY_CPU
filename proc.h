#ifndef PROC_H
#define PROC_H

#include "stack.h"
#include "asm.h"

#define AmntCommands 12

typedef struct
{
    int            ip;
    int            size;
    StructStack*   stack;
    StructStack*   addres_stack;
    unsigned char* Array;
    double*        RAM;
    double*        Regs;
} StructCPU;



void cpu_constructor (FILE* Bin, StructCPU* CPU);

int check_passport (FILE* Bin, StructCPU* CPU);

void init_CPU (StructCPU* CPU);

int read_array_of_code (FILE* Bin, StructCPU* CPU);

int execute_code (StructCPU* CPU);

//=============================================
//=============================================


//=============================================

int execute_command (StructCPU* CPU);

void cpu_destructor (StructCPU* CPU);

void run_push_or_pop (StructCPU* CPU, const char* line);

void run_jump (StructCPU* CPU, const char* line);

#endif

