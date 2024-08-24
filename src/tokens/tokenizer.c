#include <stdio.h>
#include <regex.h>
#include <string.h>
#include "../../include/tokens/tokenizer.h"

#define MAX_TOKEN_LENGTH 100

#define INITIAL_CAPACITY 200

typedef struct
{
    regex_t regex;
    const char *name;
    const char *pattern;
} TokenPattern;

TokenPattern token_patterns[] = {
    {{0}, "INCREMENT_OPERATOR", "\\+\\+|--"},
    {{0}, "IDENTIFIER", "[a-zA-Z_][a-zA-Z0-9_]*"},
    {{0}, "FLOAT", "[+-]?[0-9]*\\.[0-9]+([eE][+-]?[0-9]+)?"},
    {{0}, "INTEGER", "[+-]?[0-9]+"},
    {{0}, "OPERATOR", "[+*/=-]"},                         // Properly escaped hyphen
    {{0}, "KEYWORD", "\\b(var|if|else|while|return)\\b"}, // Keywords with word boundaries
    {{0}, "CONDITIONAL_OPERATOR", "==|!=|<=|>=|<|>"},
    {{0}, "PUNCTUATION", "\\(|\\)|\\{|\\}|\\[|\\]|:|;|,"}, // Escaped brackets and parentheses
    {{0}, "STRING_CONSTANT", "\"[^\"]*\"|'[^']*'"}         // Handles double and single quotes

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
            return 1;
        }
    }
    return 0;
}

Token *getNextToken(FILE *file)
{
    static char buffer[MAX_TOKEN_LENGTH] = "";
    static int buffer_pos = 0;
    static int buffer_len = 0;

    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Fill buffer if empty
    if (buffer_pos >= buffer_len)
    {
        buffer_pos = 0;
        if (fgets(buffer, MAX_TOKEN_LENGTH, file) == NULL)
        {
            free(token);
            return NULL; // Return NULL at EOF
        }
        buffer_len = strlen(buffer);
    }

    while (buffer_pos < buffer_len)
    {
        int matched = 0;
        for (int i = 0; i < NUM_PATTERNS; i++)
        {
            regmatch_t match;
            if (regexec(&token_patterns[i].regex, &buffer[buffer_pos], 1, &match, 0) == 0 && match.rm_so == 0)
            {
                int token_length = match.rm_eo;
                strncpy(token->lexeme, &buffer[buffer_pos], token_length);
                token->lexeme[token_length] = '\0';
                buffer_pos += token_length;
                matched = 1;
                return token;
            }
        }

        if (!matched)
        {
            buffer_pos++;
        }

        // If the end of the buffer is reached and no token is matched, read the next line
        if (buffer_pos >= buffer_len)
        {
            buffer_pos = 0;
            if (fgets(buffer, MAX_TOKEN_LENGTH, file) == NULL)
            {
                free(token);
                return NULL; // Return NULL at EOF
            }
            buffer_len = strlen(buffer);
        }
    }

    free(token);
    return NULL; // Return NULL if no match is found
}

Token *tokenizeFile(FILE *fp, int *tokenCount)
{

    if (compile_token_patterns())
    {
        fprintf(stderr, "Failed to compile token patterns\n");
        return NULL;
    }

    // Initialize token storage
    int capacity = INITIAL_CAPACITY;
    Token *tokens = (Token *)malloc(capacity * sizeof(Token));
    if (tokens == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    Token *currentToken;
    int count = 0;

    while ((currentToken = getNextToken(fp)) != NULL)
    {
        // Expand the tokens array if needed
        if (count >= capacity)
        {
            capacity *= 2;
            tokens = (Token *)realloc(tokens, capacity * sizeof(Token));
            if (tokens == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                return NULL;
            }
        }

        // Add the token to the array
        tokens[count++] = *currentToken;
        free(currentToken); // Free the memory allocated for the token
    }

    tokens[count].value = TEOF;
    strcpy(tokens[count].lexeme, "EOF");
    *tokenCount = count;
    return tokens;
}

void free_token_patterns()
{
    for (int i = 0; i < NUM_PATTERNS; i++)
    {
        regfree(&token_patterns[i].regex);
    }
}

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
        if (!isalpha(*token))
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
    case TEOF:
        return "EOF";

    default:
        return "UNKNOWN";
    }
}

