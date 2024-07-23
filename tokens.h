#ifndef TOKENS_H
#define TOKENS_H

// clang-format off
#define INT_TOKEN_IF                    0
#define INT_TOKEN_ELSE                  1
#define INT_TOKEN_RETURN                2
#define INT_TOKEN_WHILE                 3
#define INT_TOKEN_PLUS                  4
#define INT_TOKEN_MINUS                 5
#define INT_TOKEN_MUL                   6
#define INT_TOKEN_DIV                   7
#define INT_TOKEN_ASSIGN                8
#define INT_TOKEN_EQUALS                9
#define INT_TOKEN_OPEN_PAREN            10
#define INT_TOKEN_CLOSE_PAREN           11
#define INT_TOKEN_OPEN_CURLY_PAREN      12
#define INT_TOKEN_CLOSE_CURLY_PAREN     13
#define INT_TOKEN_DOUBLE_QT             14
#define INT_TOKEN_SINGLE_QT             15
#define INT_TOKEN_COLAN                 16
#define INT_TOKEN_SEMI_COLAN            17
#define INT_TOKEN_LESS_THAN             18
#define INT_TOKEN_GRATER_THAN           19
#define INT_TOKEN_LESS_THAN_EQTO        20
#define INT_TOKEN_GRATER_THAN_EQTO      21
#define INT_TOKEN_COMMA                 22
#define INT_TOKEN_KEYWORD               23
#define INT_TOKEN_IDENTIFIER            24
#define INT_TOKEN_CONSTANT              25
#define INT_TOKEN_UNKNOWN               26  
#define INT_TOKEN_VAR                   27
#define INVALID_TOKEN                   -1

// clang-format on
// Token types
// typedef enum
// {
//     TOKEN_KEYWORD,
//     TOKEN_IDENTIFIER,
//     TOKEN_PUNCTUATION,
//     TOKEN_CONSTANT,
//     TOKEN_OPERATOR,
//     TOKEN_UNKNOWN,

// } TokenType;

typedef enum
{
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_CURLY_PAREN,
    CLOSE_CURLY_PAREN,
    PLUS,
    MUL,
    DIV,
    MINUS,
    ASSIGN,
    EQUALTO,
    VAR,
    IF,
    ELSE,
    FOR,
    RETURN,
    WHILE,
    DOUBLE_QT,
    SINGLE_QT,
    COLAN,
    COMMA,
    SEMI_COLAN,
    LESS_THAN,
    GRATER_THAN,
    LESS_THAN_EQTO,
    GRATER_THAN_EQTO,
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    UNKNOWN,
    TEOF
    
} TokenValue;

typedef struct
{
    int line;
    int col;
} Position;

// Token structure
typedef struct
{
    TokenValue value;
    char lexeme[100]; // Assuming maximum token length
    Position pos;

} Token;

#endif
