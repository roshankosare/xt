#include "../../include/parser/ast.h"
#include "../../include/tokens/tokens.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *createASTNode(Token token)
{
    printf("\nCreated AST Node: %s", token.lexeme);
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->token = token;
    node->left = node->right = node->next = NULL;
    return node;
}

void freeASTNode(ASTNode *root)
{
    if (root == NULL)
    {
        return;
    }
    freeASTNode(root->left);
    freeASTNode(root->right);
    freeASTNode(root->next);
    free(root);
    return;
}
