#ifndef ALL_STMT
#define ALL_STMT

ASTNode *parseSelectionStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseIterationStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseJumpStatement(Token *tokens, int *index, SymbolTableStack *stack);

ASTNode *parseSelectionStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
}

ASTNode *parseIterationStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    // Implement iteration statement parsing (e.g., while loop)
}

ASTNode *parseJumpStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    // Implement jump statement parsing (e.g., return)
}

#endif