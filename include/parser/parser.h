
#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../context/context.h"
#include "../tokens/tokens.h"


void parseProgram(Context *context, FILE *op);
void printAST(ASTNode *node, int depth);





#endif