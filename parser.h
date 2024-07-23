
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "lex_helper.h"
#include "symboltable.h"

#define PARSER_IMPLEMENTATION

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
ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseBlockStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseSelectionStatement(Token *tokens, int *index);
ASTNode *parseIterationStatement(Token *tokens, int *index);
ASTNode *parseJumpStatement(Token *tokens, int *index);
ASTNode *parseFunctionDefinitionOrCall(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseFunCallParameterList(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseFunDefParameterList(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseProgram(Token *tokens, int *index, int token_count);
void printAST(ASTNode *node, int depth);

ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack)
{

    return parseAssignmentExpression(tokens, index, stack);
}

ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack)
{
    ASTNode *left = parsePrimaryExpression(tokens, index, stack);

    if (tokens[*index].value == ASSIGN)
    {
        Token assignToken = tokens[(*index)++];
        ASTNode *right = parseAdditiveExpression(tokens, index, stack);
        ASTNode *assignNode = createASTNode(assignToken);
        assignNode->left = left;
        assignNode->right = right;
        return assignNode;
    }

    return left;
}

ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack)
{
    ASTNode *left = parseMultiplicativeExpression(tokens, index, stack);
    while (tokens[*index].value == PLUS || tokens[*index].value == MINUS)
    {
        Token opToken = tokens[(*index)++];
        ASTNode *right = parseMultiplicativeExpression(tokens, index, stack);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }
    return left;
}

ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack)
{
    ASTNode *left = parsePrimaryExpression(tokens, index, stack);
    while (tokens[*index].value == MUL || tokens[*index].value == DIV)
    {
        Token opToken = tokens[(*index)++];
        ASTNode *right = parsePrimaryExpression(tokens, index, stack);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }
    return left;
}

ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack)
{

    if (tokens[*index].value == VAR)
    {
        (*index)++;
        SymbolTableEntry *entry = lookupSymbol(stack, tokens[*index].lexeme);

        if (entry == NULL)
        { // create symbol entry in stack if there is no same symbol
            insertSymbol(stack, tokens[*index].lexeme, stack->scope);
        }
        else if (entry->scope == stack->scope)
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
            exit(1);
        }
    }
    SymbolTableEntry *entry = lookupSymbol(stack, tokens[*index].lexeme);

    if (tokens[*index].value == IDENTIFIER && (lookupSymbol(stack, tokens[*index].lexeme) == NULL))
    {
        printf("\nERROR: undecleared variable `%s` at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }

    Token token = tokens[(*index)++];
    return createASTNode(token);
}

ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    if (tokens[*index].value == KEYWORD)
    {
        switch (getTokenIntCodeValue(tokens[*index].lexeme))
        {
        case INT_TOKEN_IF:
            return parseSelectionStatement(tokens, index);
        case INT_TOKEN_WHILE:
            return parseIterationStatement(tokens, index);
        case INT_TOKEN_RETURN:
            return parseJumpStatement(tokens, index);
        default:
            return parseExpressionStatement(tokens, index, stack);
        }
    }
    else
    {
        return parseExpressionStatement(tokens, index, stack);
    }
}

ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack)
{

    ASTNode *exprNode = parseExpression(tokens, index, stack);

    if (tokens[*index].value != SEMI_COLAN)
    {
        --(*index);
        // go back one token to get line and col number of previous token
        printf("\nError: Expected ';' at line %d, col %d\n", tokens[*index].pos.line, tokens[*index].pos.col + (int)strlen(tokens[*index].lexeme));
        exit(1);
    }
    (*index)++;
    return exprNode;
}

