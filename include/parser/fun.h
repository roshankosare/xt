#ifndef PARSE_FUN_DEF
#define PARSE_FUN_DEF
#include "../../include/context/context.h"
#include "ast.h"
ASTNode *parseFunctionDefinitionOrCall(Context *context);
ASTNode *parseFunCallParameterList(Context *context);
ASTNode *parseFunDefParameterList(Context *context);
ASTNode *parseFunCall(Context *context);

#endif