
#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser/stmt.h"
#include "../../include/parser/all_stmt.h"
#include "../../include/parser/exp_stmt.h"
#include "../../include/tokens/tokens.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack,FunctionTable *table)
{

    switch (getTokenIntCodeValue(tokens[*index].lexeme))
    {
    case INT_TOKEN_IF:
        return parseSelectionStatement(tokens, index, stack,table);
    case INT_TOKEN_WHILE:
        return parseIterationStatement(tokens, index, stack,table);
    case INT_TOKEN_VAR:
        return parseExpressionStatement(tokens, index, stack,table);
    case INT_TOKEN_IDENTIFIER:
        return parseExpressionStatement(tokens, index, stack,table);
    default:
        printf("\nERROR: unexpected token %s at line %d and col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
}