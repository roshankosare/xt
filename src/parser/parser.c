#include <stdio.h>
#include <stdlib.h>

#include "../../include/parser/ast.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/context/context.h"
#include "../../include/parser/program.h"
#include "../../include/parser/helper.h"

ASTNode *parseProgram(Token *tokens, int *index, int token_count)
{
    Token token = {.lexeme = "Program", .value = UNKNOWN};
    ASTNode *programNode = createASTNode(token);
    ASTNode *current = programNode;
    int count = 0;

    Context *context = initContext(tokens);

    initSymbolTableStack(context->stack);
    pushSymbolTable(context->stack);
    int i = 0;
    while (context->current.value != TEOF && i < 1000)
    {
        i++;
        ASTNode *stmt = program(context);
        if (programNode == current)
        {
            current->right = stmt;
            current = stmt;
            continue;
        }

        current->next = stmt;
        current = stmt;
    }

    return programNode;
}

void printAST(ASTNode *node, int depth)
{

    if (node == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < depth; ++i)
    {
        printf("  ");
    }

    // Print the current node's token
    printf("%s -> %s", node->token.lexeme, getTokenStringValue(node->token.value));
    printf("\n");

    // Recursively print the left and right children
    if (node->left)
    {

        printAST(node->left, depth + 1);
    }
    if (node->right)
    {

        printAST(node->right, depth + 1);
    }
    if (node->next)
    {
        printAST(node->next, depth);
    }
}