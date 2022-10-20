#ifndef ASM_H
#define ASM_H

#include "stack.h"
#include <unistd.h>

//TODO colors lib
#define KBLK "\x1b[30m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KBLU "\x1B[34m"
#define KYLW "\x1b[33m"
#define KMGN "\x1b[35m"
#define KCYN "\x1b[36m"
#define KWHT "\x1b[37m"

#define KNRM "\x1B[0m"


#define Kbright "\x1b[1m"
#define Kdim "\x1b[2m"
#define Kunderscore "\x1b[4m"
#define Kblink "\x1b[5m"
#define Kreverse "\x1b[7m"
#define Khidden "\x1b[8m"



#define Kblack "\x1b[40m"
#define KGred "\x1b[41m"
#define Kgreen "\x1b[42m"
#define Kyellow "\x1b[43m"
#define Kblue "\x1b[44m"
#define Kmagenta "\x1b[45m"
#define Kcyan "\x1b[46m"
#define Kwhite "\x1b[47m"


#define AmntCommands 50
#define LabelsAmnt 30

#define DEF_CMD(name, e_num, num, ...) \
    e_num = num,

enum ECommandNums
{
    #include "commands.h"
};
#undef DEF_CMD

typedef struct
{
    ECommandNums  num;
    char          name[10];
} StructCommands;

typedef struct
{
    char* Buffer;
    int   amnt_symbols;
    int   amnt_lines;
    int   pointer;
} StructSource;

typedef struct
{
    int            ip;
    unsigned char* ArrCode;
    unsigned char  sygnature;
    unsigned char  version;
    int            vram_size_x;
    int            vram_size_y;
    int            size;
    FILE*          listing_file;
} StructMachineCode;

typedef struct
{
    int   num = -1;
    char* name;
} StructLabels;

#define DEF_CMD(name, e_num, ...) \
    {e_num, name},

const StructCommands ArrCommands[AmntCommands] =
{
    #include "commands.h"
};
#undef DEF_CMD

//======================================================================

int count_lines_in_file (FILE* stream);

int count_symbols_in_file (FILE* stream);

void separate_buffer_on_lines (char** ArrLinePtrs, const char* Buffer, int AmntLines);

//======================================================================
void make_array_of_code (int Amnt_lines, StructSource* Source, StructMachineCode* Code, const char* Filename);

void skip_line (StructSource* Source);

int seek (StructSource* Source);


int parse (StructSource* Source, StructMachineCode* Code);



int parse_push_or_pop (StructSource* Source, StructMachineCode* Code, const char* Command);

int parse_jump (StructSource* Source, StructMachineCode* Code, const char* Command);



void parse_op (StructSource* Source, StructMachineCode* Code, const char* Command);

void parse_int (StructSource* Source, StructMachineCode* Code,  const char* Command);

void parse_double (StructSource* Source, StructMachineCode* Code, const char* Command);

void parse_str (StructSource* Source, StructMachineCode* Code, const char* Command);


void jump_int (StructSource* Source, StructMachineCode* Code);

void jump_label (StructSource* Source, StructMachineCode* Code);



void print_command (StructMachineCode* Code, const char* Name);

void print_element (StructMachineCode* Code);

void make_bin_file (FILE* Bin, StructMachineCode* Code);


int clear_comments (char* Name);

void free_labels (StructMachineCode* Code);

#endif
