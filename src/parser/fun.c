
#include "../../include/parser/all_stmt.h";
#include "../../include/parser/fun.h";
#include "../../include/parser/ast.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include <stdio.h>
#include <stdlib.h>

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
            entry = lookupSymbol(stack, tokens[*index].lexeme);
            entry->isDefined = 1;
        }
        else if (entry->scope != stack->scope)
        {
            // entry is found but out of scope then create entry and set isDefined to true;
            insertSymbol(stack, tokens[*index].lexeme, stack->scope);
            entry = lookupSymbol(stack, tokens[*index].lexeme);
            entry->isDefined = 1;
        }

        else
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

ASTNode *parseFunctionDefinitionOrCall(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{

    // Check for identifier (function name)
    ASTNode *funcNode = createASTNode(tokens[*index]);
    FunctionTableEntry *funtionEntry = lookupFuntionSymbol(table, tokens[*index].lexeme);
    SymbolTableEntry *symbolEntry = lookupSymbol(stack, tokens[*index].lexeme);
    if (funtionEntry || symbolEntry)
    {
        printf("\nERROR: fution name `%s` is alredy in use \n", tokens[*index].lexeme);
        exit(1);
    }
    insertFuntionSymbol(table, tokens[*index].lexeme);
    insertSymbol(stack, tokens[*index].lexeme, 0);
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
    funcNode->right = parseBlockStatement(tokens, index, stack, table);

    // pop value from symbol table of cuurent funtional scope symbol table
    popSymbolTable(stack);

    return funcNode;
}
ASTNode *parseFunCall(Token *tokens, int *index, SymbolTableStack *stack, FunctionTable *table)
{
    // Check for identifier (function name)
    FunctionTableEntry *funtionEntry = lookupFuntionSymbol(table, tokens[*index].lexeme);
    if (funtionEntry == NULL)
    {
        printf("\nERROR: funtion is used before its defination at line %d and col %d \n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
    ASTNode *funcNode = createASTNode(tokens[*index]);

    // printf("\nTooken:%s",tokens[*index].lexeme);
    (*index)++; // Move to the opening parenthesis
    (*index)++; // Move to the parameter list or closing parenthesis

    // Parse parameter list
    if (tokens[*index].value != CLOSE_PAREN)
    {

        funcNode->left = parseFunCallParameterList(tokens, index, stack);
        if (tokens[*index].value != CLOSE_PAREN)
        {
            printf("\nExpected ')' after parameter list at line %d and col %d", tokens[*index].pos.line, tokens[*index].pos.col);
        }
        (*index)++; // skip to  semicolan

        return funcNode;
    }
    (*index)++; // skip to semicolan;
    if (tokens[*index].value != SEMI_COLAN)
    {
        printf("\nERROR: expected ; at line %d and col %d\n", tokens[*index].pos.line, tokens[*index].pos.col + 1);
        exit(1);
    }
    return funcNode;
}