#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"

static int ROUND = 1;

//DO NOT BEGIN LABELS AFTER EMPTY POP WITH LETTER R
//DO NOT NAME LABEL "LABEL"


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

//extern StructCommands ArrCommands[AmntCommands];

static const StructRegisters ArrRegisters[4] =
{
    {rax, "rax"},
    {rbx, "rbx"},
    {rcx, "rcx"},
    {rdx, "rdx"}
};

static StructLabels ArrLabels[LabelsAmnt] = {};

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

    int size = Code->vram_size_x * Code->vram_size_y;

    fprintf (Code->listing_file,
             "---------------------------------------------------------------\n"
             "sygnature 0x%0.2X | version 0x%0.2X (%d) |"
             " size [%0.2X %0.2X %0.2X %0.2X] (%d) | VRAM-size [%0.2X %0.2X %0.2X %0.2X] (%d)\n"
             "---------------------------------------------------------------\n"
             "Compilation ended: %s %s\n"
             "===============================================================\n"
            ,Code->sygnature, Code->version, Code->version,
            ((unsigned char*) &Code->size)[0], ((unsigned char*) &Code->size)[1],
            ((unsigned char*) &Code->size)[2], ((unsigned char*) &Code->size)[3], Code->size,
            ((unsigned char*) &size)[0], ((unsigned char*) &size)[1],
            ((unsigned char*) &size)[2], ((unsigned char*) &size)[3], size,
             __DATE__, __TIME__);

    fclose (Code->listing_file);
    return 0;
}



void make_array_of_code (int Amnt_lines, StructSource* Source, StructMachineCode* Code, const char* Filename)
{
    Code->ip = 0;
    Source->pointer  = 0;
    Source->amnt_lines = Amnt_lines;

    if (ROUND < 2)
    {
        open_listing_file (Filename, Code);
    }

    int LineNum = 0;
    for (; LineNum < Source->amnt_lines * 8; LineNum++)
    {
        if (seek (Source) == 0)
        {
            if (parse (Source, Code) == -1)
            {
                printf ("crush!\n");
                return;
            }
        }
    }
    //printf ("%d %d\n", LineNum, Source->amnt_lines);

    Code->sygnature = 218;
    Code->version   = 1;
    Code->size      = Code->ip;

    fprintf (Code->listing_file,"---------------------------------------------------------------\n"
                                "Round %d ended\n"
                                "---------------------------------------------------------------\n"
                                , ROUND);

    if (ROUND < 2)
    {
        ROUND++;
        make_array_of_code (Amnt_lines, Source, Code, Filename);
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


    for (int i = 0; i < LabelsAmnt; i++)
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

            (ArrLabels[i].name)[Length - 1] = '\0'; //TODO %ms
            ArrLabels[i].num  = Code->ip;

            fprintf (Code->listing_file, "added label: %s %d  [%0.2X %0.2X %0.2X %0.2X]\n", Name, Code->ip,
            ((unsigned char*) &Code->ip)[0],
            ((unsigned char*) &Code->ip)[1],
            ((unsigned char*) &Code->ip)[2],
            ((unsigned char*) &Code->ip)[3]
            );

            return 0;
        }
    }

    return 1;
}



#define DEF_CMD(name, e_num, num, f_proc, f_parse, f_reparse)\
    case e_num: \
        f_parse

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
            printf ("error labels same '%s'\n", Name);
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
                int marker = ArrCommands[j].num;

                Code->ArrCode[Code->ip] = ArrCommands[j].num;
                //printf ("#%0.2X\n", Code->ArrCode[Code->ip]);

                Source->pointer += i;
                Code->ip++;

                //printf ("!! %d\n", marker);

                switch (marker)
                {
                    #include "commands.h"
                    default:
                        printf ("default error! %s %d \n", Name, marker);
                        return -1;
                }

                return 0;
            }
        }
        printf ("Unknown command! --%s-- ip %d\n", Name, Code->ip);
        exit (0);
    }
}

#undef DEF_CMD


int parse_push_or_pop (StructSource* Source, StructMachineCode* Code, const char* Command)
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

    if (Name[0] == '[')
    {
        parse_op (Source, Code, Command);
    }
    else if ((Name[0] == 'r') && (Name[2] == 'x'))
    {
        parse_str (Source, Code, Command);
    }
    else if ((Command = "push") && (Name[0] <= '9') && ((Name[0] >= '0')) || (Name[0] == '-') )
    {
        parse_double (Source, Code, Command);
    }
    else
    {
        if (Command = "pop")
        {
            //Source->pointer++;
            //printf ("!'%s'\n", Name);
            print_command (Code, Command);

            return 0;
        }
        else
        {
            printf ("hui\n");
            abort ();

            return -1;
        }

    }

    return 0;

}

