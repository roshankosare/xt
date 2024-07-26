#ifndef PARSE_FUN_DEF
#define PARSE_FUN_DEF
#include "ast.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"
ASTNode *parseFunctionDefinitionOrCall(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseFunCallParameterList(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseFunDefParameterList(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseFunCall(Token *tokens, int *index, SymbolTableStack *stack);


#endif