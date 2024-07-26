#ifndef PARSE_STMT
#define PARSE_STMT

#include "ast.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"


ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack);



#endif