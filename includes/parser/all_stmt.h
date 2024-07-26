#ifndef ALL_STMT
#define ALL_STMT
#include "ast.h"
#include "../tokens/tokens.h"
#include "../symboltable/symboltable.h"


ASTNode *parseSelectionStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseIterationStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseBlockStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseJumpStatement(Token *tokens, int *index, SymbolTableStack *stack);




#endif