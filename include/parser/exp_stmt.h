#ifndef EXPRESIION_STATEMENT_PARSER
#define EXPRESIION_STATEMENT_PARSER
#include "../../include/context/context.h"

ASTNode *parseExpression(Context *context);
ASTNode *parseAssignmentExpression(Context *context);
ASTNode *parseAdditiveExpression(Context *context);
ASTNode *parseMultiplicativeExpression(Context *context);
ASTNode *parsePrimaryExpression(Context *context);
ASTNode *parseExpressionStatement(Context *context);
ASTNode *parseConditionalExpression(Context *context);

#endif