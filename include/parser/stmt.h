#ifndef STMT_H
#define STMT_H
#include "../parser/ast.h"
#include "../context/context.h"

void stmt(Context *context, FILE *fp);
void dec_stmt(Context *context, FILE *fp);
void selc_stmt(Context *context, FILE *fp);
void iter_stmt(Context *context, FILE *fp);
void dec_stmt(Context *context, FILE *fp);
void jump_stmt(Context *context, FILE *fp);
void parse_block(Context *context, FILE *fp);
void parseAsm(Context *context, FILE *fp);

#endif