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
ASTQueue *initASTQueue()
{
    ASTQueue *queue = (ASTQueue *)malloc(sizeof(ASTQueue));
    if (queue == NULL)
    {
        printf("\nERRROR: failed to allocate memory to ASTQueue");
        exit(1);
    }
    queue->queueFront = NULL;
    queue->num_elements = 0;
    return queue;
}

void pushASTQnodeInQueue(ASTQueue *queue, ASTNode *ast, char *label)
{
    ASTQNode *node = (ASTQNode *)malloc(sizeof(ASTQNode));
    node->next = NULL;
    node->ast = ast;
    node->label = label;
    ASTQNode *queuefront = queue->queueFront;
    queue->num_elements++;
    if (queuefront == NULL)
    {
        queue->queueFront = node;
        return;
    }
    while (queuefront->next != NULL)
    {
        queuefront = queuefront->next;
    }
    queuefront->next = node;
    return;
}

ASTQNode *popFromASTQueueFront(ASTQueue *queue)
{
    ASTQNode *node = queue->queueFront;
    if (node == NULL)
    {
        return NULL;
    }
    queue->queueFront = node->next;
    node->next = NULL;
    queue->num_elements--;
    return node;
}
ASTQNode *popFromASTQueueRear(ASTQueue *queue)
{
    ASTQNode *next = queue->queueFront;
    ASTQNode *previous = queue->queueFront;
    if (next == NULL)
    {
        return NULL;
    }

    while (next->next != NULL)
    {
        previous = next;
        next = next->next;
    }
    if (next == previous)
    {
        queue->queueFront = NULL;
    }
    queue->num_elements--;
    previous->next = NULL;
    next->next = NULL;
    return next;
}

ASTStack *initASTStack()
{
    ASTStack *stack = (ASTStack *)malloc(sizeof(ASTStack));
    stack->top = NULL;
    return stack;
}

void pushASTStack(ASTStack *stack, ASTQNode *node)
{
    node->next = stack->top;
    stack->top = node;
}
ASTQNode *popASTStack(ASTStack *stack)
{
    if (stack->top != NULL)
    {
        ASTQNode *top = stack->top;
        stack->top = top->next;
        top->next = NULL;
        return top;
    }
    return NULL;
}