#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdio.h>
#include "tokens.h"


#define MAX_TOKEN_STATE 10
typedef struct TokenizerState
{
    int buffer_pos;
    int buffer_len;
    int current_row;
    int current_col;
    long file_pointer;
    int isEmpty;
} TokenizerState;

typedef struct TokenizerStateStack
{
    TokenizerState data[MAX_TOKEN_STATE];
    int top;
} TokenizerStateStack;


int isIdentifierChar(char ch);
int getTokenIntCodeValue(TokenTable *table, char *token);
char *getTokenStringValue(int token);
int isIdentifierToken(char *token);
int isHexNumber(char *token);

TokenizerStateStack *initTokenizerStateStack();
void pushTokenizerState(TokenizerStateStack *stack, TokenizerState state);
TokenizerState popTokenizerState(TokenizerStateStack *stack);

#endif