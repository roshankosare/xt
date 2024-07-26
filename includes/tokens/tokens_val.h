#ifndef TOKENS_VAL
#define TOKENS_VAL

// characters checking funtions
int isOperator(char ch);
int ispunctChar(char ch);
int isIdentifierChar(char ch);

// tokens checking funtion
int isOperatorToken(char *token);
int isKeywordToken(char *token);
int isPunctuationToken(char *token);
int isIdentifierToken(char *token);
int isConstantToken(char *token);
int isConditionalOperatorToken(char *token);



#endif