#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "proc.h"
#include "disasm.h"


int DBG = 0; //TODO put in CPU struct

static const int REGS_CONST = 5;

static const int dmpsz = 20;

static int start = 100;

static int width = 0;
static int height = 0;
static int vram_size = 0;

StructDisasm* Array;
StructMachineCode* Code;

StructDebug* ArrDbg;

int dis_command (StructCPU* CPU, int num)
{
    int n = -1;

    for (int i = 0; i < CPU->size; i++)
    {
        if (num == ArrDbg[i].counter)
        {
            n = i;
            break;
        }
    }

    if (n != -1)
    {
       // if ()
        printf (Kbright KYLW "%s" KNRM, ArrDbg[n].line);
    }

    return 0;
}

#undef DEF_CMD

void DODUMP (StructCPU* CPU) //TODO rename
{
    if (DBG != 1)
    {
        return;
    }

    system ("clear");

    printf ("\n");
    printf (Kbright "====DUMP LOG========\n" KNRM);
    printf ("%s %s\n\n", __DATE__, __TIME__);

    int min = (CPU->ip > dmpsz) ? CPU->ip - dmpsz : 0;
    int max = (CPU->ip < (CPU->size - dmpsz)) ? CPU->ip + dmpsz : CPU->size;

    printf (Kunderscore "Code line:\n" KNRM);
    for (int i = min; i < max; i++)
    {
        printf (KWHT "%04d|         0x%02X" , i, CPU->Array[i]);


        if (i == CPU->ip)
        {
            printf (Kreverse);
        }

        dis_command (CPU, i);

        if (i == CPU->ip)
        {
            printf (KYLW " <" KNRM);
            //dis_command (CPU, &n);
        }
        printf ("\n");
    }
    printf ("\n");

    printf (Kunderscore "Stack:\n" KNRM);
    int i = 0;
    for (; i < CPU->stack->size; i++)
    {
        printf (KBLU "%04d| %13lg\n" KNRM , i, CPU->stack->data[i]);
    }
    if (i == 0)
    {
        printf (KGRN "empty\n" KNRM);
    }
    printf ("\n");

    printf (Kunderscore "Address stack:\n" KNRM);
    i = 0;
    for (; i < CPU->addres_stack->size; i++)
    {
        printf (KMGN "%04d| %13lg\n" KNRM, i, CPU->addres_stack->data[i]);
    }
    if (i == 0)
    {
        printf (KGRN "empty\n" KNRM);
    }
    printf ("\n");

    printf (Kbright"====================\n" KNRM);

    // char* buff_str = (char*) calloc (sizeof (buff_str), 10);
    // scanf ("%s", buff_str);
    // if ((strcmp (buff_str, "n")) != 0)
    // {
    //     printf ("!no! '%s' \n", buff_str);
    //     exit (0);
    // }

    char cx = getchar ();
    if (cx != '\n')
    {
        printf ("!no way!\n");
        exit (0); //TODO return
    }

}

void cpu_constructor (FILE* Bin, StructCPU* CPU)
{
    check_passport (Bin, CPU);

    read_array_of_code (Bin, CPU);

    CPU->Regs = (double*) calloc (REGS_CONST, sizeof (*CPU->RAM));
    CPU->RAM  = (double*) calloc (start + vram_size + 10, sizeof (*CPU->RAM)); // TODO remove magic constant

    CPU->stack = (StructStack*) calloc (1, sizeof (StructStack));
    CPU->addres_stack = (StructStack*) calloc (1, sizeof (StructStack));

    STACKCTOR (CPU->stack, 20);         //TODO remove magic constants
    STACKCTOR (CPU->addres_stack, 20);

    return;
}


