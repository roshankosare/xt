#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../context/context.h"
#include "../tokens/tokens.h"

void program(Context *context ,FILE *fp);
ASTNode *fun(Context *context,FILE *fp);

#endif