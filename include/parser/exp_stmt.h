#ifndef EXPRESIION_STATEMENT_PARSER
#define EXPRESIION_STATEMENT_PARSER
#include "ast.h"
#include "../symboltable/symboltable.h"
#include "../symboltable/functiontable.h"
#include "../tokens/tokens.h"

ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);
ASTNode *parseConditionalExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table);

#endif