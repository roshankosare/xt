#ifndef ASM_H
#define ASM_H


#include "../tokens/tokens.h"
#include "../parser/ast.h"
#include "../context/context.h"

void translate(ASTNode *ast,Context *context,FILE *fp);

#endif