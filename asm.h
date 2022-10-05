#ifndef ASM_H
#define ASM_H

int count_lines_in_file (FILE* stream);

int count_symbols_in_file (FILE* stream);

void separate_buffer_on_lines (char** ArrLinePtrs, const char* Buffer, int AmntLines);

//======================================================================

int make_array_of_code (char** ArrLinePtrs, int AmntLines, int* ArrCode, int* ArrNumber);


//void make_command_from_line (char* Line, int* ArrCode, int* ArrNumber);

void make_array_of_code (char* Buffer, int* ArrCode, int AmntSymbols, int AmntLines);

void seek (char* Buffer, int* pointer, int AmntSymbols);

//int parse (char* Name, int* Cursor, int* ArrCode, int* ArrNumber);
int parse (char* Buffer, int* pointer, int* ArrCode, int* size);

void parse_argument (char* Buffer, int* pointer, int* ArrCode, int* size);

//void parse_arguments (int AmntArgs, char* Name, int* Cursor, int* ArrCode, int* ArrNumber);

#endif