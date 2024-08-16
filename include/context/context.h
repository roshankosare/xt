#ifndef CONTEXT
#define CONTEXT
#include "../symboltable/functiontable.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"

typedef struct Context
{
    SymbolTableStack *symbolTableStack;
    FunctionTable *functionTable;
    SymbolTable * symbolTableQueue;
    Token *tokens;
    Token current;
    Token lookahed;
    int offset;
    int index;

} Context;

Context *initContext(Token *tokens);

#endif