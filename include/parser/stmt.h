#ifndef STMT_H
#define STMT_H
#include "../parser/ast.h"
#include "../context/context.h"

ASTNode *stmt(Context *context, FILE *fp);
ASTNode *dec_stmt(Context *context, FILE *fp);
ASTNode *selc_stmt(Context *context, FILE *fp);
ASTNode *iter_stmt(Context *context, FILE *fp);
ASTNode *dec_stmt(Context *context, FILE *fp);
ASTNode *jump_stmt(Context *context, FILE *fp);
ASTNode *parse_block(Context *context, FILE *fp);
ASTNode *parseAsm(Context *context, FILE *fp);

#endif