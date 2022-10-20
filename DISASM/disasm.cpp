#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int ROUND = 7;

static StructLabels ArrLabels[LabelsAmnt] = {};

int read_array_of_code (FILE* Bin, StructMachineCode* Code)
{
    MCA (Bin  != NULL, -4);
    MCA (Code != NULL, -6);

    Code->ArrCode = (unsigned char*) calloc (Code->size * 3, sizeof (*Code->ArrCode));
    fread (Code->ArrCode, Code->size, sizeof (*Code->ArrCode), Bin);

    //printf ("%0.2X %d", Code->ArrCode[Code->ip], Code->ip);

    return 0;
}

int check_passport (FILE* Bin, StructMachineCode* Code) // TODO copypaste
{
    MCA (Bin  != NULL, -1);
    MCA (Code != NULL, -2);

    int vram_size = 0;

    fread (&Code->sygnature, 1, sizeof (Code->sygnature), Bin);
    fread (&Code->version,   1, sizeof (Code->version),   Bin);
    fread (&Code->size,      1, sizeof (Code->size),      Bin);
    fread (&vram_size,       1, sizeof (vram_size),       Bin);


    if (Code->sygnature != 291)
    {
        printf ("Wrong sygnature!\n");
        exit (0); // TODO remove
    }
    if (Code->version != 1)
    {
        printf ("Wrong version!\n");
        exit (0); // TODO remove
    }

    // printf ("%0.2X %0.2X   %0.2X %0.2X %0.2X %0.2X (%d)\n", Code->sygnature, Code->version,
    //  ((unsigned char*) &Code->size)[0],  ((unsigned char*) &Code->size)[1],
    //  ((unsigned char*) &Code->size)[2],  ((unsigned char*) &Code->size)[3], Code->size);

    return 7;
}

int make_text_from_code (StructDisasm* Array, StructMachineCode* Code)
{
    Array->pointer = 0;
    Code->ip = 0;

    for (; Code->ip < Code->size;)
    {
        if (read_command (Array, Code) == 2)
        {
            printf ("crush!\n");
            return 2;
        }
    }

    if (ROUND > 1)
    {
        return 1;
    }
    else
    {
        ROUND++;
        make_text_from_code (Array, Code);
    }

    // for (int i = 0; i < LabelsAmnt; i++)
    // {
    //     printf ("%d  %s\n", ArrLabels[i].num, ArrLabels[i].name);
    // }

    return 1;
}


#define DEF_CMD(name, e_num, num, f_proc, f_parse, f_reparse)\
    case e_num: \
        f_reparse \
        return 0;

int read_command (StructDisasm* Array, StructMachineCode* Code)
{
    for (int i = 0; i < LabelsAmnt; i++)// TODO naming
    {
        if (Code->ip == ArrLabels[i].num)
        {
            int length = strlen (ArrLabels[i].name);

            for (int j = 0; j < length; j++) // TODO naming
            {
                Array->Buffer[Array->pointer] = (ArrLabels[i].name)[j];
                Array->pointer++;
            }

            Array->Buffer[Array->pointer] = ':';
            Array->pointer++;

            Array->Buffer[Array->pointer] = '\0';
            Array->pointer++;
        }
    }


    int marker = Code->ArrCode[Code->ip] & 29;

    for (int i = 0;  i < AmntCommands; i++) // TODO naming
    {
        if (ArrCommands[i].num == marker)
        {
            switch (marker)
            {
                #include "../ASM/commands.h"
                default:
                    printf ("default error!\n");
                    return -1;
            }
        }
    }
    printf ("Unknown Command! marker = %d ip %d\n", marker, Code->ip);

    return 0;
}
#undef DEF_CMD

void add_to_array (StructDisasm* Array, const char* line)
{
    Array->Buffer[Array->pointer] = '\t';
    Array->pointer++;

    int length = strlen (line);

    for (int i = 0; i < length; i++) // TODO naming
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

    if ((Code->ArrCode[Code->ip] & 3) && (Code->ArrCode[Code->ip] & 2))
    {
        Array->Buffer[Array->pointer] = '[';
        Array->pointer++;

        Code->ip++;
        reparse_reg (Array, Code);
       //printf ("or\n");

        Array->Buffer[Array->pointer] = ']';
        Array->pointer++;
    }
    else if (Code->ArrCode[Code->ip] & 2)
    {
        Array->Buffer[Array->pointer] = '[';
        Array->pointer++;

        Code->ip++;
        reparse_int (Array, Code);
        //printf ("oi\n");

        Array->Buffer[Array->pointer] = ']';
        Array->pointer++;
    }
    else if (Code->ArrCode[Code->ip] & 5)
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

    Array->Buffer[Array->pointer] = '\0';
    Array->pointer++;

    return 0;
}



