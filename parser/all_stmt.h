#ifndef ALL_STMT
#define ALL_STMT

ASTNode *parseSelectionStatement(Token *tokens, int *index);
ASTNode *parseIterationStatement(Token *tokens, int *index);
ASTNode *parseJumpStatement(Token *tokens, int *index);


ASTNode *parseSelectionStatement(Token *tokens, int *index)
{
}

ASTNode *parseIterationStatement(Token *tokens, int *index)
{
    // Implement iteration statement parsing (e.g., while loop)
}

ASTNode *parseJumpStatement(Token *tokens, int *index)
{
    // Implement jump statement parsing (e.g., return)
}

#endif