#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "asm.h"
#include "disasm.h"


//TODO jump labels

//static int ROUND = 1;

// typedef struct
// {
//     ECommandNums  num;
//     char          name[10];
//     int           args;
// } StructCommands;



//extern StructCommands ArrCommands[AmntCommands];

static StructLabels ArrLabels[15] = {};

int read_array_of_code (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    Code->ArrCode = (unsigned char*) calloc (Code->size * 2, sizeof (*Code->ArrCode));
    fread (Code->ArrCode, Code->size, sizeof (*Code->ArrCode), Bin);

    //printf ("%0.2X %d", Code->ArrCode[Code->ip], Code->ip);

    return 0;
}

int check_passport (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    fread (&Code->sygnature, 1, sizeof (Code->sygnature), Bin);
    fread (&Code->version,   1, sizeof (Code->version),   Bin);
    fread (&Code->size,      1, sizeof (Code->size),      Bin);

    // printf ("%0.2X %0.2X   %0.2X %0.2X %0.2X %0.2X (%d)\n", Code->sygnature, Code->version,
    //  ((unsigned char*) &Code->size)[0],  ((unsigned char*) &Code->size)[1],
    //  ((unsigned char*) &Code->size)[2],  ((unsigned char*) &Code->size)[3], Code->size);

    return 0;
}

int make_text_from_code (StructDisasm* Array, StructMachineCode* Code)
{
    for (; Code->ip < Code->size;)
    {
        if (read_command (Array, Code) == 1)
        {
            printf ("crush!\n");
            return 1;
        }
    }

    return 0;
}


#define DEF_CMD(name, e_num, num, f_proc, f_parse, f_reparse)\
    case e_num: \
        f_reparse

int read_command (StructDisasm* Array, StructMachineCode* Code)
{
    // for (int i = 0; i < 15; i++)
    // {
    //     if ((Code->ArrCode[Code->ip] == ArrLabels[i]) && (ROUND == 1))
    //     {
    //
    //     }
    // }

    int marker = Code->ArrCode[Code->ip] & 31;

    for (int i = 0;  i < AmntCommands; i++)
    {
        if (ArrCommands[i].num == marker)
        {
            switch (marker)
            {
                #include "commands.h"
                default:
                    printf ("default error!\n");
                    return 1;
            }
//
//             if ((marker == hlt) || (marker == ret ) || ((marker >= add) && (marker <= dup)))
//             {
//                 add_to_array (Array, ArrCommands[i].name);
//                 Code->ip++;
//                 Array->Buffer[Array->pointer] = '\n';
//                 Array->pointer++;
//
//                 if (marker == hlt)
//                 {
//                     Array->Buffer[Array->pointer] = '\n';
//                     Array->pointer++;
//                 }
//
//                 return 0;
//             }
//             else if ((marker == pop) || (marker == push))
//             {
//                 add_to_array (Array, ArrCommands[i].name);
//                 reparse_push_or_pop (Array, Code, ArrCommands[i].name);
//                 return 0;
//             }
//             else if ((marker == jump) || (marker == call))
//             {
//                 add_to_array (Array, ArrCommands[i].name);
//                 reparse_jump (Array, Code, ArrCommands[i].name);
//                 return 0;
//             }
//             else
//             {
//                 printf ("default error! marker %d\n", marker);
//                 exit(0);

        }
    }
    printf ("Unknown Command! marker = %d ip %d\n", marker, Code->ip);

    return 1;
}
#undef DEF_CMD

void add_to_array (StructDisasm* Array, const char* line)
{
    Array->Buffer[Array->pointer] = '\t';
    Array->pointer++;

    int length = strlen (line);

    for (int i = 0; i < length; i++)
    {
        Array->Buffer[Array->pointer] = line[i];
        Array->pointer++;
    }
    Array->Buffer[Array->pointer] = ' ';
    Array->pointer++;

    //print_array (Array);

    return;
}

int reparse_push_or_pop (StructDisasm* Array, StructMachineCode* Code, const char* line)
{
    //printf ("a\n");

    if ((Code->ArrCode[Code->ip] & 128) && (Code->ArrCode[Code->ip] & 64))
    {
        Array->Buffer[Array->pointer] = '[';
        Array->pointer++;

        Code->ip++;
        reparse_reg (Array, Code);
       //printf ("or\n");

        Array->Buffer[Array->pointer] = ']';
        Array->pointer++;
    }
    else if (Code->ArrCode[Code->ip] & 128)
    {
        Array->Buffer[Array->pointer] = '[';
        Array->pointer++;

        Code->ip++;
        reparse_int (Array, Code);
        //printf ("oi\n");

        Array->Buffer[Array->pointer] = ']';
        Array->pointer++;
    }
    else if (Code->ArrCode[Code->ip] & 64)
    {
        Code->ip++;
        //printf ("r\n");
        reparse_reg (Array, Code);
    }
    else if (strcmp (line, "push") == 0)
    {
        Code->ip++;
        //printf ("d\n");
        reparse_double (Array, Code);
    }
    else if (strcmp (line, "pop") == 0)
    {
        Code->ip++;
    }

    //print_array (Array);
    //printf ("\n");

    Array->Buffer[Array->pointer] = '\n';
    Array->pointer++;

    return 0;
}

int reparse_jump (StructDisasm* Array, StructMachineCode* Code, const char* line)
{
    Code->ip++;

    reparse_int (Array, Code);

    Array->Buffer[Array->pointer] = '\n';
    Array->pointer++;

    return -1;
}

void print_array (StructDisasm* Array)
{
    for (int i = 0; i < Array->pointer; i++)
    {
        printf ("%c", Array->Buffer[i]);
    }
    printf ("\n");

    return;
}

void reparse_reg (StructDisasm* Array, StructMachineCode* Code)
{
    Array->Buffer[Array->pointer] = 'r';
    Array->pointer++;

    Array->Buffer[Array->pointer] = 'a' + Code->ArrCode[Code->ip] - 1;
    Code->ip++;
    Array->pointer++;

    Array->Buffer[Array->pointer] = 'x';
    Array->pointer++;
}

void reparse_int (StructDisasm* Array, StructMachineCode* Code)
{
    int argument = 0;

    for (int i = 0; i < sizeof (int); i++)
    {
        ((unsigned char*) &argument)[i] = Code->ArrCode[Code->ip];
        Code->ip++;
    }
    char buf[10];

    sprintf (buf, "%d\0", argument);

    int length = strlen (buf);

    sprintf ((char*) &Array->Buffer[Array->pointer], "%d", argument);
    Array->pointer += length;
    //printf ("!%s!-------------------------------", Array->Buffer);

    return;
}

void reparse_double (StructDisasm* Array, StructMachineCode* Code)
{
    double argument = 0;

    for (int i = 0; i < sizeof (double); i++)
    {
        ((unsigned char*) &argument)[i] = Code->ArrCode[Code->ip];
        Code->ip++;
    }
    char buf[20];

    sprintf (buf, "%lg\0", argument);

    int length = strlen (buf);

    sprintf ((char*) &Array->Buffer[Array->pointer], "%lg", argument);

    Array->pointer += length;
}


void print_text_in_file (FILE* Text, StructDisasm* Array)
{
    fprintf (Text, "==========================================================\n");
    fprintf (Text, "DISASSEMBLED FILE\n");
    fprintf (Text, "==========================================================\n");

    fwrite (Array->Buffer, Array->pointer, sizeof (*Array->Buffer), Text);
    fprintf (Text, "==========================================================\n");
}
