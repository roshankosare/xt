#include "../../include/parser/ast.h"

ASTNode *createASTNode(Token token)
{
    printf("\nCreated AST Node: %s", token.lexeme);
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->token = token;
    node->left = node->right = node->next = NULL;
    return node;
}