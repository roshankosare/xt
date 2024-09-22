
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../include/tokens/tokens.h"
#include "../../include/tokens/tokenizer.h"
#include "../../include/lexer/lexer.h"

Token *lexer(Context *context, FILE *fp, int *tokenCount)
{

    fillEntrys(context->tokenTable);
    Token *tokens = tokenizeFile(fp, tokenCount);
    for (int i = 0; i < *tokenCount; i++)
    {
        int TOKEN_INT_CODE = getTokenIntCodeValue(context->tokenTable, tokens[i].lexeme);
        tokens[i].value = (TokenValue)TOKEN_INT_CODE;
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

// Token *getCurrentToken(FILE *fp)
// {
//     // Save the current file pointer position
//     long original_position = ftell(fp);
//     if (original_position == -1)
//     {
//         // Error in getting position
//         return NULL;
//     }

//     // Call getNextToken which will modify the file pointer
//     Token *t = getNextToken(fp);

//     // Reset the file pointer to the original position
//     if (fseek(fp, original_position, SEEK_SET) != 0)
//     {
//         // Error in resetting position
//         return NULL;
//     }

//     return t;
// }
// void consume(FILE *fp)
// {
//     return;
// }
// int match(FILE *fp, TokenValue t)
// {
//     return;
// }
// int expect(FILE *fp, TokenValue t)
// {
//     return;
// }