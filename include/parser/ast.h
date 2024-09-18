#ifndef AST
#define AST
#include <stdio.h>
#include <stdlib.h>
#include "../tokens/tokens.h"

typedef struct ASTNode
{
    Token token;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;

} ASTNode;



ASTNode *createASTNode(Token token);


#endif