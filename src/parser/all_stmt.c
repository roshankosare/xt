
#include "../../includes/parser/all_stmt.h"
#include "../../includes/parser/ast.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSelectionStatement(Token *tokens, int *index, SymbolTableStack *stack)
{

    // tokens[*index].lexme = if
    ASTNode *ifNode = createASTNode(tokens[*index]);
    (*index)++;

    if (tokens[*index].value != OPEN_PAREN)
    {
        printf("\nERROR: expected ( at line %d and col %d", tokens[*index].pos.line, tokens[*index]);
        exit(1);
    }
    (*index)++; // skip ( character

    // parse expression stmt or conditional stmt
    ASTNode *left = parseExpression(tokens, index, stack);
    if (tokens[*index].value != CLOSE_PAREN)
    {
        printf("\nERROR: expected ) token at if block at line %d ", tokens[*index].pos.line);
        exit(1);
    }
    (*index)++; // skip ) chracter
    if (tokens[*index].value != OPEN_CURLY_PAREN)
    {
        ASTNode *right = parseExpressionStatement(tokens, index, stack);
        ifNode->right = right;
        ifNode->left = left;
        return ifNode;
    }
    ASTNode *right = parseBlockStatement(tokens, index, stack);
    ifNode->right = right;
    ifNode->left = left;
    return ifNode;
}

ASTNode *parseIterationStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    if (tokens[*index].value == WHILE)
    {
        // tokens[*index].lexme = if
        ASTNode *whileNode = createASTNode(tokens[*index]);
        (*index)++;

        if (tokens[*index].value != OPEN_PAREN)
        {
            printf("\nERROR: expected ( at line %d and col %d", tokens[*index].pos.line, tokens[*index]);
            exit(1);
        }
        (*index)++; // skip ( character

        // parse expression stmt or conditional stmt
        ASTNode *left = parseExpression(tokens, index, stack);
        if (tokens[*index].value != CLOSE_PAREN)
        {
            printf("\nERROR: expected ) token at if block at line %d ", tokens[*index].pos.line);
            exit(1);
        }
        (*index)++; // skip ) chracter
        if (tokens[*index].value != OPEN_CURLY_PAREN)
        {
            ASTNode *right = parseExpressionStatement(tokens, index, stack);
            whileNode->right = right;
            whileNode->left = left;
            return whileNode;
        }
        ASTNode *right = parseBlockStatement(tokens, index, stack);
        whileNode->right = right;
        whileNode->left = left;
        return whileNode;
    }
}

ASTNode *parseJumpStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    ASTNode *returnNode = createASTNode(tokens[*index]);
    (*index)++;
    // if (tokens[*index].value == OPEN_CURLY_PAREN)
    // {
    //     returnNode->right = parseBlockStatement(tokens, index, stack);
    //     if (tokens[*index].value != SEMI_COLAN)
    //     {
    //         printf("\nERROR: expected ; at line %d and col %d", tokens[*index].pos.line, tokens[*index].pos.col + 1);
    //         exit(1);
    //     }
    //     return returnNode;
    // }
    returnNode->right = parseExpressionStatement(tokens, index, stack);
    if (tokens[*index].value != SEMI_COLAN)
    {
        printf("\nERROR: expected ; at line %d and col %d", tokens[*index].pos.line, tokens[*index].pos.col + 1);
        exit(1);
    }
    return returnNode;
}

ASTNode *parseBlockStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    if (tokens[*index].value != OPEN_CURLY_PAREN)
    {
        printf("\nError: Expected '{' at line %d, col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    (*index)++;
    Token funtion_def = {.lexeme = "BLOCK", .value = UNKNOWN};
    ASTNode *compoundNode = createASTNode(funtion_def);

    ASTNode *current = compoundNode;
    while (tokens[*index].value != CLOSE_CURLY_PAREN && tokens[*index].value != TEOF)
    {

        switch (getTokenIntCodeValue(tokens[*index].lexeme))
        {

        case INT_TOKEN_IF:
            current->next = parseSelectionStatement(tokens, index, stack);
            current = current->next;
            break;
        case INT_TOKEN_WHILE:
            current->next = parseIterationStatement(tokens, index, stack);
            current = current->next;
            break;
        case INT_TOKEN_RETURN:
            current->next = parseJumpStatement(tokens, index, stack);
            current = current->next;
            break;
        case INT_TOKEN_VAR:
            current->next = parseExpressionStatement(tokens, index, stack);
            current = current->next;
            break;
        case INT_TOKEN_IDENTIFIER:
            current->next = parseExpressionStatement(tokens, index, stack);
            current = current->next;
            break;
        default:
            printf("\nERROR: unexpected token %s at line %d and col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
            exit(1);
        }
    }

    if (tokens[*index].value != CLOSE_CURLY_PAREN)
    {
        printf("\nError: Expected '}' at line %d, col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    (*index)++;

    return compoundNode;
}