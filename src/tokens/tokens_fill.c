#include "../../include/tokens/tokens_fill.h"
#include "../../include/tokens/tokens.h"

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
    case INT_TOKEN_INC:
        token->value = INC;
        break;
    case INT_TOKEN_DEC:
        token->value = DEC;
        break;

    default:
        break;
    }
}