#ifndef PROC_H
#define PROC_H

#include "stack.h"

#define AmntCommands 10

enum ECommandNums
{
    push = 1,
    add  = 2,
    sub  = 3,
    mul  = 4,
    dvd  = 5,
    pop  = 6,
    inp  = 7,
    out  = 8,
    dump = 9,
    hlt  = 0
};

typedef double DataType;

typedef struct
{
    int            top_number;
    unsigned char* ArrCode;
    unsigned char  size;
    unsigned char  sygnature;
    unsigned char  version;
} StructMachineCode;

typedef struct
{
    unsigned char     name;
    int               amnt_args;
    int             (*func)  (StructStack*, DataType*);
    const char*       long_name;
} StructFunctions;


int check_passport (FILE* Bin, StructMachineCode* Code);

int read_array_of_code (FILE* Bin, StructMachineCode* Code);

//=============================================

int push_proc (StructStack*, DataType* args);

int pop_proc (StructStack*, DataType* args);

int add_proc (StructStack*, DataType* args);

int sub_proc (StructStack*, DataType* args);

int mul_proc (StructStack*, DataType* args);

int div_proc (StructStack*, DataType* args);

int inp_proc (StructStack*, DataType* args);

int out_proc (StructStack*, DataType* args);

int dump_proc (StructStack*, DataType* args);

int hlt_proc (StructStack*, DataType* args);

//=============================================


//=============================================


#endif
