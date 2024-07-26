
#include <stdio.h>
#include <stdlib.h>
#include "../../includes/parser/stmt.h"
#include "../../includes/parser/all_stmt.h"
#include "../../includes/parser/exp_stmt.h"
#include "../../includes/tokens/tokens.h"
ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack)
{

    switch (getTokenIntCodeValue(tokens[*index].lexeme))
    {
    case INT_TOKEN_IF:
        return parseSelectionStatement(tokens, index, stack);
    case INT_TOKEN_WHILE:
        return parseIterationStatement(tokens, index, stack);
    case INT_TOKEN_VAR:
        return parseExpressionStatement(tokens, index, stack);
    case INT_TOKEN_IDENTIFIER:
        return parseExpressionStatement(tokens, index, stack);
    default:
        printf("\nERROR: unexpected token %s at line %d and col %d\n", tokens[*index].pos.line, tokens[*index].pos.col);
        exit(1);
    }
}