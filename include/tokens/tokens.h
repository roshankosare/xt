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
#define INT_TOKEN_BLOCK_VAR             28
#define INT_TOKEN_INC                   29
#define INT_TOKEN_DEC                   30
#define INT_TOKEN_FUNCTION              31
#define INT_TOKEN_LOGICAL_AND           32
#define INT_TOKEN_LOGICAL_OR            33
#define INT_TOKEN_LOGICAL_NOT           34
#define INT_TOKEN_NOT_EQLTO             35
#define INT_TOKEN_ARGS                  36
#define INT_TOKEN_OPEN_SQ_PARAN         37
#define INT_TOKEN_CLOSE_SQ_PARAN        38
#define INT_TOKEN_STRING_CONSTANT       39
#define INT_TOKEN_INTEGER_CONSTANT      40
#define INT_TOKEN_FLOAT_CONSTANT        41
#define INT_TOKEN_ASM                   42
#define INT_TOKEN_CHAR                  43
#define INT_TOKEN_INT                   44
#define INT_TOKEN_FLOAT                 45
#define INT_TOKEN_STRING                46
#define INT_TOKEN_VALUE_AT              47
#define INT_TOKEN_BIT_AND               48
#define INT_TOKEN_BIT_OR                49
#define INT_TOKEN_BIT_NOT               50
#define INT_TOKEN_HEX_CONSTANT          51
#define INVALID_TOKEN                   -1



typedef enum
{
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_CURLY_PAREN,
    CLOSE_CURLY_PAREN,
    OPEN_SQ_PARAN,
    CLOSE_SQ_PARAN,
    PLUS,
    MUL,
    DIV,
    MINUS,
    ASSIGN,
    EQUALTO,
    INC,
    DEC,
    VAR,
    BLOCK_VAR,
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
    NOT_EQLTO,
    KEYWORD,
    IDENTIFIER,
    FUNCTION,
    UNKNOWN,
    CONDITIONAL_TOKEN,
    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,
    BODYSTART,
    BODYEND,
    EXP_TOKEN,
    ARGS_START,
    ARGS_END,
    PARAM_START,
    PARAM_END,
    FUNCTION_CALL,
    STRING_CONSTANT ,
    INTEGER_CONSTANT ,
    FLOAT_CONSTANT  , 
    CHAR_T,
    INT_T,
    FLOAT_T,
    STRING_T,
    VALUE_AT,
    ASM,
    BIT_AND,
    BIT_OR,
    BIT_NOT,
    HEX_CONSTANT,
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
