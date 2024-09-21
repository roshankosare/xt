#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/tokens/tokenizer.h"
#include "../../include/tokens/tokens.h"

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

Token *getNextToken(FILE *file)
{
    static char buffer[MAX_TOKEN_LENGTH] = "";
    static int buffer_pos = 0;
    static int buffer_len = 0;
    static int current_row = 0;
    static int current_col = 0;

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
        current_row++;
        current_col++;
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
                if (strcmp(token_patterns[i].name, "SINGLE_LINE_COMMENT") == 0
                    //  || 1
                    // strcmp(token_patterns[i].name, "MULTI_LINE_COMMENT") == 0)
                )
                {
                    // Move the buffer position past the comment
                    buffer_pos += token_length;
                    current_col += token_length;

                    // For single-line comments, move to the next line
                    if (strcmp(token_patterns[i].name, "SINGLE_LINE_COMMENT") == 0)
                    {
                        buffer_pos = buffer_len; // Force reading a new line
                    }
                    continue; // Break out of the for loop and continue
                }
                // Check if the character following the FLOAT is alphanumeric or an unders  ascore

                strncpy(token->lexeme, &buffer[buffer_pos], token_length);
                token->lexeme[token_length] = '\0';
                buffer_pos += token_length;
                token->pos.col = current_col + match.rm_so;
                token->pos.line = current_row;
                matched = 1;
                current_col += token_length;
                return token;
            }
        }

        if (!matched)
        {
            buffer_pos++;
            current_col++;
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
            current_row++;
            current_col = 0;
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
    TokenEntry *entry = lookUpTokenEntry(table, token);
    if (entry)
    {
        return entry->value;
    }
    if(isHexNumber(token))               return HEX_CONSTANT; 
    if (isIdentifierToken(token))        return IDENTIFIER;
    if (isIntegerConstant(token))        return INTEGER_CONSTANT;  
    if (isFloatConstant(token))          return FLOAT_CONSTANT; 
    if (isStringConstant(token))         return STRING_CONSTANT;
    
    return UNKNOWN;
    // clang-format on
}
