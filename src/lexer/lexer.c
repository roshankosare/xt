
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../include/tokens/tokens.h"
#include "../../include/tokens/tokenizer.h"

Token *lexer(FILE *fp, int *tokenCount)
{
    Token *tokens = tokenizeFile(fp, tokenCount);
    for (int i = 0; i < *tokenCount; i++)
    {
        fillTokenValue(&tokens[i]);
    }
    return tokens;
}
void printTokens(Token *tokens, int tokenCount)
{

    for (int i = 0; i <= tokenCount; i++)
    {
        printf("\nLexme: %s ", tokens[i].lexeme);
        printf("\nValue: %s", getTokenStringValue(tokens[i].value));
        printf("\npos: line %d  col %d ", tokens[i].pos.line, tokens[i].pos.col);
        printf("\n----------------------------------------------------------");
    }

    for (int i = 0; i <= tokenCount; i++)
    {
        if (tokens[i].value == UNKNOWN)
        {
            printf("\nERROR:- unkown token at line %d and col %d", tokens[i].pos.line, tokens[i].pos.col);
        }
    }
}