#ifndef EXPRESIION_STATEMENT_PARSER
#define EXPRESIION_STATEMENT_PARSER
#include "ast.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"

ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseConditionalExpression(Token *tokens, int *index, SymbolTableStack *stack);



#endif