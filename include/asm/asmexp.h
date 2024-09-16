#ifndef ASM_EXP_H
#define ASM_EXP_H
#include <stdio.h>
#include <stdlib.h>
#include "../../include/context/context.h"
#include "../parser/ast.h"

void print_identifier(ASTNode *ast, Context *context, FILE *fp);
void print_string_constant(ASTNode *ast, Context *context, FILE *fp);
void print_integer_constant(ASTNode *ast, Context *context, FILE *fp);
void print_float_constant(ASTNode *ast, Context *context, FILE *fp);
void print_hex_constant(ASTNode *ast, Context *context, FILE *fp);

void print_add(FILE *fp);
void print_sub(FILE *fp);
void print_mul(FILE *fp);
void print_div(FILE *fp);
void print_modulas(FILE *fp);
void print_inc(ASTNode *ast, Context *context, FILE *fp);
void print_dec(ASTNode *ast, Context *context, FILE *fp);
void print_valueat(FILE *fp);
void print_typeof(FILE *fp);

void print_less_than(FILE *fp);
void print_grater_than(FILE *fp);
void print_lessthan_eqto(FILE *fp);
void print_graterthan_eqto(FILE *fp);
void print_eqto(FILE *fp);
void print_noteqto(FILE *fp);

void print_logical_and(FILE *fp);
void print_logigcal_or(FILE *fp);
void print_logical_not(FILE *fp);

void print_bitwise_and(FILE *fp);
void print_bitwise_or(FILE *Fp);
void print_bitwise_not(FILE *fp);

void print_funtion_call(ASTNode *ast, Context *context, FILE *fp);

#endif