#ifndef PROC_H
#define PROC_H

#include "stack.h"
#include "asm.h"

#define AmntCommands 12



int check_passport (FILE* Bin, StructMachineCode* Code);

int read_array_of_code (FILE* Bin, StructMachineCode* Code);

void run_code ( StructStack* stack, StructMachineCode* Code);

//=============================================
//=============================================


//=============================================

int make_text_from_code (StructStack* stack, StructMachineCode* Code);

int read_command (StructStack* stack, StructMachineCode* Code);

#endif
