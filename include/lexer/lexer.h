#ifndef LEXER_H
#define LEXER_H
#include "../tokens/tokens.h"
#include "../context/context.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations
Token *lexer(Context *context, FILE *fp, int *tokenCount);
void printTokens(Token *tokens, int tokenCount);

// Token *getCurrentToken(FILE *fp);
// void consume(FILE *fp);
// int match(FILE *fp,TokenValue t);
// int expect(FILE *fp,TokenValue t);

#endif