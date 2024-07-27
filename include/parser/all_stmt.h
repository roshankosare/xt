#ifndef ALL_STMT
#define ALL_STMT
#include "../../include/context/context.h"
#include "ast.h"

ASTNode *parseSelectionStatement(Context *context);
ASTNode *parseIterationStatement(Context *context);
ASTNode *parseBlockStatement(Context *context);
ASTNode *parseJumpStatement(Context *context);

#endif