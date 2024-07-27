
#include "../../include/parser/all_stmt.h";
#include "../../include/parser/fun.h";
#include "../../include/parser/ast.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include <stdio.h>
#include <stdlib.h>

ASTNode *parseFunDefParameterList(Context *context)
{
    Token funtion_pram = {.lexeme = "FUNTION PARANERTERS", .value = UNKNOWN};
    ASTNode *paramList = createASTNode(funtion_pram);

    ASTNode *current = paramList;

    while (context->tokens[context->index].value == IDENTIFIER)
    {
        ASTNode *param = createASTNode(context->tokens[context->index]);
        current->next = param;
        current = param;
        SymbolTableEntry *entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);

        if (entry == NULL)

        { // create symbol entry in stack if there is no same symbol
            insertSymbol(context->stack, context->tokens[context->index].lexeme, context->stack->scope);
            entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);
            entry->isDefined = 1;
        }
        else if (entry->scope != context->stack->scope)
        {
            // entry is found but out of scope then create entry and set isDefined to true;
            insertSymbol(context->stack, context->tokens[context->index].lexeme, context->stack->scope);
            entry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);
            entry->isDefined = 1;
        }

        else
        {
            printf("\nERROR: multiple decleration of variable `%s`  at line %d and col %d\n", context->tokens[context->index].lexeme, context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
            exit(1);
        }

        (context->index)++;

        if (context->tokens[context->index].value == COMMA)
        {
            (context->index)++; // Move to the next parameter
        }
        else
        {
            break; // No more parameters
        }
    }

    if (context->tokens[context->index].value == CONSTANT)
    {
        printf("\nERROR: constant paratmeter in funtion defination at line %d", context->tokens[context->index].pos.line);
        exit(1);
    }
    return paramList;
}
ASTNode *parseFunCallParameterList(Context *context)
{
    Token funtion_pram = {.lexeme = "FUNTION PARANERTERS", .value = UNKNOWN};
    ASTNode *paramList = createASTNode(funtion_pram);
    ASTNode *current = paramList;

    while (context->tokens[context->index].value == IDENTIFIER || context->tokens[context->index].value == CONSTANT)
    {
        ASTNode *param = createASTNode(context->tokens[context->index]);
        current->next = param;
        current = param;
        (context->index)++;

        if (context->tokens[context->index].value == COMMA)
        {
            (context->index)++; // Move to the next parameter
        }
        else
        {
            break; // No more parameters
        }
    }
    return paramList;
}

ASTNode *parseFunctionDefinitionOrCall(Context *context)
{

    // Check for identifier (function name)
    ASTNode *funcNode = createASTNode(context->tokens[context->index]);
    FunctionTableEntry *funtionEntry = lookupFuntionSymbol(context->table, context->tokens[context->index].lexeme);
    SymbolTableEntry *symbolEntry = lookupSymbol(context->stack, context->tokens[context->index].lexeme);
    if (funtionEntry || symbolEntry)
    {
        printf("\nERROR: funtion name `%s` is alredy in use \n", context->tokens[context->index].lexeme);
        exit(1);
    }
    insertFuntionSymbol(context->table, context->tokens[context->index].lexeme);
    insertSymbol(context->stack, context->tokens[context->index].lexeme, 0);
    // printf("\nTooken:%s",context->tokens[context->index].lexeme);
    (context->index)++; // Move to the opening parenthesis
    (context->index)++; // Move to the parameter list or closing parenthesis

    // Parse parameter list
    if (context->tokens[context->index].value != CLOSE_PAREN)
    {
        int count = context->index;

        while (context->tokens[count].value != CLOSE_PAREN && context->tokens[count].value != TEOF)
        {
            count++;
        }

        if (context->tokens[count].value == TEOF)
        {
            printf("\nERROR: Expected ) at line %d ", context->tokens[context->index].pos.line);
            exit(1);
        }
        count++;

        if (context->tokens[count].value == SEMI_COLAN)
        {
            funcNode->left = parseFunCallParameterList(context);
            if (context->tokens[context->index].value == SEMI_COLAN)
            {
                (context->index)++;
                return funcNode;
            }
        }

        // change scope to funtion to create new funtion scope symbol table
        pushSymbolTable(context->stack);

        if (context->tokens[count].value == OPEN_CURLY_PAREN)
            funcNode->left = parseFunDefParameterList(context);
        // funcNode->left = parseFunCallParameterList(tokens, index);
    }

    // Check for closing parenthesis
    if (context->tokens[context->index].value != CLOSE_PAREN)
    {
        printf("\nExpected ')' after parameter list at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
    }
    (context->index)++; // Move to the function body (compound statement).

    // Parse function body
    funcNode->right = parseBlockStatement(context);

    // pop value from symbol table of cuurent funtional scope symbol table
    popSymbolTable(context->stack);

    return funcNode;
}
ASTNode *parseFunCall(Context *context)
{
    // Check for identifier (function name)
    FunctionTableEntry *funtionEntry = lookupFuntionSymbol(context->table, context->tokens[context->index].lexeme);
    if (funtionEntry == NULL)
    {
        printf("\nERROR: funtion is used before its defination at line %d and col %d \n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
    ASTNode *funcNode = createASTNode(context->tokens[context->index]);

    // printf("\nTooken:%s",context->tokens[context->index].lexeme);
    (context->index)++; // Move to the opening parenthesis
    (context->index)++; // Move to the parameter list or closing parenthesis

    // Parse parameter list
    if (context->tokens[context->index].value != CLOSE_PAREN)
    {

        funcNode->left = parseFunCallParameterList(context);
        if (context->tokens[context->index].value != CLOSE_PAREN)
        {
            printf("\nExpected ')' after parameter list at line %d and col %d", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        }
        (context->index)++; // skip to  semicolan

        return funcNode;
    }
    (context->index)++; // skip to semicolan;
    if (context->tokens[context->index].value != SEMI_COLAN)
    {
        printf("\nERROR: expected ; at line %d and col %d\n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col + 1);
        exit(1);
    }
    return funcNode;
}