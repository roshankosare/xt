#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
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
    {{0}, "KEYWORD", "\\b(var|if|else|while|return|function|asm|char|int|float|string|continue|break)\\b"}, // KEYWORDS
    {{0}, "IDENTIFIER", "[a-zA-Z_][a-zA-Z0-9_]*"},                                                          // IDENTIFIER
    {{0}, "SINGLE_LINE_COMMENT", "\\/\\/[^\\n]*"},                                                          // Single-line comments
    // {{0}, "MULTI_LINE_COMMENT", "/\\*([^*]|\\*+[^/*])*\\*+/"}  ,                  // Multi-line comments                                           // NUMBER
    {{0}, "INVALID_IDENTIFIER", "[0-9]+\\.[a-zA-Z]+"},
    {{0}, "HEX", "0[x][0-9a-fA-F]+"},
    {{0}, "INCREMENT_OPERATOR", "\\+\\+|--"},                                // INC/DEC
    {{0}, "FLOAT", "[+-]?([0-9]+\\.[0-9]*|\\.[0-9]+)([eE][+-]?[0-9]+)\\b)"}, // FLOAT
    {{0}, "INTEGER", "[+-]?[0-9]+"},

    {{0}, "CONDITIONAL_OPERATOR", "==|!=|<=|>=|<|>"},
    {{0}, "OPERATOR", "[&|!@+*/=-]"},                      // OPERATOR                                                       // CONDITIONAL OPERATORS
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
    case INT_TOKEN_ASM:
        token->value = ASM;
        break;
    case INT_TOKEN_CHAR:
        token->value = CHAR_T;
        break;
    case INT_TOKEN_INT:
        token->value = INT_T;
        break;
    case INT_TOKEN_FLOAT:
        token->value = FLOAT_T;
        break;
    case INT_TOKEN_STRING:
        token->value = STRING_T;
        break;
    case INT_TOKEN_VALUE_AT:
        token->value = VALUE_AT;
        break;
    case INT_TOKEN_BIT_AND:
        token->value = BIT_AND;
        break;
    case INT_TOKEN_BIT_OR:
        token->value = BIT_OR;
        break;
    case INT_TOKEN_BIT_NOT:
        token->value = BIT_NOT;
        break;

    case INT_TOKEN_HEX_CONSTANT:
        token->value = HEX_CONSTANT;
        break;
    case INT_TOKEN_CONTINUE:
        token->value = CONTINUE;
        break;

    case INT_TOKEN_BREAK:
        token->value = BREAK;
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
    if(strcmp("asm",token) == 0)        return INT_TOKEN_ASM;
    if(strcmp("int",token)== 0)         return INT_TOKEN_INT;
    if(strcmp("char",token) == 0)       return INT_TOKEN_CHAR;
    if(strcmp("float",token) == 0)      return INT_TOKEN_FLOAT;
    if(strcmp("string",token)== 0)      return INT_TOKEN_STRING;
    if(strcmp("continue",token) == 0)   return INT_TOKEN_CONTINUE;
    if(strcmp("break",token) == 0)      return INT_TOKEN_BREAK;
    
    if (strcmp(token, "+") == 0)        return INT_TOKEN_PLUS;
    if (strcmp(token, "-") == 0)        return INT_TOKEN_MINUS;
    if (strcmp(token, "*") == 0)        return INT_TOKEN_MUL;
    if (strcmp(token, "/") == 0)        return INT_TOKEN_DIV;
    if (strcmp(token,"@") == 0)          return INT_TOKEN_VALUE_AT;
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
    if (strcmp(token,"++") == 0)         return INT_TOKEN_INC;
    if (strcmp(token,"--")== 0)          return INT_TOKEN_DEC;
    if(strcmp(token,"&") == 0)           return INT_TOKEN_BIT_AND;
    if(strcmp(token,"|") == 0)           return INT_TOKEN_BIT_OR;
    if(strcmp(token,"!") == 0)           return INT_TOKEN_BIT_NOT;  
    if(isHexNumber(token))               return INT_TOKEN_HEX_CONSTANT; 
    if (isIdentifierToken(token))        return INT_TOKEN_IDENTIFIER;
    if (isIntegerConstant(token))        return INT_TOKEN_INTEGER_CONSTANT;  
    if (isFloatConstant(token))          return INT_TOKEN_FLOAT_CONSTANT; 
    if (isStringConstant(token))         return INT_TOKEN_STRING_CONSTANT;
    
    
    
    return INT_TOKEN_UNKNOWN;
    // clang-format onSS
}
