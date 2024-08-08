#ifndef FUN_H
#define FUN_H
#include "../parser/ast.h"
#include "../context/context.h"

ASTNode *parseDefParam(Context *context);
ASTNode *parseCallParam(Context *context);
ASTNode *parse_fun(Context *context);

#endif