#ifndef LEXER_H
#define LEXER_H
#include "../tokens/tokens.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations
Token *lexer(FILE *fp, int *tokenCount);
void printTokens(Token *tokens, int tokenCount);



#endif
/* LEXER_H */
