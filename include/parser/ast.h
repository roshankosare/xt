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

typedef struct ASTQNode
{
    ASTNode *ast;
    struct ASTQnode *next;
    char *label;
} ASTQNode;

typedef struct ASTQueue
{
    ASTQNode *queueFront;
    int num_elements;

} ASTQueue;

typedef struct ASTStack
{
    ASTQNode *top;
} ASTStack;

ASTNode *createASTNode(Token token);
void pushASTQnodeInQueue(ASTQueue *queue, ASTNode *ast, char *label);
ASTQNode *popFromASTQueueFront(ASTQueue *queue);
ASTQNode *popFromASTQueueRear(ASTQueue *queue);
void pushASTStack(ASTStack *stack,ASTQNode *node);
ASTQNode *popASTStack(ASTStack *stack);
ASTQueue *initASTQueue();
ASTStack *initASTStack();

#endif