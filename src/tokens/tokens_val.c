#include "../../includes/tokens/tokens_val.h"
#include <string.h>
#include <ctype.h>

int isOperator(char ch)
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

int ispunctChar(char ch)
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

int isIdentifierChar(char ch)
{
    return isalnum(ch) || ch == '_';
}

int isOperatorToken(char *token)
{
    // clang-format off
   if(strcmp(token,"+") == 0) return 1;
   if(strcmp(token,"-") == 0) return 1;
   if(strcmp(token,"*") == 0) return 1;
   if(strcmp(token,"/") == 0) return 1;
   if(strcmp(token,"++") == 0) return 1;
   if(strcmp(token,"--") == 0) return 1;
   if(strcmp(token,"<") == 0) return 1;
   if(strcmp(token,">") == 0) return 1;
   if(strcmp(token,"<=") == 0) return 1;
   if(strcmp(token,">=") == 0) return 1;
   if(strcmp(token,"==") == 0) return 1;
   if(strcmp(token,"=") == 0) return 1;
    return 0;

    // clang-format on
}

int isConditionalOperatorToken(char *token)
{
    // clang-format off
    if(strcmp(token,"<") == 0) return 1;
    if(strcmp(token,">") == 0) return 1;
    if(strcmp(token,"<=") == 0) return 1;
    if(strcmp(token,">=") == 0) return 1;
    if(strcmp(token,"==") == 0) return 1;
    return 0;
    // clang-format on
}

int isKeywordToken(char *token)
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
int isPunctuationToken(char *token)
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
int isIdentifierToken(char *token)
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

int isConstantToken(char *token)
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