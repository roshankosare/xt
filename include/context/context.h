#ifndef CONTEXT
#define CONTEXT
#include "../symboltable/functiontable.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"
#include "../parser/ast.h"

typedef struct Context
{
    SymbolTableStack *symbolTableStack;
    FunctionTable *functionTable;
    SymbolTable * symbolTableQueue;
    SymbolTableStack *symbolTableTempStack;
    Token *tokens;
    Token current;
    Token lookahed;
    ASTQueue *astQueue;
    ASTStack *astStack;
    int index;

} Context;

Context *initContext(Token *tokens);

#endif