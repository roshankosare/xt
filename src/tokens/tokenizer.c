#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/tokens/tokenizer.h"
#include "../../include/tokens/tokens.h"

int isIdentifierChar(char ch)
{
    return isalnum(ch) || ch == '_';
}

int isIdentifierToken(char *token)
{

    if (!isalpha(*token) && *token != '_' && !ispunct(*token))
    {
        return 0;
    }

    while (*token != '\0')
    {
        if (!isalnum(*token) && *token != '_')
        {
            return 0;
        }
        token++;
    }

    return 1;
}

int isStringConstant(char *token)
{
    char *firstChar = token;

    if (*firstChar == '\"' || *firstChar == '\'')
    {
        while (*token != '\0')
        {
            token++;
        }
        token--;
        if (*firstChar == '\"' && *token == '\"')
            return 1;
        if (*firstChar == '\'' && *token == '\'')
            return 1;

        return 0;
    }

    return 0;
}

int isIntegerConstant(char *token)
{
    if (!isdigit(*token) && *token != '+' && *token != '-')
    {
        return 0;
    }
    token++;

    while (*token != '\0')
    {
        if (!isdigit(*token))
        {
            return 0;
        }
        token++;
    }
    return 1;
}

int isFloatConstant(char *token)
{
    // Check if the first character is a digit or a sign
    if (!isdigit(*token) && *token != '+' && *token != '-')
    {
        return 0;
    }

    // Move to the next character
    token++;

    int dot = 0;
    while (*token != '\0')
    {
        if (!isdigit(*token))
        {
            if (*token == '.' && dot == 0)
            {
                dot = 1;
            }
            else
            {
                return 0;
            }
        }
        token++;
    }
    return 1;
}

int isHexNumber(char *token)
{

    if (*token != '0' && *(token + 1) != 'x')
    {
        return 0;
    }
    token = token + 2;
    if (*token == '\0')
    {
        return 0;
    }
    while (*token != '\0')
    {
        if (!isxdigit(*token))
        {
            return 0;
        }
        return 1;
    }
}

char *getTokenStringValue(int token)
{

    switch (token)
    {
    case OPEN_PAREN:
        return "OPEN_PAREN";
    case CLOSE_PAREN:
        return "CLOSE_PAREN";
    case OPEN_CURLY_PAREN:
        return "OPEN_CURLY_PAREN";
    case CLOSE_CURLY_PAREN:
        return "CLOSE_CURLY_PAREN";
    case PLUS:
        return "PLUS";
    case MUL:
        return "MUL";
    case DIV:
        return "DIV";
    case MINUS:
        return "MINUS";
    case ASSIGN:
        return "ASSIGN";
    case EQUALTO:
        return "EQUALTO";
    case VAR:
        return "VAR";
    case BLOCK_VAR:
        return "BLOCK_VAR";
    case FUNCTION:
        return "FUNCTION";
    case IF:
        return "IF";
    case ELSE:
        return "ELSE";
    case FOR:
        return "FOR";
    case RETURN:
        return "RETURN";
    case WHILE:
        return "WHILE";
    case DOUBLE_QT:
        return "DOUBLE_QT";
    case SINGLE_QT:
        return "SINGLE_QT";
    case COLAN:
        return "COLAN";
    case COMMA:
        return "COMMA";
    case SEMI_COLAN:
        return "SEMI_COLAN";
    case LESS_THAN:
        return "LESS_THAN";
    case GRATER_THAN:
        return "GRATER_THAN";
    case LESS_THAN_EQTO:
        return "LESS_THAN_EQTO";
    case GRATER_THAN_EQTO:
        return "GRATER_THAN_EQTO";
    case KEYWORD:
        return "KEYWORD";
    case IDENTIFIER:
        return "IDENTIFIER";
    case UNKNOWN:
        return "UNKNOWN";
    case LOGICAL_AND:
        return "LOGICAL_AND";
    case LOGICAL_OR:
        return "LOGICAL_OR";
    case LOGICAL_NOT:
        return "LOGICAL_NOT";
    case NOT_EQLTO:
        return "NOT_EQLTO";
    case ARGS_START:
        return "ARGS_START";
    case ARGS_END:
        return "ARGS_END";
    case PARAM_START:
        return "PARAM_START";
    case PARAM_END:
        return "PARAM_END";
    case INC:
        return "INC";
    case DEC:
        return "DEC";

    case OPEN_SQ_PARAN:
        return "OPEN_SQ_PARAN";
    case CLOSE_SQ_PARAN:
        return "CLOSE_SQ_PARAN";
    case INTEGER_CONSTANT:
        return "INTEGER_CONSTANT";
    case FLOAT_CONSTANT:
        return "FLOAT_CONSTANT";
    case STRING_CONSTANT:
        return "STRING_CONSTANT";
    case ASM:
        return "ASM";
    case TEOF:
        return "EOF";
    case CHAR_T:
        return "CHAR";
    case FLOAT_T:
        return "FLOAT";
    case INT_T:
        return "INT";
    case STRING_T:
        return "STRING";
    case VALUE_AT:
        return "VALUE_AT";
    case BIT_AND:
        return "BIT_AND";
    case BIT_OR:
        return "BIT_OR";
    case BIT_NOT:
        return "BIT_NOT";
    case HEX_CONSTANT:
        return "HEX_CONSTANT";
    case CONTINUE:
        return "CONTINUE";
    case BREAK:
        return "BREAK";
    case TYPEOF:
        return "TYPEOF";
    case MODULAS:
        return "MODULAS";

    default:
        return "UNKNOWN";
    }
}

int getTokenIntCodeValue(TokenTable *table, char *token)
{
    int value = 0;
    TokenEntry *entry = lookUpTokenEntry(table, token);
    if (entry)
    {
        value = entry->value;
        free(entry);
        return value;
    }
    if (isHexNumber(token))
        return HEX_CONSTANT;
    if (isIdentifierToken(token))
        return IDENTIFIER;
    if (isIntegerConstant(token))
        return INTEGER_CONSTANT;
    if (isFloatConstant(token))
        return FLOAT_CONSTANT;
    if (isStringConstant(token))
        return STRING_CONSTANT;

    return UNKNOWN;
    // clang-format on
}

TokenizerStateStack *initTokenizerStateStack()
{
    TokenizerStateStack *stack = (TokenizerStateStack *)malloc(sizeof(TokenizerStateStack));
    for (int i = 0; i < MAX_TOKEN_STATE; i++)
    {
        stack[i].data->isEmpty = 1;
    }
    return stack;
}
void pushTokenizerState(TokenizerStateStack *stack, TokenizerState state)
{

    (stack->top)++;
    if (stack->top >= MAX_TOKEN_STATE)
    {
        stack->top = 0;
    }
    stack->data[stack->top] = state;
    return;
}
TokenizerState popTokenizerState(TokenizerStateStack *stack)
{
    if (stack->top < 0)
    {
        stack->top = MAX_TOKEN_STATE - 1;
        if (stack->data[stack->top].isEmpty == 1)
        {
            printf("\nERROR: tokenizer state stack underflow\n");
            exit(1);
        }
    }

    TokenizerState state = stack->data[stack->top];
    (stack->top)--;
    return state;
}
