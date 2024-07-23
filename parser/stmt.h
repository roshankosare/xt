#ifndef PARSE_BLOCK_STMT
#define PARSE_BLOCK_STMT
#include "ast.h"
#include "../symboltable/symboltable.h"
#include "exp_stmt.h"
#include "../tokens/tokens.h"

ASTNode *parseBlockStatement(Token *tokens, int *index, SymbolTableStack *stack);
ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack);

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

ASTNode *parseStatement(Token *tokens, int *index, SymbolTableStack *stack)
{
    if (tokens[*index].value == KEYWORD)
    {
        switch (getTokenIntCodeValue(tokens[*index].lexeme))
        {
        // case INT_TOKEN_IF:
        //     return parseSelectionStatement(tokens, index);
        // case INT_TOKEN_WHILE:
        //     return parseIterationStatement(tokens, index);
        // case INT_TOKEN_RETURN:
        //     return parseJumpStatement(tokens, index);
        default:
            return parseExpressionStatement(tokens, index, stack);
        }
    }
    else
    {
        return parseExpressionStatement(tokens, index, stack);
    }
}

#endif