int reparse_jump (StructDisasm* Array, StructMachineCode* Code, const char* line)
{
    Code->ip++;

    int num = reparse_label (Array, Code);

    char* str = (char*) calloc (16, sizeof (*str));
    str = strcpy (str, "label_");
    sprintf (&(str[4]), "%d", num);


    for (int i = 0; i < LabelsAmnt; i++)
    {
        if (ArrLabels[i].num == num)
        {
            break;
        }

        if (ArrLabels[i].num == -1)
        {
            //printf ("%d %d\n", i, ArrLabels[i].num);
            ArrLabels[i].name = (char*) calloc (6, sizeof (*ArrLabels[i].name));
            ArrLabels[i].name = strcpy (ArrLabels[i].name, str);
            ArrLabels[i].num = num;
            //printf ("%-3d %-3d %s\n", i, ArrLabels[i].num , ArrLabels[i].name);
            // fprintf (Code->listing_file, "added label: %s %d  [%0.2X %0.2X %0.2X %0.2X]\n", Name, Code->ip,
            break;
        }
    }

    free (str);

    Array->Buffer[Array->pointer] = '\0';
    Array->pointer++;

    return 0;
}

void print_array (StructDisasm* Array)
{
    for (int i = 0; i < Array->pointer; i++) // TODO naming
    {
        printf ("%c", Array->Buffer[i]);
    }
    printf ("\n");

    return;
}

int reparse_label (StructDisasm* Array, StructMachineCode* Code)
{
    int argument = 0;

    for (int i = 0; i < sizeof (int); i++) // TODO naming
    {
        ((unsigned char*) &argument)[i] = Code->ArrCode[Code->ip];
        Code->ip++;
    }
    char buf[20];

    sprintf (buf, "label_%d", argument);

    int length = strlen (buf);

    sprintf ((char*) &Array->Buffer[Array->pointer], "%s", buf);
    Array->pointer += length;

    return argument;
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

    for (int i = 0; i < sizeof (int); i++) // TODO naming
    {
        ((unsigned char*) &argument)[i] = Code->ArrCode[Code->ip];
        Code->ip++;
    }
    char buf[7];

    sprintf (buf, "%d", argument);

    int length = strlen (buf);

    sprintf ((char*) &Array->Buffer[Array->pointer], "%d", argument);
    Array->pointer += length;
    //printf ("!%s!-------------------------------", Array->Buffer);

    return;
}

void reparse_double (StructDisasm* Array, StructMachineCode* Code)
{
    double argument = 0;

    for (int i = 0; i < sizeof (double); ++i) // TODO naming
    {
        ((unsigned char*) &argument)[i] = Code->ArrCode[Code->ip];
        Code->ip++;
    }
    char buf[2];

    sprintf (buf, "%lg", argument);

    int length = strlen (buf);

    sprintf ((char*) &Array->Buffer[Array->pointer], "%lg", argument);

    Array->pointer += length;
}


void print_text_in_file (FILE* Text, StructDisasm* Array)
{
    change_0_n (Array);

    fprintf (Text, "==========================================================\n");
    fprintf (Text, "DISASSEMBLED FILE\n");
    fprintf (Text, "==========================================================\n");

    fwrite (Array->Buffer, Array->pointer, sizeof (*Array->Buffer), Text);
    fprintf (Text, "==========================================================\n");

    for (int i = 0; i < LabelsAmnt; i++) // TODO naming
    {
        if (ArrLabels[i].num != -2)
        {
            free (ArrLabels[i].name);
        }
    }
}

void change_0_n (StructDisasm* Array) // TODO rename to "change_line_end_to_newline"
{
    for (int i = 0; i < Array->pointer; i++) // TODO naming
    {
        if (Array->Buffer[i] == '\0')
        {
            Array->Buffer[i] = '\n';
        }
    }

    return;
}
