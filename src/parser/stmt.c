
#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser/stmt.h"
#include "../../include/parser/all_stmt.h"
#include "../../include/parser/exp_stmt.h"
ASTNode *parseStatement(Context *context)
{

    switch (getTokenIntCodeValue(context->tokens[context->index].lexeme))
    {
    case INT_TOKEN_IF:
        return parseSelectionStatement(context);
    case INT_TOKEN_WHILE:
        return parseIterationStatement(context);
    case INT_TOKEN_VAR:
        return parseExpressionStatement(context);
    case INT_TOKEN_IDENTIFIER:
        return parseExpressionStatement(context);
    default:
        printf("\nERROR: unexpected token %s at line %d and col %d\n", context->tokens[context->index].pos.line, context->tokens[context->index].pos.col);
        exit(1);
    }
}