ASTNode *parseBlockStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    if (tokens[*index].value != OPEN_CURLY_PAREN)
    {
        printf("\nError: Expected '{' at line %d, col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    (*index)++;
    Token funtion_def = {.lexeme = "FUNTION DEFINATION", .value = UNKNOWN};
    ASTNode *compoundNode = createASTNode(funtion_def);

    ASTNode *current = compoundNode;
    while (tokens[*index].value != CLOSE_CURLY_PAREN && tokens[*index].value != TEOF)
    {
        printf("\n last token: %s", tokens[*index].lexeme);
        current->next = parseExpressionStatement(tokens, index, stack);
        current = current->next;
    }

    if (tokens[*index].value != CLOSE_CURLY_PAREN)
    {
        printf("\nError: Expected '}' at line %d, col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    (*index)++;
    return compoundNode;
}

ASTNode *parseFunDefParameterList(Token *tokens, int *index, SymbolTableStack *stack)
{
    Token funtion_pram = {.lexeme = "FUNTION PARANERTERS", .value = UNKNOWN};
    ASTNode *paramList = createASTNode(funtion_pram);

    ASTNode *current = paramList;

    while (tokens[*index].value == IDENTIFIER)
    {
        ASTNode *param = createASTNode(tokens[*index]);
        current->next = param;
        current = param;
        SymbolTableEntry *entry = lookupSymbol(stack, tokens[*index].lexeme);

        if (entry == NULL)
        { // create symbol entry in stack if there is no same symbol
            insertSymbol(stack, tokens[*index].lexeme, stack->scope);
        }
        else if (entry->scope == stack->scope)
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
            exit(1);
        }

        (*index)++;

        if (tokens[*index].value == COMMA)
        {
            (*index)++; // Move to the next parameter
        }
        else
        {
            break; // No more parameters
        }
    }

    if (tokens[*index].value == CONSTANT)
    {
        printf("\nERROR: constant paratmeter in funtion defination at line %d", tokens[*index].pos.line);
        exit(1);
    }
    return paramList;
}
ASTNode *parseFunCallParameterList(Token *tokens, int *index, SymbolTableStack *stack)
{
    Token funtion_pram = {.lexeme = "FUNTION PARANERTERS", .value = UNKNOWN};
    ASTNode *paramList = createASTNode(funtion_pram);
    ASTNode *current = paramList;

    while (tokens[*index].value == IDENTIFIER || tokens[*index].value == CONSTANT)
    {
        ASTNode *param = createASTNode(tokens[*index]);
        current->next = param;
        current = param;
        (*index)++;

        if (tokens[*index].value == COMMA)
        {
            (*index)++; // Move to the next parameter
        }
        else
        {
            break; // No more parameters
        }
    }
    return paramList;
}

ASTNode *parseFunctionDefinitionOrCall(Token *tokens, int *index, SymbolTableStack *stack)
{

    if (tokens[*index].value == IDENTIFIER)
    {
        // Check for identifier (function name)
        ASTNode *funcNode = createASTNode(tokens[*index]);
        // printf("\nTooken:%s",tokens[*index].lexeme);
        (*index)++; // Move to the opening parenthesis
        (*index)++; // Move to the parameter list or closing parenthesis

        // Parse parameter list
        if (tokens[*index].value != CLOSE_PAREN)
        {
            int count = *index;

            while (tokens[count].value != CLOSE_PAREN && tokens[count].value != TEOF)
            {
                count++;
            }

            if (tokens[count].value == TEOF)
            {
                printf("\nERROR: Expected ) at line %d ", tokens[*index].pos.line);
                exit(1);
            }
            count++;

            if (tokens[count].value == SEMI_COLAN)
            {
                funcNode->left = parseFunCallParameterList(tokens, index, stack);
                if (tokens[*index].value == SEMI_COLAN)
                {
                    (*index)++;
                    return funcNode;
                }
            }

            // change scope to funtion to create new funtion scope symbol table
            pushSymbolTable(stack);

            if (tokens[count].value == OPEN_CURLY_PAREN)
                funcNode->left = parseFunDefParameterList(tokens, index, stack);
            // funcNode->left = parseFunCallParameterList(tokens, index);
        }

        // Check for closing parenthesis
        if (tokens[*index].value != CLOSE_PAREN)
        {
            printf("\nExpected ')' after parameter list at line %d and col %d", tokens[*index].pos.line, tokens[*index].pos.col);
        }
        (*index)++; // Move to the function body (compound statement).

        // Parse function body
        funcNode->right = parseBlockStatement(tokens, index, stack);

        // pop value from symbol table of cuurent funtional scope symbol table
        popSymbolTable(stack);

        return funcNode;
    }
    return NULL;
}

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

ASTNode *parseProgram(Token *tokens, int *index, int token_count)
{
    Token program = {.lexeme = "Program", .value = UNKNOWN};
    ASTNode *programNode = createASTNode(program);
    ASTNode *current = programNode;
    int count = 0;

    SymbolTableStack stack;
    initSymbolTableStack(&stack);
    pushSymbolTable(&stack);

    while (tokens[*index].value != TEOF)
    {

        if (tokens[*index].value == IDENTIFIER && tokens[*index + 1].value == OPEN_PAREN)
        {
            ASTNode *stmt = parseFunctionDefinitionOrCall(tokens, index, &stack);
            if (programNode == current)
            {
                current->right = stmt;
                current = stmt;
            }

            current->next = stmt;
            current = stmt;
            continue;
        }
        ASTNode *stmt = parseStatement(tokens, index, &stack);
        if (programNode == current)
        {
            current->right = stmt;
            current = stmt;
        }

        current->next = stmt;
        current = stmt;
    }

    return programNode;
}

void printAST(ASTNode *node, int depth)
{

    if (node == NULL)
        return;

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

#endif