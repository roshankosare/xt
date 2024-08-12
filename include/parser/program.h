#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../context/context.h"
#include "../tokens/tokens.h"

ASTNode *program(Context *context);
ASTNode *fun(Context *context);

#endif