#ifndef LEXER_H
#define LEXER_H
#include "../tokens/tokens.h"
#include "../context/context.h"
#include <stdio.h>
#include <stdlib.h>

// Function declarations

Token *getNextToken(Context *context,int unconsume);
Token getCurrentToken(Context *context);
void consume(Context *context);
void unconsume(Context *context);

// int match(FILE *fp,TokenValue t);
// int expect(FILE *fp,TokenValue t);

#endif