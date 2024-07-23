#ifndef LEX_HELPER
#define LEX_HELPER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokens.h"

static int isOperator(char ch);
;
static int getTokenIntCodeValue(char *token);
char *getTokenStringValue(int token);
static int isIdentifierChar(char ch);
static int ispunctChar(char ch);
void fillKeywordTokenValue(Token *token);
void fillPunctuationTokenValue(Token *token);
void fillOperatorTokenValue(Token *token);

static int isOperatorToken(char *token);
static int isKeywordToken(char *token);
static int isPunctuationToken(char *token);
static int isIdentifierToken(char *token);
static int isConstantToken(char *token);

static int isOperator(char ch)
{
    switch (ch)
    {
    case '+':
        return 1;
        break;
    case '*':
        return 1;
        break;
    case '-':
        return 1;
        break;
    case '/':
        return 1;
        break;
    case '=':
        return 1;
        break;
    case '<':
        return 1;
        break;
    case '>':
        return 1;
        break;

    default:
        return 0;
        break;
    }
}
static int ispunctChar(char ch)
{
    switch (ch)
    {
    case ')':
        return 1;
        break;
    case '(':
        return 1;
        break;
    case '}':
        return 1;
        break;
    case '{':
        return 1;
        break;
    case ';':
        return 1;
        break;
    case ':':
        return 1;
        break;
    case ',':
        return 1;
        break;

    default:
        return 0;
        break;
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
    case CONSTANT:
        return "CONSTANT";
    case UNKNOWN:
        return "UNKNOWN";

    default:
        return "UNKNOWN";
    }
}

static int getTokenIntCodeValue(char *token)
{
    // clang-format off
    if(strcmp(token,"var") == 0)        return INT_TOKEN_VAR; 
    if (strcmp(token, "if") == 0)       return INT_TOKEN_IF;
    if (strcmp(token, "else") == 0)     return INT_TOKEN_ELSE;
    if (strcmp(token, "return") == 0)   return INT_TOKEN_RETURN;
    if (strcmp(token, "while") == 0)    return INT_TOKEN_WHILE;
    if (strcmp(token, "+") == 0)        return INT_TOKEN_PLUS;
    if (strcmp(token, "-") == 0)        return INT_TOKEN_MINUS;
    if (strcmp(token, "*") == 0)        return INT_TOKEN_MUL;
    if (strcmp(token, "/") == 0)        return INT_TOKEN_DIV;
    if (strcmp(token, "=") == 0)        return INT_TOKEN_ASSIGN;
    if (strcmp(token, "==") == 0)       return INT_TOKEN_EQUALS;
    if (strcmp(token, "(") == 0)        return INT_TOKEN_OPEN_PAREN;
    if (strcmp(token, ")") == 0)        return INT_TOKEN_CLOSE_PAREN;
    if (strcmp(token, "{") == 0)        return INT_TOKEN_OPEN_CURLY_PAREN;
    if (strcmp(token, "}") == 0)        return INT_TOKEN_CLOSE_CURLY_PAREN;
    if (strcmp(token, "\"") == 0)       return INT_TOKEN_DOUBLE_QT;
    if (strcmp(token, "\'") == 0)       return INT_TOKEN_SINGLE_QT;
    if (strcmp(token, ":") == 0)        return INT_TOKEN_COLAN;
    if (strcmp(token, ";") == 0)        return INT_TOKEN_SEMI_COLAN;
    if (strcmp(token, "<") == 0)        return INT_TOKEN_LESS_THAN;
    if (strcmp(token, ">") == 0)        return INT_TOKEN_GRATER_THAN;
    if (strcmp(token, "<=") == 0)       return INT_TOKEN_LESS_THAN_EQTO;
    if (strcmp(token, ">=") == 0)       return INT_TOKEN_GRATER_THAN_EQTO;
    if (strcmp(token, ",") == 0)        return INT_TOKEN_COMMA;
    if(isKeywordToken(token))           return INT_TOKEN_KEYWORD;
    if(isIdentifierToken(token))        return INT_TOKEN_IDENTIFIER;   
    if(isConstantToken(token))          return INT_TOKEN_CONSTANT;
    return INT_TOKEN_UNKNOWN;

    

    return -1;
    // clang-format on
}

