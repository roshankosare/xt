#ifndef LEXER_H
#define LEXER_H
#include "../tokens/tokens.h"
#include "../tokens/tokens_fill.h"
#include "../tokens/tokens_val.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations
Token *tokenize(FILE *fp, int *tokenCount);
void printTokens(Token *tokens, int tokenCount);



#endif
/* LEXER_H */
