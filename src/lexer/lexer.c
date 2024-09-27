#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/tokens/tokens.h"
#include "../../include/tokens/tokenizer.h"
#include "../../include/lexer/lexer.h"

#define MAX_TOKEN_LENGTH 500

#define INITIAL_CAPACITY 200

typedef struct
{
    regex_t regex;
    const char *name;
    const char *pattern;
} TokenPattern;

TokenPattern token_patterns[] = {
    {{0}, "KEYWORD", "\\b(var|if|else|while|return|function|asm|char|int|float|string|continue|break|typeof)\\b"}, // KEYWORDS
    {{0}, "IDENTIFIER", "[a-zA-Z_][a-zA-Z0-9_]*"},                                                                 // IDENTIFIER
    {{0}, "SINGLE_LINE_COMMENT", "\\/\\/[^\\n]*"},                                                                 // Single-line comments
    // {{0}, "MULTI_LINE_COMMENT", "/\\*([^*]|\\*+[^/*])*\\*+/"}  ,                  // Multi-line comments                                           // NUMBER
    {{0}, "INVALID_IDENTIFIER", "[0-9]+\\.[a-zA-Z]+"},
    {{0}, "HEX", "0[x][0-9a-fA-F]+"},
    {{0}, "INCREMENT_OPERATOR", "\\+\\+|--"},                                // INC/DEC
    {{0}, "FLOAT", "[+-]?([0-9]+\\.[0-9]*|\\.[0-9]+)([eE][+-]?[0-9]+)\\b)"}, // FLOAT
    {{0}, "INTEGER", "[+-]?[0-9]+"},

    {{0}, "CONDITIONAL_OPERATOR", "==|!=|<=|>=|<|>"},
    {{0}, "OPERATOR", "[%&|!@+*/=-]"},                     // OPERATOR                                                       // CONDITIONAL OPERATORS
    {{0}, "PUNCTUATION", "\\(|\\)|\\{|\\}|\\[|\\]|:|;|,"}, // PUNCTUATION
    {{0}, "STRING_CONSTANT", "\"[^\"]*\"|'[^']*'"},        // STRING CONSTANT

};

#define NUM_PATTERNS (sizeof(token_patterns) / sizeof(token_patterns[0]))

int compile_token_patterns()
{

    for (int i = 0; i < NUM_PATTERNS; i++)
    {
        int result = regcomp(&token_patterns[i].regex, token_patterns[i].pattern, REG_EXTENDED);
        if (result)
        {
            char error_buf[100];
            regerror(result, &token_patterns[i].regex, error_buf, sizeof(error_buf));
            fprintf(stderr, "Could not compile regex for %s: %s\n", token_patterns[i].name, error_buf);
            exit(1);
        }
    }
    return 0;
}

Token *getNextToken(Context *context, int unconsume)
{
    static char buffer[MAX_TOKEN_LENGTH] = "";
    static TokenizerState state = {0};
    state.file_pointer = ftell(context->ip);

    if (unconsume)
    {

        TokenizerState s = popTokenizerState(context->tokenizerStateStack);
        state = s;
        state.buffer_pos = 0;
        state.buffer_len = 0;
        fseek(context->ip, s.file_pointer, SEEK_SET);
        return;
    }

    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Fill buffer if empty
    if (state.buffer_pos >= state.buffer_len)
    {

        state.buffer_pos = 0;
        if (fgets(buffer, MAX_TOKEN_LENGTH, context->ip) == NULL)
        {
            free(token);
            return NULL; // Return NULL at EOF
        }
        state.buffer_len = strlen(buffer);
        state.current_row++;
        state.current_col++;
    }
    // printf("\n%s\n", buffer);

    while (state.buffer_pos < state.buffer_len)
    {

        int matched = 0;
        for (int i = 0; i < NUM_PATTERNS; i++)
        {

            regmatch_t match;

            if (regexec(&token_patterns[i].regex, &buffer[state.buffer_pos], 1, &match, 0) == 0 && match.rm_so == 0)
            {

                int token_length = match.rm_eo;
                state.file_pointer = ftell(context->ip) - state.buffer_len + state.buffer_pos;
                pushTokenizerState(context->tokenizerStateStack, state);

                if (strcmp(token_patterns[i].name, "SINGLE_LINE_COMMENT") == 0
                    //  || 1
                    // strcmp(token_patterns[i].name, "MULTI_LINE_COMMENT") == 0)
                )
                {
                    // Move the buffer position past the comment
                    state.buffer_pos += token_length;
                    state.current_col += token_length;

                    // For single-line comments, move to the next line
                    if (strcmp(token_patterns[i].name, "SINGLE_LINE_COMMENT") == 0)
                    {
                        state.buffer_pos = state.buffer_len; // Force reading a new line
                    }
                    continue; // Break out of the for loop and continue
                }
                // Check if the character following the FLOAT is alphanumeric or an unders  ascore

                strncpy(token->lexeme, &buffer[state.buffer_pos], token_length);
                token->lexeme[token_length] = '\0';
                state.buffer_pos += token_length;
                token->pos.col = state.current_col + match.rm_so;
                token->pos.line = state.current_row;
                matched = 1;
                state.current_col += token_length;

                token->value = getTokenIntCodeValue(context->tokenTable, token->lexeme);

                return token;
            }
        }

        if (!matched)
        {
            state.buffer_pos++;
            state.current_col++;
        }

        // If the end of the buffer is reached and no token is matched, read the next line
        if (state.buffer_pos >= state.buffer_len)
        {
            state.buffer_pos = 0;
            if (fgets(buffer, MAX_TOKEN_LENGTH, context->ip) == NULL)
            {
                token->value = TEOF;
                return token; // Return NULL at EOF
            }
            state.buffer_len = strlen(buffer);
            state.current_row++;
            state.current_col = 0;
        }
    }

    free(token);
    return NULL; // Return NULL if no match is found
}

Token getCurrentToken(Context *context)
{
    if (context->current.value == START)
    {
        Token *token = getNextToken(context, 0);
        context->current = *token;
        free(token);
       
        return context->current;
    }
    return context->current;
}
void consume(Context *context)
{
    Token *token = getNextToken(context, 0);
    context->current = *token;
    free(token);
    return;
}
void unconsume(Context *context)
{
    getNextToken(context, 1);
    getNextToken(context, 1);
    context->current.value = START;
    getCurrentToken(context);
    return;
}
