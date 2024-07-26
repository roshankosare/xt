#include "../../include/parser/all_stmt.h"
#include "../../include/parser/fun.h"
#include "../../include/parser/exp_stmt.h"
#include "../../include/parser/ast.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/symboltable/functiontable.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{

    if (isConditionalOperatorToken(tokens[*index + 1].lexeme))
    {
        return parseConditionalExpression(tokens, index, stack, table);
    }
    if (tokens[*index + 1].value == OPEN_PAREN)
    {
        return parseFunCall(tokens, index, stack, table);
    }
    return parseAssignmentExpression(tokens, index, stack, table);
}

ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{

    // store index value to update variable defined flag to set to true
    int *count = (int *)malloc(sizeof(int));
    *count = *index;

    ASTNode *left = parsePrimaryExpression(tokens, index, stack, table);

    if (tokens[*index].value == ASSIGN)
    {
        Token assignToken = tokens[(*index)++];

        ASTNode *right = parseAdditiveExpression(tokens, index, stack, table);
        ASTNode *assignNode = createASTNode(assignToken);
        // this block of code will run to set var x = exp; statement x->isDefined flag to true
        parsePrimaryExpression(tokens, count, stack, table);
        assignNode->left = left;
        assignNode->right = right;
        return assignNode;
    }

    // this block of code will run to set var x; statement x->isDefined flag to true
    parsePrimaryExpression(tokens, index, stack, table);

    return left;
}

ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{
    ASTNode *left = parseMultiplicativeExpression(tokens, index, stack, table);
    while (tokens[*index].value == PLUS || tokens[*index].value == MINUS)
    {
        Token opToken = tokens[(*index)++];
        ASTNode *right = parseMultiplicativeExpression(tokens, index, stack, table);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }
    return left;
}

ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{
    ASTNode *left = parsePrimaryExpression(tokens, index, stack, table);
    while (tokens[*index].value == MUL || tokens[*index].value == DIV)
    {
        Token opToken = tokens[(*index)++];
        ASTNode *right = parsePrimaryExpression(tokens, index, stack, table);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }
    return left;
}

ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{

    if (tokens[*index].value == VAR)
    {
        (*index)++;
        SymbolTableEntry *entry = lookupSymbol(stack, tokens[*index].lexeme);
        FunctionTableEntry *functionEntry = lookupFuntionSymbol(table, tokens[*index].lexeme);
        if (functionEntry != NULL)
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
            exit(1);
        }

        if (entry == NULL)
        { // create symbol entry in stack if there is no same symbol
            insertSymbol(stack, tokens[*index].lexeme, stack->scope);
            Token token = tokens[(*index)++];
            return createASTNode(token);
        }
        else if (entry->scope != stack->scope)
        {

            insertSymbol(stack, tokens[*index].lexeme, stack->scope);
            Token token = tokens[(*index)++];
            return createASTNode(token);
        }
        else if (entry->isDefined == 0)
        {
            entry->isDefined = 1;
            return NULL;
        }
        else
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
            exit(1);
        }
    }
    SymbolTableEntry *entry = lookupSymbol(stack, tokens[*index].lexeme);

    if (tokens[*index].value == IDENTIFIER && entry == NULL)
    {
        printf("\nERROR: undefined variable `%s` at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    else if (tokens[*index].value == IDENTIFIER && entry->isDefined == 0)
    {
        printf("\nERROR: variable used before its defined `%s` at line %d and col %d\n", tokens[*index].lexeme, tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }

    Token token = tokens[(*index)++];
    return createASTNode(token);
}
ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{

    ASTNode *exprNode = parseExpression(tokens, index, stack, table);

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

ASTNode *parseConditionalExpression(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{
    ASTNode *left = parsePrimaryExpression(tokens, index, stack, table);

    if (!isOperatorToken(tokens[*index].lexeme))
    {
        printf("\nERROR: invalid conditional operator at line %d and col %d", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    ASTNode *condition = createASTNode(tokens[(*index)++]);
    ASTNode *right = parsePrimaryExpression(tokens, index, stack, table);
    condition->right = right;
    condition->left = left;
    return condition;
}