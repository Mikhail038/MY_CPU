#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "asm.h"

#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KBLU "\x1B[34m"
#define KNRM "\x1B[0m"

static int ROUND = 1;

enum ECommandNums
{
    push = 1,
    pop  = 2,
    add  = 3,
    sub  = 4,
    mul  = 5,
    dvd  = 6,
    inp  = 7,
    out  = 8,
    dump = 9,
    dup  = 10,
    jump = 11,
    hlt  = 0
};

enum ERegisterNums
{
    rax  = 1,
    rbx  = 2,
    rcx  = 3,
    rdx  = 4
};

typedef struct
{
    ERegisterNums  num;
    const char*    name;
} StructRegisters;

typedef struct
{
    ECommandNums  num;
    char          name[10];
    int           args;
} StructCommands;

typedef struct
{
    int   num = -1;
    char* name;
} StructLabels;

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

static const StructRegisters ArrRegisters[4] =
{
    {rax, "rax"},
    {rbx, "rbx"},
    {rcx, "rcx"},
    {rdx, "rdx"}
};

static StructLabels ArrLabels[15] = {};

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

int open_listing_file (const char* Filename, StructMachineCode* Code)
{
    Code->listing_file = fopen (Filename, "w");
    MCA (Code->listing_file != NULL, 1);

    fprintf (Code->listing_file,
             "===============================================================\n"
             "LISTING\n"
             "===============================================================\n"
             "Compilation started: %s %s\n"
             "---------------------------------------------------------------\n"
            , __DATE__, __TIME__);

    return 0;
}

int close_listing_file (StructMachineCode* Code)
{
    MCA (Code->listing_file != NULL, 1);

    fprintf (Code->listing_file,
             "---------------------------------------------------------------\n"
             "Compilation ended: %s %s\n"
             "===============================================================\n"
            , __DATE__, __TIME__);

    fclose (Code->listing_file);
    return 0;
}

void make_array_of_code (int Amnt_lines, StructSource* Source, StructMachineCode* Code)
{
    Code->top_number = 0;
    Source->pointer  = 0;
    Source->amnt_lines = Amnt_lines;

    if (ROUND < 2)
    {
        open_listing_file ("proc.log", Code);
    }

    for (int LineNum = 0; LineNum < Source->amnt_lines; LineNum++)
    {
        if (seek (Source) == 0)
        {
            if (parse (Source, Code) == -1)
            {
                printf ("pzdc!\n");
            }
        }
    }

    Code->sygnature = 218;
    Code->version   = 1;
    Code->size      = Code->top_number;

    fprintf (Code->listing_file,"---------------------------------------------------------------\n"
                                "Round %d ended\n"
                                "---------------------------------------------------------------\n"
                                , ROUND);

    if (ROUND < 2)
    {
        ROUND++;
        make_array_of_code (Amnt_lines, Source, Code);
    }

    return;
}

int seek (StructSource* Source)
{
    for (; Source->pointer < Source->amnt_symbols; Source->pointer++)
    {
        if ( (Source->Buffer[Source->pointer] != ' ' ) &&
             (Source->Buffer[Source->pointer] != '\n') &&
             (Source->Buffer[Source->pointer] != '\0'))
        {
            if (Source->Buffer[Source->pointer] == ';')
            {
                skip_line (Source);
                continue;
            }

            //printf ("'%c'\n", Source->Buffer[Source->pointer]);
            return 0;
        }
    }
    return 1;

}

void skip_line (StructSource* Source)
{
    //printf ("s\n");

    Source->amnt_lines--;
    //Source->pointer++;

    for (; Source->pointer < Source->amnt_symbols; Source->pointer++)
    {
        if (Source->Buffer[Source->pointer] == '\n')
        {
            return;
        }
    }
}

int add_label (char* Name, StructMachineCode* Code)
{
    int Length = strlen (Name);

    Name[Length] = '\0';


    for (int i = 0; i < 15; i++)
    {
        if  ((ArrLabels[i].num != -1) && (strncmp (Name, ArrLabels[i].name, Length - 1) == 0))
        {
            return 0;
        }
        if (ArrLabels[i].num == -1)
        {
            //printf ("%d %d\n", i, ArrLabels[i].num);
            ArrLabels[i].name = (char*) calloc (Length + 1, sizeof (char));
            ArrLabels[i].name = strcpy (ArrLabels[i].name, Name);
            (ArrLabels[i].name)[Length - 1] = '\0';
            ArrLabels[i].num  = Code->top_number;

            fprintf (Code->listing_file, "added label: %s %d  %0.2x %0.2x %0.2x %0.2x\n", Name, Code->top_number,
            ((unsigned char*) &Code->top_number)[0],
            ((unsigned char*) &Code->top_number)[1],
            ((unsigned char*) &Code->top_number)[2],
            ((unsigned char*) &Code->top_number)[3]
            );

            return 0;
        }
    }

    return 1;
}

