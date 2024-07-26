#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser/fun.h"
#include "../../include/parser/stmt.h"
#include "../../include/parser/ast.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"

ASTNode *parseProgram(Token *tokens, int *index, int token_count);
void printAST(ASTNode *node, int depth);

ASTNode *parseProgram(Token *tokens, int *index, int token_count)
{
    Token program = {.lexeme = "Program", .value = UNKNOWN};
    ASTNode *programNode = createASTNode(program);
    ASTNode *current = programNode;
    int count = 0;

    SymbolTableStack stack;
    FunctionTable *table;
    table = initFuntionTable();
    initSymbolTableStack(&stack);
    pushSymbolTable(&stack);

    while (tokens[*index].value != TEOF)
    {

        if (tokens[*index].value == IDENTIFIER && tokens[*index + 1].value == OPEN_PAREN)
        {
            ASTNode *stmt = parseFunctionDefinitionOrCall(tokens, index, &stack, table);
            if (programNode == current)
            {
                current->right = stmt;
                current = stmt;
            }

            current->next = stmt;
            current = stmt;
            continue;
        }
        ASTNode *stmt = parseStatement(tokens, index, &stack, table);
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