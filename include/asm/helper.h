

#ifndef ASM_HELPER_H
#define ASM_HELPER_H
#include <stdio.h>
#include <stdlib.h>

void print_data_section(FILE *fp, const char *instruction);
void print_bss_section(FILE *fp, const char *instruction);
void print_rodata_section(FILE *fp, const char *instruction);
#endif