int check_passport (FILE* Bin, StructCPU* CPU) // TODO move to "binary-read-write" lib
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
    MCA (vram_size >= 0, 2);

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
    MCA (CPU  != NULL, -2);

    CPU->Array = (unsigned char*) calloc (CPU->size * 2, sizeof (*CPU->Array));
    fread (CPU->Array, CPU->size, sizeof (*CPU->Array), Bin);

    if (DBG == 1)
    {
        Code = (StructMachineCode*) calloc (1, sizeof (*Code)); // TODO move all of this to constructor
        Code->size = CPU->size;
        Code->ArrCode = CPU->Array;
        Code->ip = 0;

        Array = (StructDisasm*) calloc (1, sizeof (*Array));
        Array->Buffer = (unsigned char*) calloc (CPU->size, 4);
        Array->pointer = 0;

        ArrDbg = (StructDebug*) calloc (CPU->size * 2, sizeof (*ArrDbg));

        //make_text_from_code (Array, Code); //printf ("'%s'", Array->Buffer); exit (0);

        int c = 0; // TODO watch your naming

        for (; Code->ip < Code->size;)
        {
            ArrDbg[c].counter = Code->ip; //printf ("!%d\n", ArrDbg[c].counter);
            int i = Array->pointer; // TODO watch your naming

            if (read_command (Array, Code) == 1)
            {
                printf ("crush!\n");
                return 1; // TODO what is this
            }

            if (strncmp ((const char*) &(Array->Buffer[i]), "label", 5) == 0)
            {
                int x = 2; // TODO whach your naming

                while (Array->Buffer[Array->pointer - x] != '\0')
                {
                    //printf ("'%c'", Array->Buffer[Array->pointer - x]);
                    x++;
                }
                //printf ("=%s=", &(Array->Buffer[Array->pointer - x + 1]));

                //Array->pointer -= x - 1;

                ArrDbg[c].line = &(Array->Buffer[Array->pointer - x + 1]);

                c++;

                continue;
            }

            ArrDbg[c].line = &(Array->Buffer[i]);

            c++;

            // for (; i < CPU->size * 4 - 2 ; i++)
            // {
            //     if ((Array->Buffer[i] == '\0') && (Array->Buffer[i + 1] != '\0'))
            //     {
            //         ArrDbg[c].line = &(Array->Buffer[i + 1]);
            //         c++;
            //     }
            // }
        }

        //  for (int i = 0; i < 30; i++)
        // {
        //     printf ("%d '%s'\n", ArrDbg[i].counter, ArrDbg[i].line);
        // }
        // exit (0);
    }

    return 0;
}

int execute_code (StructCPU* CPU)
{
    for (; CPU->ip < CPU->size;)
    {
        if (CPU->ip == 0)
        {
            DODUMP (CPU);;
        }

        int i = execute_command (CPU); // TODO watch your naming

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
    return -5; // TODO what is this
}

#define DEF_CMD(name, e_num, num, f_proc, f_parse, f_reparse) \
case num: \
    f_proc \
    DODUMP (CPU); \
    return 0;

int execute_command (StructCPU* CPU)
{
    int marker = CPU->Array[CPU->ip] & 31;//TODO define mask on 5 bits

    for (int i = 0;  i < AmntCommands; i++)
    {
       // printf ("%0.4d %d %d\n", CPU->ip, ArrCommands[i].num, marker);
        if (ArrCommands[i].num == marker)
        {
            switch (marker)
            {
                #include "../ASM/commands.h"
                default:
                    printf ("default error!\n");
                    exit (0); // TODO remove
            }
        }
    }
    printf ("Unknown Command! marker = %d ip %d\n", marker, CPU->ip);

    return 6;
}
#undef DEF_CMD

void cpu_destructor (StructCPU* CPU)
{
    free (CPU->Array);
    free (CPU->RAM);
    free (CPU->Regs);

    stack_destructor (CPU->stack);
    stack_destructor (CPU->addres_stack);

    free (CPU->stack);
    free (CPU->addres_stack);

    if (DBG == 2)
    {
        free (Code);
        free (ArrDbg);
        free (Array->Buffer);
        free (Array);
    }
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
            exit (0); // TODO
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
    marker = ( CPU->Array[CPU->ip] >> 4 );
    CPU->ip++;

    switch (marker)
    {
        #include "Pop.h"
        default:
            printf ("pop error\n");
            exit (0); // TODO remove
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
