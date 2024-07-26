#ifndef ALL_STMT
#define ALL_STMT
#include "ast.h"
#include "../tokens/tokens.h"
#include "../symboltable/symboltable.h"
#include "../symboltable/functiontable.h"

ASTNode *parseSelectionStatement(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseIterationStatement(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseBlockStatement(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseJumpStatement(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);

#endif