int parse_jump (StructSource* Source, StructMachineCode* Code, const char* Command)
{
    seek (Source);

    char* Name = &(Source->Buffer[Source->pointer]);

    int i = 0;
    for (; i < Source->amnt_symbols - Source->pointer; i++)
    {
        if ((Name[i] == ' ') || (Name[i] == '\n'))
        {
            Name[i] = '\0';
            break;
        }
    }

    if ((Name[0] >= 'A') && (Name[0] <= 'z'))
    {
        print_command (Code, Command);

        jump_label (Source, Code);
        return 0;
    }
    if ((Name[0] >= '0') && (Name[0] <= '9'))
    {
        print_command (Code, Command);

        jump_int (Source, Code);
        return 0;
    }
    else
    {
        printf ("no label\n");
    }

    return -1;
}

void parse_int (StructSource* Source, StructMachineCode* Code,  const char* Command)
{
    //fprintf (Code->listing_file, "%0.4d %0.2X %s\n", Code->ip - 1, Code->ArrCode[Code->ip - 1], "push");
    print_command (Code, Command);

    char* Name = &(Source->Buffer[Source->pointer]);


    int length = strlen (Name);

    int i_argument = 0;
    sscanf (Name, "%d", &i_argument);


    int j = 0;
    for ( ;j < sizeof (int); j++)
    {
        //printf ("%x", ((unsigned char*) &d_argument)[j]);
        Code->ArrCode[Code->ip] = ((unsigned char*) &i_argument)[j];
        print_element (Code);
        Code->ip++;
    }

    if (strcmp (Command, "vsetx") == 0)
    {
        if (i_argument > Code->vram_size_x)
        {
            Code->vram_size_x = i_argument;
        }
    }
    if(strcmp (Command, "vsety") == 0)
    {
        if (i_argument > Code->vram_size_y)
        {
            Code->vram_size_y = i_argument;
        }
    }

    //printf ("'%s' %d %d\n", Name, length, i_argument);

    Source->pointer += length;

    //printf ("(%s) pointer %d\n", Name, *pointer);

    return;
}

void parse_double (StructSource* Source, StructMachineCode* Code,  const char* Command)
{
    //fprintf (Code->listing_file, "%0.4d %0.2X %s\n", Code->ip - 1, Code->ArrCode[Code->ip - 1], "push");
    print_command (Code, Command);

    char* Name = &(Source->Buffer[Source->pointer]);


    int length = strlen (Name);

    double d_argument = strtod (Name, NULL);


    int j = 0;
    for ( ;j < sizeof (double); j++)
    {
        //printf ("%x", ((unsigned char*) &d_argument)[j]);
        Code->ArrCode[Code->ip] = ((unsigned char*) &d_argument)[j];
        print_element (Code);
        Code->ip++;
    }

    //printf (" %s %lg\n", Name, d_argument);

    Source->pointer += length;

    //printf ("(%s) pointer %d\n", Name, *pointer);

    return;
}

void parse_str (StructSource* Source, StructMachineCode* Code, const char* Command)
{
    Code->ArrCode[Code->ip - 1] = (unsigned char)((int) Code->ArrCode[Code->ip - 1] + 64);

    print_command (Code, Command);

    char* Name = &(Source->Buffer[Source->pointer]);


    int length = strlen (Name);

    clear_comments (Name);

    for (int i = 0; i < length; i++)
    {
        if (Name[i] == ' ')
        {
            Name[i] = '\0';
        }
    }

    //printf ("!%s!\n", Name);

    int j = 0;
    for ( ; j < 4; j++)
    {
        if (strcmp (Name, ArrRegisters[j].name) == 0)
        {
            Code->ArrCode[Code->ip] = (unsigned char) ArrRegisters[j].num;
            print_element (Code);
            Code->ip++;
        }

        // if (Name[j] != '\0')
        // {
        //     Code->ArrCode[Code->ip + j] = (unsigned char) ((Name)[j]);
        //     fprintf (Code->listing_file, "%0.4d %0.2X \n", Code->ip + j, (unsigned char) ((Name)[j]));
        // }
    }

    //skip_line (Source);

    Source->pointer += length;

    //printf ("(%s) pointer %d\n", Name, *pointer);

    return;
}

void parse_op (StructSource* Source, StructMachineCode* Code, const char* Command)
{
    Code->ArrCode[Code->ip - 1] = (unsigned char)((int) Code->ArrCode[Code->ip - 1] + 128);

    //print_command (Code, Command);
    Source->pointer++;

    char* Name = &(Source->Buffer[Source->pointer]);

    //clear_comments (Name);


    int length = strlen (Name);

    if (Name[0] == 'r')
    {
        parse_str (Source, Code, Command);
    }
    else if ((Name[0] <= '9') && ((Name[0] >= '0')))
    {
        parse_int (Source, Code, Command);
    }
    else
    {
        printf ("2 op pzdc\n");
        return;
    }

}



