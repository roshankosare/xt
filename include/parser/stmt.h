#ifndef STMT_H
#define STMT_H
#include "../parser/ast.h"
#include "../context/context.h"

ASTNode *stmt(Context *context);
ASTNode *dec_stmt(Context *context);
ASTNode *selc_stmt(Context *context);
ASTNode *iter_stmt(Context *context);
ASTNode *dec_stmt(Context *context);
ASTNode *jump_stmt(Context *context);
ASTNode *parse_block(Context *context);

#endif