// Function to check if a character is a valid identifier character (alphanumeric or underscore)
static int isIdentifierChar(char ch)
{
    return isalnum(ch) || ch == '_';
}

void fillKeywordTokenValue(Token *token)
{
    switch (getTokenIntCodeValue(token->lexeme))
    {
    case INT_TOKEN_VAR:
        token->value = VAR;
        break;
    case INT_TOKEN_IF:
        token->value = IF;
        break;
    case INT_TOKEN_ELSE:
        token->value = ELSE;
        break;
    case INT_TOKEN_RETURN:
        token->value = RETURN;
        break;
    case INT_TOKEN_WHILE:
        token->value = WHILE;
        break;

    default:
        break;
    }
}

void fillPunctuationTokenValue(Token *token)
{
    switch (getTokenIntCodeValue(token->lexeme))
    {
    case INT_TOKEN_OPEN_CURLY_PAREN:
        token->value = OPEN_CURLY_PAREN;
        break;
    case INT_TOKEN_CLOSE_CURLY_PAREN:
        token->value = CLOSE_CURLY_PAREN;
        break;
    case INT_TOKEN_OPEN_PAREN:
        token->value = OPEN_PAREN;
        break;
    case INT_TOKEN_CLOSE_PAREN:
        token->value = CLOSE_PAREN;
        break;
    case INT_TOKEN_SEMI_COLAN:
        token->value = SEMI_COLAN;
        break;
    case INT_TOKEN_COLAN:
        token->value = COLAN;
        break;
    case INT_TOKEN_SINGLE_QT:
        token->value = SINGLE_QT;
        break;
    case INT_TOKEN_DOUBLE_QT:
        token->value = DOUBLE_QT;
        break;
    case INT_TOKEN_COMMA:
        token->value = COMMA;
        break;

    default:
        break;
    }
}

void fillOperatorTokenValue(Token *token)
{
    switch (getTokenIntCodeValue(token->lexeme))
    {
    case INT_TOKEN_PLUS:
        token->value = PLUS;
        break;
    case INT_TOKEN_MINUS:
        token->value = MINUS;
        break;
    case INT_TOKEN_MUL:
        token->value = MUL;
        break;
    case INT_TOKEN_DIV:
        token->value = DIV;
        break;
    case INT_TOKEN_ASSIGN:
        token->value = ASSIGN;
        break;
    case INT_TOKEN_EQUALS:
        token->value = EQUALTO;
        break;
    case INT_TOKEN_LESS_THAN:
        token->value = LESS_THAN;
        break;
    case INT_TOKEN_GRATER_THAN:
        token->value = GRATER_THAN;
        break;
    case INT_TOKEN_LESS_THAN_EQTO:
        token->value = LESS_THAN_EQTO;
        break;
    case INT_TOKEN_GRATER_THAN_EQTO:
        token->value = GRATER_THAN_EQTO;
        break;

    default:
        break;
    }
}

static int isOperatorToken(char *token)
{
    while (*token != '\0')
    {
        if (!isOperator(*token))
        {
            return 0;
        }
        token++;
    }
    return 1;
}
static int isKeywordToken(char *token)
{
    // clang-format off
    if(strcmp("var",token) == 0)         return 1;
    if(strcmp("if",token) == 0)          return 1;
    if(strcmp("else",token) == 0)        return 1;
    if(strcmp("while",token) == 0)       return 1;
    if(strcmp("return",token) == 0)      return 1;
    if(strcmp("for",token) == 0)         return 1;

    return 0;
    // clang-format on
}
static int isPunctuationToken(char *token)
{

    if (!ispunctChar(*token))
    {
        return 0;
    }

    while (*token != '\0')
    {
        if (isOperator(*token))
        {
            return 0;
        }
        token++;
    }
    return 1;
}
static int isIdentifierToken(char *token)
{

    if (!isalpha(*token) && *token != '_' && !ispunct(*token))
    {
        return 0;
    }

    while (*token != '\0')
    {
        if (!isIdentifierChar(*token))
        {
            return 0;
        }
        token++;
    }

    return 1;
}

static int isConstantToken(char *token)
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

        token = firstChar;

        return 0;
    }
    token = firstChar;
    if (isdigit(*firstChar))
    {
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
    return 0;
}

#endif
