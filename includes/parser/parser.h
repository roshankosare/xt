
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "../tokens/tokens.h"
#include "../symboltable/symboltable.h"
#include "ast.h"
#include "fun.h"
#include "stmt.h"

#define PARSER_IMPLEMENTATION

ASTNode *parseProgram(Token *tokens, int *index, int token_count);
void printAST(ASTNode *node, int depth);



#endif