void jump_int (StructSource* Source,StructMachineCode* Code)
{
    char* Name = &(Source->Buffer[Source->pointer]);

    int length = strlen (Name);

    int ip = 0;
    //printf ("'%s'\n", Name);
    sscanf (Name, "%d", &ip);

    //print_command (Code, "jump");

    for (int j = 0; j < sizeof (int); j++)
    {
        Code->ArrCode[Code->ip] = ((unsigned char*) &(ip))[j];
        print_element (Code);
        Code->ip++;
    }

    Source->pointer += length;

    return;
}

void jump_label (StructSource* Source,StructMachineCode* Code)
{
    char* Name = &(Source->Buffer[Source->pointer]);

    //printf ("%d %s\n", ROUND, Name);
    int old_number = Code->ip;

    int length = strlen (Name);

    //print_command (Code, "jump");

    for (int i = 0; i < LabelsAmnt; i++)
    {
        //printf ("%d %d %s\n", i, ArrLabels[i].num, (ArrLabels[i].num != -1) ? ArrLabels[i].name : "a" );
        if ((ArrLabels[i].num != -1) && (strcmp (Name, ArrLabels[i].name) == 0))
        {
            for (int j = 0; j < sizeof (int); j++)
            {
                Code->ArrCode[Code->ip] = ((unsigned char*) &(ArrLabels[i].num))[j];
                print_element (Code);
                Code->ip++;
            }
        }
    }

    if (old_number == Code->ip)
    {
        if (ROUND < 2)
        {
            //printf ("Warning!\n");
            fprintf (Code->listing_file, "skipped %d bytes (label '%s' not found)\n", sizeof (int), Name);
            Code->ip += sizeof (int);
        }
        else
        {
            printf ("No label found! --%s-- ip %d\n", Name, Code->ip);

            for (int i = 0; i < LabelsAmnt; i++)
            {
                printf ("%d %s\n", ArrLabels[i].num, ArrLabels[i].name);
            }

            exit (0);
        }
    }

    Source->pointer += length;
}



void print_command (StructMachineCode* Code, const char* Name)
{
    fprintf (Code->listing_file, "    %0.4d 0x%0.2X %s\n", Code->ip - 1, Code->ArrCode[Code->ip - 1], Name);
    //printf ("    %0.4d %0.2X %s\n", Code->ip - 1, Code->ArrCode[Code->ip - 1], Name);

    return;
}

void print_element (StructMachineCode* Code)
{
    fprintf (Code->listing_file, "    %0.4d 0x%0.2X\n", Code->ip, Code->ArrCode[Code->ip]);
    //printf ("    %0.4d %0.2X\n", Code->ip, Code->ArrCode[Code->ip]);

    return;
}

void make_bin_file (FILE* Bin, StructMachineCode* Code)
{
    free_labels (Code);

    close_listing_file (Code);

    // unsigned char Sgn     = 218;
    // unsigned char Version = 1;
    // unsigned char Amount  = Code->ip;

    //printf ("%0.2X %0.2X %0.2X\n", Sgn, Version, Amount);

    int size = Code->vram_size_x * Code->vram_size_y;

    fwrite (&Code->sygnature, 1, sizeof (Code->sygnature), Bin);
    fwrite (&Code->version,   1, sizeof (Code->version),   Bin);
    fwrite (&Code->size,      1, sizeof (Code->size),      Bin);
    fwrite (&size,            1, sizeof (size),            Bin);

    fwrite (Code->ArrCode, Code->ip, sizeof (*Code->ArrCode), Bin);

    return;
}


int clear_comments (char* Name)
{
    int length = strlen (Name);

    int i = 0;

    for (; i < length ; i++)
    {
        //printf ("%d %c\n", i, Name[i]);

        // if ((Name[i] == ']') && (Name[i + 1] == ';'))
        // {
        //     Name[i + 1] = ' ';
        //     Name[i + 2] = ';';
        //     return 0;
        // }
        // else if (Name[i] == ';')
        // {
        //     Name[i] = ' ';
        //     Name[i + 1] = ';';
        //     return 0;
        // }

        if ((Name[i] == ']') || (Name[i] == ';'))
        {
            Name[i] = ' ';
        }
    }

    // /printf ("'%s'\n", Name);

    return 0;
}

void free_labels (StructMachineCode* Code)
{
    fprintf (Code->listing_file, "---------------------------------------------------------------\n");
    for (int i = 0; i < LabelsAmnt; i++)
    {
        if (ArrLabels[i].num != -1)
        {
            fprintf (Code->listing_file,"freed label: %-3d %-15s %-5d [%0.2X %0.2X %0.2X %0.2X]\n",
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
