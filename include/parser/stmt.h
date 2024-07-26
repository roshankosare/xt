#ifndef PARSE_STMT
#define PARSE_STMT

#include "ast.h"
#include "../symboltable/symboltable.h"
#include "../symboltable/functiontable.h"
#include "../tokens/tokens.h"


ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack,FunctionTable *table);



#endif