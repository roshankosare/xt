#ifndef FUN_H
#define FUN_H
#include "../parser/ast.h"
#include "../context/context.h"

ASTNode *parse_fun(Context *context , FILE *fp);

#endif