#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdio.h>
#include "tokens.h"

Token *getNextToken(FILE *fp);
Token *tokenizeFile(FILE *fp, int *tokenCount);

int isIdentifierChar(char ch);
void fillTokenValue(Token *token);
int getTokenIntCodeValue(char *token);
char *getTokenStringValue(int token);
int isIdentifierToken(char *token);
int isHexNumber(char *token);


#endif