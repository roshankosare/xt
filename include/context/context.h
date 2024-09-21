#ifndef CONTEXT
#define CONTEXT
#include "../symboltable/functiontable.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"
#include "../parser/ast.h"
#include "../asm/asmcontext.h"


typedef struct Context
{
    SymbolTableStack *symbolTableStack;
    FunctionTable *functionTable;
    ASMContext *asmContext;
    TokenTable *tokenTable;
    Token *tokens;
    Token current;
    Token lookahed;
    int loopStack;
    int index;

} Context;

Context *initContext();

#endif