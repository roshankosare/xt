#ifndef EXP_H
#define EXP_H
#include "../parser/ast.h"
#include "../context/context.h"

ASTNode *exp(Context *context);
ASTNode *exp_stmt(Context *context);

#endif