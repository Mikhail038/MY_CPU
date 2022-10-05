#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "asm.h"


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

typedef struct
{
    ECommandNums  num;
    char          name[10]; //TODO read about svariable array
    int           args;
} StructCommands;

static const StructCommands ArrCommands[AmntCommands] =
{
    {push, "push", 1},
    {add,  "add",  0},
    {sub,  "sub",  0},
    {mul,  "mul",  0},
    {dvd,  "div",  0},
    {pop,  "pop",  0},
    {inp,  "inp",  0},
    {out,  "out",  0},
    {dump, "dump", 0},
    {hlt,  "hlt",  0}
};

int count_lines_in_file (FILE* stream)
{
    MCA (stream != NULL, -1);

    char sym = '\0';
    int AmntLines = 0;

    while ((sym = getc (stream)) != EOF)
    {
        if (sym != '\n')
        {
            AmntLines++;
            while ((sym = getc (stream)) != '\n');
        }
    }

    return AmntLines;
}

int count_symbols_in_file (FILE* stream)
{
    MCA (stream != NULL,  -1);

    fseek (stream, 0, SEEK_END);

    int AmntSymbols = ftell (stream);

    fseek (stream, 0, SEEK_SET);
    return AmntSymbols;
}

void separate_buffer_on_lines (char** ArrLinePtrs, const char* Buffer, int AmntSymbols)
{
    MCA (ArrLinePtrs != NULL, (void) 0);
    MCA (Buffer != NULL, (void) 0);
    MCA (AmntSymbols >= 0, (void) 0);

    int FirstCharNum = 0, CharNum = 0, LineNumber = 0;


    while (FirstCharNum < AmntSymbols)
    {
        CharNum = 0;

        while (Buffer[FirstCharNum + CharNum] != '\n')
        {
            CharNum++;
        }

        if (CharNum != 0)
        {

            ArrLinePtrs[LineNumber] = (char*) &(Buffer[FirstCharNum]);

            ArrLinePtrs[LineNumber][CharNum] = '\0';

            printf ("!%s!\n", ArrLinePtrs[LineNumber]);

            LineNumber++;
        }

        FirstCharNum += CharNum + 1;
    }
    return;
}

/*
int make_array_of_code (char** ArrLinePtrs, int AmntLines, int* ArrCode, int* ArrNumber)
{    
    //printf ("'%20s'\n", ArrLinePtrs);

    for (int i = 0; i < AmntLines - 1; i++)
    {
        make_command_from_line (ArrLinePtrs[i], ArrCode, ArrNumber);
    }

    return 0;
}

void make_command_from_line (char* Line, int* ArrCode, int* ArrNumber)
{
    MCA (Line != NULL, (void) 0);

    //int OldNumber = *ArrNumber;
    int Length = strlen (Line);
    int c = 0;

    for (int i = 0; i < Length; i++)
    {
        if (Line[i] != ' ')
        {
            c = parse (&(Line[i]), &i, ArrCode, ArrNumber);    
            printf ("'%d'\n", c);        
            MCA (c >= 0, (void) 0);
            ArrNumber++;

            if (c > 0)
            {   
                parse_arguments (c, &(Line[i]), &i, ArrCode, ArrNumber);
            } 

            break;
        }
    }    

    //printf ("--%d\n", *ArrNumber);// - OldNumber));//(*ArrNumber - OldNumber);  

    return;
}

int parse (char* Name, int* Cursor, int* ArrCode, int* ArrNumber)
{
    //printf ("-%s- %d\n", Name, *Cursor); 
    
    int i = 0;
    for (; i < 100 ; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\0')) 
        {
            Name[i] = '\0';
            break;
        }
    }

    for (int j = 0; j < AmntCommands; j++)
    {

        if (strcmp (&(Name[*Cursor]), ArrCommands[j].name) == 0)
        {
            ArrCode[*ArrNumber] = ArrCommands[j].num;
            printf ("%s %d\n", ArrCommands[j].name, ArrCommands[j].num);

            (*ArrNumber) += 1;
            //printf ("Number %d\n", *ArrNumber);
            *Cursor += i;
            printf ("cursor %d\n", *Cursor);

            return ArrCommands[j].args;
        }
    }

    *Cursor += i;
    
    return -1;
}
void parse_arguments (int AmntArgs, char* Name, int* Cursor, int* ArrCode, int* ArrNumber)
{
    //printf ("%d --%c--\n", *Cursor, Name[*Cursor]);

    // for (int i = 0; i < 5; i++)
    // {
    //     printf ("'%c'", Name[i]);
    // }
    // printf ("\n");
    int a = 0;

    for (int i = *Cursor; ; i++)
    {
        if (Name[i] != ' ')
        {
            sscanf (&(Name[i]), "%d", &a);
            printf ("a %d\n", a);
            break;
        }
    }    

    int i = 0;
    for (; ; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n')) 
        {
            Name[i] = '\0';
            break;
        }
    }
    *Cursor += i;


    
    return;
}
*/

void make_array_of_code (char* Buffer, int* ArrCode, int AmntSymbols, int AmntLines)
{
    int pointer = 0;
    int AmntArgs = 0;
    int size = 0;

    for (int LineNum = 0; LineNum < AmntLines; LineNum++)
    {
        seek (Buffer, &pointer, AmntSymbols);
        AmntArgs = parse (Buffer, &pointer, ArrCode, &size);

        for (int NumArgument = 0; NumArgument < AmntArgs; NumArgument++)
        {
            seek (Buffer, &pointer, AmntSymbols);
            parse_argument (Buffer, &pointer, ArrCode, &size);
        }
    }

    return;
}

void seek (char* Buffer, int* pointer, int AmntSymbols)
{
    for (; *pointer < AmntSymbols; (*pointer)++)
    {
        if ( (Buffer[*pointer] != ' ') && (Buffer[*pointer] != '\n') && (Buffer[*pointer] != '\0'))
        {
            return;
        }
    }
}   

int parse (char* Buffer, int* pointer, int* ArrCode, int* size)
{
    char* Name = &(Buffer[*pointer]);
    
    int i = 0;
    for (; ; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n')) 
        {
            Name[i] = '\0';
            break;
        }
    }

    //printf ("(%s)\n", Name);

    for (int j = 0; j < AmntCommands; j++)
    {

        if (strcmp (Name, ArrCommands[j].name) == 0)
        {
            ArrCode[*size] = ArrCommands[j].num;
            //printf ("%s %d ", ArrCommands[j].name, ArrCommands[j].num);

            //printf ("Number %d\n", *ArrNumber);
            *pointer += i;
            //printf ("cursor %d\n", *pointer);
            
            (*size)++;
            return ArrCommands[j].args;
        }
    }

}

void parse_argument (char* Buffer, int* pointer, int* ArrCode, int* size)
{
    char* Name = &(Buffer[*pointer]);
    
    int i = 0;
    for (; ; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n')) 
        {
            Name[i] = '\0';
            break;
        }
    }

    ArrCode[*size] = strtod (Name, NULL);

    *pointer += i;

    //printf ("(%s) pointer %d\n", Name, *pointer);

    (*size)++;

    return;
}