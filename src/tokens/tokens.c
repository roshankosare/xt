
#include "../../includes/tokens/tokens.h"
#include <string.h>

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

 int getTokenIntCodeValue(char *token)
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
