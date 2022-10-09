#ifndef ASM_H
#define ASM_H

#define AmntCommands 12

typedef struct
{
    char* Buffer;
    int   amnt_symbols;
    int   amnt_lines;
    int   pointer;
} StructSource;

typedef struct
{
    int            top_number;
    unsigned char* ArrCode;
    unsigned char  sygnature;
    unsigned char  version;
    unsigned char  size;
    FILE*          listing_file;
} StructMachineCode;

//======================================================================

int count_lines_in_file (FILE* stream);

int count_symbols_in_file (FILE* stream);

void separate_buffer_on_lines (char** ArrLinePtrs, const char* Buffer, int AmntLines);

//======================================================================
void make_array_of_code (int Amnt_lines, StructSource* Source, StructMachineCode* Code);

void skip_line (StructSource* Source);

int seek (StructSource* Source);


int parse (StructSource* Source, StructMachineCode* Code);



int parse_push_or_pop (StructSource* Source, StructMachineCode* Code, const char* Command);

int parse_jump (StructSource* Source, StructMachineCode* Code);



void parse_op (StructSource* Source, StructMachineCode* Code, const char* Command);

void parse_int (StructSource* Source, StructMachineCode* Code,  const char* Command);

void parse_double (StructSource* Source, StructMachineCode* Code, const char* Command);

void parse_str (StructSource* Source, StructMachineCode* Code, const char* Command);


void jump_int (StructSource* Source, StructMachineCode* Code);

void jump_label (StructSource* Source, StructMachineCode* Code);



void print_command (StructMachineCode* Code, const char* Name);

void print_element (StructMachineCode* Code);

void make_bin_file (FILE* Bin, StructMachineCode* Code);


void free_labels (StructMachineCode* Code);

#endif