int parse (StructSource* Source, StructMachineCode* Code)
{
    char* Name = &(Source->Buffer[Source->pointer]);

    //printf ("(%s)\n", Name);

    int i = 0;
    for (; i < Source->amnt_symbols; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n') || (Name[i] == '\0'))
        {
            Name[i] = '\0';
            break;
        }
    }

    //printf ("(%s) '%c'\n", Name, Name[i - 1]);

    if (Name[i - 1] == ':')
    {
        if (add_label (Name, Code) != 0)
        {
            printf ("hui\n");
            return -1;
        }
        Source->pointer += i;
        return 0;
    }
    else
    {
        for (int j = 0; j < AmntCommands; j++)
        {
            if (strcmp (Name, ArrCommands[j].name) == 0)
            {
                unsigned char symbol = ArrCommands[j].num;

                Code->ArrCode[Code->top_number] = symbol;

                Source->pointer += i;
                Code->top_number++;

                if ((ArrCommands[j].num == push) || (ArrCommands[j].num == push))
                {
                    if (parse_push_or_pop (Source, Code) != 0)
                    {
                        return -1;
                    }
                }
                else if (ArrCommands[j].num == jump)
                {
                    if (parse_jump (Source, Code) != 0)
                    {
                        return -1;
                    }
                }
                else
                {
                    fprintf (Code->listing_file, "    %0.4d %0.2x %s\n", Code->top_number - 1, Code->ArrCode[Code->top_number - 1], ArrCommands[j].name);
                }

                return ArrCommands[j].args;
            }
        }
    }

}

int parse_push_or_pop (StructSource* Source, StructMachineCode* Code)
{
    seek (Source);

    char* Name = &(Source->Buffer[Source->pointer]);

    int i = 0;
    for (; i < Source->amnt_symbols; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n') || (Name[i] == '\0'))
        {
            Name[i] = '\0';
            break;
        }
    }

    if (Name[0] == 'r')
    {
        parse_str (Source, Code);
    }
    else if ((Name[0] <= '9') && ((Name[0] >= '1')))
    {
        parse_double (Source, Code);
    }
    else
    {
        printf ("hui\n");
        return -1;
    }

    return 0;

}

int parse_jump (StructSource* Source, StructMachineCode* Code)
{
    seek (Source);

    char* Name = &(Source->Buffer[Source->pointer]);

    int i = 0;
    for (; i < Source->amnt_symbols; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n'))
        {
            Name[i] = '\0';
            break;
        }
    }

    if ((Name[0] >= 'A') && (Name[0] <= 'z'))
    {
        jump_label (Source, Code);
        return 0;
    }
    if ((Name[0] >= '1') && (Name[0] <= '9'))
    {
        jump_int (Source, Code);
        return 0;
    }
    else
    {
        printf ("hui\n");
    }

    return -1;
}

void parse_double (StructSource* Source, StructMachineCode* Code)
{
    //fprintf (Code->listing_file, "%0.4d %0.2x %s\n", Code->top_number - 1, Code->ArrCode[Code->top_number - 1], "push");
    print_command (Code, "push");

    char* Name = &(Source->Buffer[Source->pointer]);

    int length = strlen (Name);

    double d_argument = strtod (Name, NULL);

    int j = 0;
    for ( ;j < sizeof (double); j++)
    {
        Code->ArrCode[Code->top_number + j] = ((unsigned char*) &d_argument)[j];
        print_element (Code);
        Code->top_number++;
    }

    Source->pointer += length;

    //printf ("(%s) pointer %d\n", Name, *pointer);

    return;
}

