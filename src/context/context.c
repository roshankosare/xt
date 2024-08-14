
#include "../../include/context/context.h"
#include <stdio.h>
#include <stdlib.h>

Context *initContext(Token *tokens)
{
    Context *context = (Context *)malloc(sizeof(Context));
    context->symbolTableStack = (SymbolTableStack *)malloc(sizeof(SymbolTableStack));
    context->tokens = tokens;
    context->index = 0;
    context->current = tokens[0];
    context->lookahed = tokens[1];

    initSymbolTableStack(context->symbolTableStack);
    context->functionTable = initFuntionTable();
    return context;
};