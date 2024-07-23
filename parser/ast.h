#ifndef AST
#define AST
#include<stdio.h>
#include<stdlib.h>
#include "../tokens.h"

typedef struct ASTNode
{
    Token token;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;

} ASTNode;

ASTNode *createASTNode(Token token)
{
    printf("\nCreated AST Node: %s", token.lexeme);
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->token = token;
    node->left = node->right = node->next = NULL;
    return node;
}

#endif