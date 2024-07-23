#ifndef EXPRESIION_STATEMENT_P
#define EXPRESIION_STATEMENT_P


ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack);

#endif