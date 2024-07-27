
#include "../../include/context/context.h"
#include <stdio.h>
#include <stdlib.h>

Context *initContext(Token *tokens)
{
    Context *context = (Context *)malloc(sizeof(Context));
    context->stack = (SymbolTableStack *)malloc(sizeof(SymbolTableStack));
    context->tokens = tokens;
    context->index = 0;
    initSymbolTableStack(context->stack);
    context->table = initFuntionTable();
    return context;
};