void fillTokenValue(Token *token)
{

    switch (getTokenIntCodeValue(token->lexeme))
    {
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
    case INT_TOKEN_OPEN_PAREN:
        token->value = OPEN_PAREN;
        break;
    case INT_TOKEN_CLOSE_PAREN:
        token->value = CLOSE_PAREN;
        break;
    case INT_TOKEN_OPEN_CURLY_PAREN:
        token->value = OPEN_CURLY_PAREN;
        break;
    case INT_TOKEN_CLOSE_CURLY_PAREN:
        token->value = CLOSE_CURLY_PAREN;
        break;
    case INT_TOKEN_DOUBLE_QT:
        token->value = DOUBLE_QT;
        break;
    case INT_TOKEN_SINGLE_QT:
        token->value = SINGLE_QT;
        break;
    case INT_TOKEN_COLAN:
        token->value = COLAN;
        break;
    case INT_TOKEN_SEMI_COLAN:
        token->value = SEMI_COLAN;
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
    case INT_TOKEN_COMMA:
        token->value = COMMA;
        break;
    case INT_TOKEN_KEYWORD:
        token->value = KEYWORD;
        break;
    case INT_TOKEN_IDENTIFIER:
        token->value = IDENTIFIER;
        break;
    case INT_TOKEN_INTEGER_CONSTANT:
        token->value = INTEGER_CONSTANT;
        break;
    case INT_TOKEN_FLOAT_CONSTANT:
        token->value = FLOAT_CONSTANT;
        break;
    case INT_TOKEN_STRING_CONSTANT:
        token->value = STRING_CONSTANT;
        break;
    case INT_TOKEN_UNKNOWN:
        token->value = UNKNOWN;
        break;
    case INT_TOKEN_VAR:
        token->value = VAR;
        break;
    case INT_TOKEN_BLOCK_VAR:
        token->value = BLOCK_VAR;
        break;
    case INT_TOKEN_INC:
        token->value = INC;
        break;
    case INT_TOKEN_DEC:
        token->value = DEC;
        break;
    case INT_TOKEN_FUNCTION:
        token->value = FUNCTION;
        break;
    case INT_TOKEN_LOGICAL_AND:
        token->value = LOGICAL_AND;
        break;
    case INT_TOKEN_LOGICAL_OR:
        token->value = LOGICAL_OR;
        break;
    case INT_TOKEN_LOGICAL_NOT:
        token->value = LOGICAL_NOT;
        break;
    case INT_TOKEN_NOT_EQLTO:
        token->value = NOT_EQLTO;
        break;
    case INT_TOKEN_OPEN_SQ_PARAN:
        token->value = OPEN_SQ_PARAN;
        break;
    case INT_TOKEN_CLOSE_SQ_PARAN:
        token->value = CLOSE_SQ_PARAN;
        break;
    default:
        token->value = UNKNOWN;
    }
}

int getTokenIntCodeValue(char *token)
{
    // clang-format off

    if(strcmp(token,"function") == 0)   return INT_TOKEN_FUNCTION;
    if(strcmp(token,"var") == 0)        return INT_TOKEN_VAR; 
    if (strcmp(token, "if") == 0)       return INT_TOKEN_IF;
    if (strcmp(token, "else") == 0)     return INT_TOKEN_ELSE;
    if (strcmp(token, "return") == 0)   return INT_TOKEN_RETURN;
    if (strcmp(token, "while") == 0)    return INT_TOKEN_WHILE;
    if(strcmp("and",token)== 0)         return INT_TOKEN_LOGICAL_AND;
    if(strcmp("or",token)== 0)          return INT_TOKEN_LOGICAL_OR;
    if(strcmp("not",token)==0 )         return INT_TOKEN_LOGICAL_NOT;
    
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
    if(strcmp(token,"[") == 0)          return INT_TOKEN_OPEN_SQ_PARAN;
    if(strcmp(token,"]") == 0)          return INT_TOKEN_CLOSE_SQ_PARAN;
    if (strcmp(token, "\"") == 0)       return INT_TOKEN_DOUBLE_QT;
    if (strcmp(token, "\'") == 0)       return INT_TOKEN_SINGLE_QT;
    if (strcmp(token, ":") == 0)        return INT_TOKEN_COLAN;
    if (strcmp(token, ";") == 0)        return INT_TOKEN_SEMI_COLAN;
    if (strcmp(token, "<") == 0)        return INT_TOKEN_LESS_THAN;
    if (strcmp(token, ">") == 0)        return INT_TOKEN_GRATER_THAN;
    if (strcmp(token, "<=") == 0)       return INT_TOKEN_LESS_THAN_EQTO;
    if (strcmp(token, ">=") == 0)       return INT_TOKEN_GRATER_THAN_EQTO;
    if (strcmp(token,"!=") == 0)        return INT_TOKEN_NOT_EQLTO;
    if (strcmp(token, ",") == 0)        return INT_TOKEN_COMMA;
    if(strcmp(token,"++") == 0)         return INT_TOKEN_INC;
    if(strcmp(token,"--")== 0)          return INT_TOKEN_DEC;
    if(isIdentifierToken(token))        return INT_TOKEN_IDENTIFIER;
    if(isIntegerConstant(token))        return INT_TOKEN_INTEGER_CONSTANT;  
    if(isFloatConstant(token))          return INT_TOKEN_FLOAT_CONSTANT; 
    if(isStringConstant(token))         return INT_TOKEN_STRING_CONSTANT;
    
    return INT_TOKEN_UNKNOWN;
    // clang-format onSS
}
