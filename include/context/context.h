#ifndef CONTEXT
#define CONTEXT
#include "../symboltable/functiontable.h"
#include "../symboltable/symboltable.h"
#include "../tokens/tokens.h"
#include "../parser/ast.h"
#include "../litrals/litrals.h"
#include "../asm/asmcontext.h"


typedef struct Context
{
    SymbolTableStack *symbolTableStack;
    FunctionTable *functionTable;
    SymbolTable * symbolTableQueue;
    SymbolTableStack *symbolTableTempStack;
    LitralTable *litralTable;
    ASMContext *asmContext;
    Token *tokens;
    Token current;
    Token lookahed;
    int loopStack;
    ASTQueue *astQueue;
    ASTStack *astStack;
    int index;

} Context;

Context *initContext(Token *tokens);

#endif