void parse_str (StructSource* Source, StructMachineCode* Code)
{
    Code->ArrCode[Code->top_number - 1] = (unsigned char)((int) Code->ArrCode[Code->top_number - 1] + 32);

    print_command (Code, "push");

    char* Name = &(Source->Buffer[Source->pointer]);

    int length = strlen (Name);

    int j = 0;
    for ( ; j < 4; j++)
    {
        if (strcmp (Name, ArrRegisters[j].name) == 0)
        {
            Code->ArrCode[Code->top_number] = (unsigned char) ArrRegisters[j].num;
            print_element (Code);
            Code->top_number++;
        }

        // if (Name[j] != '\0')
        // {
        //     Code->ArrCode[Code->top_number + j] = (unsigned char) ((Name)[j]);
        //     fprintf (Code->listing_file, "%0.4d %0.2x \n", Code->top_number + j, (unsigned char) ((Name)[j]));
        // }
    }

    Source->pointer += length;

    //printf ("(%s) pointer %d\n", Name, *pointer);

    Code->top_number++;

    return;
}

void parse_op (StructSource* Source, StructMachineCode* Code)
{
    Code->ArrCode[Code->top_number - 1] = (unsigned char)((int) Code->ArrCode[Code->top_number - 1] + 32);
}

void jump_int (StructSource* Source,StructMachineCode* Code)
{
    char* Name = &(Source->Buffer[Source->pointer]);

    int length = strlen (Name);

    int ip = 0;
    //printf ("'%s'\n", Name);
    sscanf (Name, "%d", &ip);

    print_command (Code, "jump");

    for (int j = 0; j < sizeof (int); j++)
    {
        Code->ArrCode[Code->top_number] = ((unsigned char*) &(ip))[j];
        print_element (Code);
        Code->top_number++;
    }

    Source->pointer += length;

    return;
}

void jump_label (StructSource* Source,StructMachineCode* Code)
{
    char* Name = &(Source->Buffer[Source->pointer]);

    //printf ("%d %s\n", ROUND, Name);
    int old_number = Code->top_number;

    int length = strlen (Name);

    print_command (Code, "jump");

    for (int i = 0; i < 15; i++)
    {
        //printf ("%d %d %s\n", i, ArrLabels[i].num, (ArrLabels[i].num != -1) ? ArrLabels[i].name : "a" );
        if ((ArrLabels[i].num != -1) && (strcmp (Name, ArrLabels[i].name) == 0))
        {
            for (int j = 0; j < sizeof (int); j++)
            {
                Code->ArrCode[Code->top_number] = ((unsigned char*) &(ArrLabels[i].num))[j];
                print_element (Code);
                Code->top_number++;
            }
        }
    }

    if (old_number == Code->top_number)
    {
        if (ROUND < 2)
        {
            //printf ("Warning!\n");
            fprintf (Code->listing_file, "skipped %d bytes (label not found)\n", sizeof (int));
            Code->top_number += sizeof (int);
        }
        else
        {
            abort ();
        }
    }

    Source->pointer += length;
}

void print_command (StructMachineCode* Code, const char* Name)
{
    fprintf (Code->listing_file, "    %0.4d %0.2x %s\n", Code->top_number - 1, Code->ArrCode[Code->top_number - 1], Name);

    return;
}

void print_element (StructMachineCode* Code)
{
    fprintf (Code->listing_file, "    %0.4d %0.2x\n", Code->top_number, Code->ArrCode[Code->top_number]);

    return;
}

void make_bin_file (FILE* Bin, StructMachineCode* Code)
{
    free_labels (Code);

    close_listing_file (Code);

    // unsigned char Sgn     = 218;
    // unsigned char Version = 1;
    // unsigned char Amount  = Code->top_number;

    //printf ("%0.2x %0.2x %0.2x\n", Sgn, Version, Amount);
    unsigned char Passport[3] = {Code->sygnature, Code->version, Code->size};

    fwrite (Passport, 3, sizeof (*Passport), Bin);
    fwrite (Code->ArrCode, Code->top_number, sizeof (*Code->ArrCode), Bin);

    return;
}

void free_labels (StructMachineCode* Code)
{
    fprintf (Code->listing_file, "---------------------------------------------------------------\n");
    for (int i = 0; i < 15; i++)
    {
        if (ArrLabels[i].num != -1)
        {
            fprintf (Code->listing_file,"freed label: %d %s %d  %0.2x %0.2x %0.2x %0.2x\n",
            i, ArrLabels[i].name, ArrLabels[i].num,
            ((unsigned char*) &ArrLabels[i].num)[0],
            ((unsigned char*) &ArrLabels[i].num)[1],
            ((unsigned char*) &ArrLabels[i].num)[2],
            ((unsigned char*) &ArrLabels[i].num)[3]
            );

            free (ArrLabels[i].name);
        }
    }

    return;
}
