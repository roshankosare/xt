#ifndef ASM_STMT_H
#define ASM_STMT_H
#include <stdio.h>
#include <stdlib.h>
#include "../../include/context/context.h"
#include "../parser/ast.h"

void print_if(ASTNode *ast, Context *context, FILE *fp);
void print_while(ASTNode *ast, Context *context, FILE *fp);

void print_var(ASTNode *ast, Context *context, FILE *fp);
void print_function(ASTNode *ast, Context *context, FILE *fp);
void print_assign(ASTNode *ast, Context *context, FILE *fp);
void print_return(ASTNode *ast, Context *context, FILE *fp);
void print_continue(ASTNode *ast, Context *context, FILE *fp);
void print_break(ASTNode *ast, Context *context, FILE *fp);

void print_body_start(ASTNode *ast, Context *context, FILE *fp);
void print_body_end(ASTNode *ast, Context *context, FILE *fp);

void print_asm(ASTNode *ast, Context *context, FILE *fp);

#endif