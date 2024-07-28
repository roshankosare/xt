
#include "../../include/parser/all_stmt.h"
#include "../../include/parser/ast.h"
#include "../../include/parser/exp_stmt.h" /
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/context/context.h"

#include <stdio.h>
#include <stdlib.h>

ASTNode *parseSelectionStatement(Context *context)
{

    // context->tokens[context->index].lexme = if*
    ASTNode *ifNode = createASTNode(context->tokens[context->index]);
    (context->index)++;

    if (context->tokens[context->index].value != OPEN_PAREN)
    {
        printf("\nERROR: expected ( at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index]);
        exit(1);
    }
    (context->index)++; // skip ( character

    // parse expression stmt or conditional stmt
    ASTNode *left = parseExpression(context);
    if (context->tokens[context->index].value != CLOSE_PAREN)
    {
        printf("\nERROR: expected ) token at if block at line %d ", context->tokens[context->index].pos.line);
        exit(1);
    }
    (context->index)++; // skip ) chracter
    if (context->tokens[context->index].value != OPEN_CURLY_PAREN)
    {
        ASTNode *right = parseExpressionStatement(context);
        ifNode->right = right;
        ifNode->left = left;
        return ifNode;
    }
    ASTNode *right = parseBlockStatement(context);
    ifNode->right = right;
    ifNode->left = left;
    return ifNode;
}

ASTNode *parseIterationStatement(Context *context)
{
    if (context->tokens[context->index].value == WHILE)
    {
        // context->tokens[context->index].lexme = if
        ASTNode *whileNode = createASTNode(context->tokens[context->index]);
        (context->index)++;

        if (context->tokens[context->index].value != OPEN_PAREN)
        {
            printf("\nERROR: expected ( at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index]);
            exit(1);
        }
        (context->index)++; // skip ( character

        // parse expression stmt or conditional stmt
        ASTNode *left = parseExpression(context);
        if (context->tokens[context->index].value != CLOSE_PAREN)
        {
            printf("\nERROR: expected ) token at if block at line %d ", context->tokens[context->index].pos.line);
            exit(1);
        }
        (context->index)++; // skip ) chracter
        if (context->tokens[context->index].value != OPEN_CURLY_PAREN)
        {
            ASTNode *right = parseExpressionStatement(context);
            whileNode->right = right;
            whileNode->left = left;
            return whileNode;
        }
        ASTNode *right = parseBlockStatement(context);
        whileNode->right = right;
        whileNode->left = left;
        return whileNode;
    }
}

ASTNode *parseJumpStatement(Context *context)
{
    ASTNode *returnNode = createASTNode(context->tokens[context->index]);
    (context->index)++;
    // if (context->tokens[context->index].value == OPEN_CURLY_PAREN)
    // {
    //     returnNode->right = parseBlockStatement(tokens, index, stack);
    //     if (context->tokens[context->index].value != SEMI_COLAN)
    //     {
    //         printf("\nERROR: expected ; at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col + 1);
    //         exit(1);
    //     }
    //     return returnNode;
    // }
    if (context->tokens[context->index].value == SEMI_COLAN)
    {
        (context->index)++;
        return returnNode;
    }
    returnNode->right = parseExpressionStatement(context);
    if (context->tokens[context->index].value != SEMI_COLAN)
    {
        printf("\nERROR: expected ; at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col + 1);
        exit(1);
    }
    return returnNode;
}

ASTNode *parseBlockStatement(Context *context)
{
    if (context->tokens[context->index].value != OPEN_CURLY_PAREN)
    {
        printf("\nError: Expected '{' at line %d, col %d\n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    (context->index)++;
    Token funtion_def = {.lexeme = "BLOCK", .value = UNKNOWN};
    ASTNode *compoundNode = createASTNode(funtion_def);

    ASTNode *current = compoundNode;
    while (context->tokens[context->index].value != CLOSE_CURLY_PAREN && context->tokens[context->index].value != TEOF)
    {

        switch (getTokenIntCodeValue(context->tokens[context->index].lexeme))
        {

        case INT_TOKEN_IF:
            current->next = parseSelectionStatement(context);
            current = current->next;
            break;
        case INT_TOKEN_WHILE:
            current->next = parseIterationStatement(context);
            current = current->next;
            break;
        case INT_TOKEN_RETURN:
            current->next = parseJumpStatement(context);
            current = current->next;
            break;
        case INT_TOKEN_VAR:
            current->next = parseBlockDeclerationStatement(context);
            current = current->next;
            break;
        case INT_TOKEN_IDENTIFIER:
            current->next = parseExpressionStatement(context);
            current = current->next;
            break;
        default:
            printf("\nERROR: unexpected token %s at line %d and col %d\n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
            exit(1);
        }
    }

    if (context->tokens[context->index].value != CLOSE_CURLY_PAREN)
    {
        printf("\nError: Expected '}' at line %d, col %d\n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    (context->index)++;

    return compoundNode;
}

ASTNode *parseDeclerationStatement(Context *context)
{

    ASTNode *varDecNode = createASTNode(context->tokens[(context->index)++]);
    int idp = context->index;
    int temp;
    if (context->tokens[context->index].value != IDENTIFIER)
    {
        printf("\nERROR: expected identifer at line %d and col %d ", context->tokens[context->index - 1].pos.line, context->tokens[context->index - 1].pos.col + 1);
        exit(1);
    }
    SymbolTableEntry *entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);
    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->table, context->tokens[context->index].lexeme);
    if (entry != NULL)
    {
        printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    if (functionEntry != NULL)
    {
        printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    insertSymbol(context->stack, context->tokens[context->index].lexeme, context->stack->scope);
    entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);

    (context->index)++; // skip to assign node;
    if (context->tokens[context->index].value == SEMI_COLAN)
    {
        entry->isDefined = 1;
        return varDecNode;
    }
    if (context->tokens[context->index].value != ASSIGN)
    {
        printf("\nERROR: expected  `=`  at line %d and col %d\n", context->tokens[context->index -1].pos.line, context->tokens[context->index -1].pos.col);
        exit(1);
    }
    ASTNode *assignNode = createASTNode(context->tokens[context->index]);
    varDecNode->right = assignNode;
    (context->index)++;

    assignNode->right = parseAdditiveExpression(context);

    if (context->tokens[context->index].value != SEMI_COLAN)
    {
        printf("\nERROR: expected  `;`  at line %d and col %d\n", context->tokens[context->index - 1].pos.line, context->tokens[context->index - 1].pos.col);
        exit(1);
    }
    (context->index)++;
    temp = context->index;
    context->index = idp;
    entry->isDefined = 1;
    assignNode->left = parsePrimaryExpression(context);
    context->index = temp;

    return varDecNode;
}

ASTNode *parseBlockDeclerationStatement(Context *context)
{
    Token token = {.lexeme = "var", .value = BLOCK_VAR};
    ASTNode *varDecNode = createASTNode(token);
    (context->index)++;
    int idp = context->index;
    int temp;
    if (context->tokens[context->index].value != IDENTIFIER)
    {
        printf("\nERROR: expected identifer at line %d and col %d ", context->tokens[context->index - 1].pos.line, context->tokens[context->index - 1].pos.col + 1);
    }
    SymbolTableEntry *entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);
    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->table, context->tokens[context->index].lexeme);
    if (entry != NULL)
    {
        printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    if (functionEntry != NULL)
    {
        printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    insertSymbol(context->stack, context->tokens[context->index].lexeme, context->stack->scope);
    entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);

    (context->index)++; // skip to assign node;
    if (context->tokens[context->index].value == SEMI_COLAN)
    {
        entry->isDefined = 1;
        return varDecNode;
    }
    if (context->tokens[context->index].value != ASSIGN)
    {
        printf("\nERROR: expected  `=`  at line %d and col %d\n", context->tokens[context->index -1].pos.line, context->tokens[context->index-1].pos.col);
        exit(1);
    }
    ASTNode *assignNode = createASTNode(context->tokens[context->index]);
    varDecNode->right = assignNode;
    (context->index)++;

    assignNode->right = parseAdditiveExpression(context);

    if (context->tokens[context->index].value != SEMI_COLAN)
    {
        printf("\nERROR: expected  `;`  at line %d and col %d\n", context->tokens[context->index-1].pos.line, context->tokens[context->index-1].pos.col);
        exit(1);
    }
    (context->index)++;
    temp = context->index;
    context->index = idp;
    entry->isDefined = 1;
    assignNode->left = parsePrimaryExpression(context);
    context->index = temp;

    return varDecNode;
}