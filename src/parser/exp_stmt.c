#include "../../include/parser/all_stmt.h"
#include "../../include/parser/fun.h"
#include "../../include/parser/exp_stmt.h"
#include "../../include/parser/ast.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/symboltable/functiontable.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseExpression(Context *context)
{

    if (isConditionalOperatorToken(context->tokens[context->index + 1].lexeme))
    {
        return parseConditionalExpression(context);
    }
    if (context->tokens[context->index + 1].value == OPEN_PAREN)
    {
        return parseFunCall(context);
    }
    return parseAssignmentExpression(context);
}

ASTNode *parseAssignmentExpression(Context *context)
{

    // store index value to update variable defined flag to set to true
    int count;
    count = context->index;
    int temp;

    ASTNode *left = parsePrimaryExpression(context);

    if (context->tokens[context->index].value == ASSIGN)
    {
        Token assignToken = context->tokens[(context->index)++];

        ASTNode *right = parseAdditiveExpression(context);
        ASTNode *assignNode = createASTNode(assignToken);
        // this block of code will run to set var x = exp; statement x->isDefined flag to true
        temp = context->index;
        context->index = count;
        parsePrimaryExpression(context);
        context->index = temp;
        assignNode->left = left;
        assignNode->right = right;
        return assignNode;
    }

    // this block of code will run to set var x; statement x->isDefined flag to true
    parsePrimaryExpression(context);

    return left;
}

ASTNode *parseAdditiveExpression(Context *context)
{
    ASTNode *left = parseMultiplicativeExpression(context);
    while (context->tokens[context->index].value == PLUS || context->tokens[context->index].value == MINUS)
    {
        Token opToken = context->tokens[(context->index)++];
        ASTNode *right = parseMultiplicativeExpression(context);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }
    return left;
}

ASTNode *parseMultiplicativeExpression(Context *context)
{
    ASTNode *left = parsePrimaryExpression(context);
    while (context->tokens[context->index].value == MUL || context->tokens[context->index].value == DIV)
    {
        Token opToken = context->tokens[(context->index)++];
        ASTNode *right = parsePrimaryExpression(context);
        ASTNode *opNode = createASTNode(opToken);
        opNode->left = left;
        opNode->right = right;
        left = opNode;
    }
    return left;
}

ASTNode *parsePrimaryExpression(Context *context)
{

    if (context->tokens[context->index].value == VAR)
    {
        (context->index)++;
        SymbolTableEntry *entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);
        FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->table, context->tokens[context->index].lexeme);
        if (functionEntry != NULL)
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
            exit(1);
        }

        if (entry == NULL)
        { // create symbol entry in stack if there is no same symbol
            insertSymbol(context->stack, context->tokens[context->index].lexeme, context->stack->scope);
            Token token = context->tokens[(context->index)++];
            return createASTNode(token);
        }
        else if (entry->scope != context->stack->scope)
        {

            insertSymbol(context->stack, context->tokens[context->index].lexeme, context->stack->scope);
            Token token = context->tokens[(context->index)++];
            return createASTNode(token);
        }
        else if (entry->isDefined == 0)
        {
            entry->isDefined = 1;
            return NULL;
        }
        else
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
            exit(1);
        }
    }
    SymbolTableEntry *entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);

    if (context->tokens[context->index].value == IDENTIFIER && entry == NULL)
    {
        printf("\nERROR: undefined variable `%s` at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    else if (context->tokens[context->index].value == IDENTIFIER && entry->isDefined == 0)
    {
        printf("\nERROR: variable used before its defined `%s` at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }

    Token token = context->tokens[(context->index)++];
    return createASTNode(token);
}
ASTNode *parseExpressionStatement(Context *context)
{

    ASTNode *exprNode = parseExpression(context);

    if (context->tokens[context->index].value != SEMI_COLAN)
    {
        --(context->index);
        // go back one token to get line and col number of previous token
        printf("\nError: Expected ';' at line %d, col %d\n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col + (int)strlen(context->tokens[context->index].lexeme));
        exit(1);
    }
    (context->index)++;
    return exprNode;
}

ASTNode *parseConditionalExpression(Context *context)
{
    ASTNode *left = parsePrimaryExpression(context);

    if (!isOperatorToken(context->tokens[context->index].lexeme))
    {
        printf("\nERROR: invalid conditional operator at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    ASTNode *condition = createASTNode(context->tokens[(context->index)++]);
    ASTNode *right = parsePrimaryExpression(context);
    condition->right = right;
    condition->left = left;
    return condition;
}