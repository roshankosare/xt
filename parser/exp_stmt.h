#ifndef EXPRESIION_STATEMENT_PARSER
#define EXPRESIION_STATEMENT_PARSER
#include "ast.h"
#include "../symboltable/symboltable.h"


ASTNode *parseExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAssignmentExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseAdditiveExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseMultiplicativeExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parsePrimaryExpression(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseExpressionStatement(Token *tokens, int *index, SymbolTableStack *stack);

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
        else if (entry->scope != stack->scope)
        {
           
            insertSymbol(stack, tokens[*index].lexeme, stack->scope);
        }
        else
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


#endif