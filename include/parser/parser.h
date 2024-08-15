
#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../context/context.h"
#include "../tokens/tokens.h"


ASTNode *parseProgram(Context *context,Token *tokens, int *index, int token_count);
void printAST(ASTNode *node, int depth);





#endif