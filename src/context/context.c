
#include "../../include/context/context.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser/ast.h"

Context *initContext(Token *tokens)
{
    Context *context = (Context *)malloc(sizeof(Context));
    context->symbolTableStack = (SymbolTableStack *)malloc(sizeof(SymbolTableStack));
    context->symbolTableTempStack = (SymbolTableStack *)malloc(sizeof(SymbolTableStack));
    context->astQueue = initASTQueue();
    context->astStack = initASTStack();
    context->litralTable = initLitralTable();
    context->tokens = tokens;
    context->index = 0;
    context->current = tokens[0];
    context->lookahed = tokens[1];


    initSymbolTableStack(context->symbolTableStack);
    initSymbolTableStack(context->symbolTableTempStack);
    context->functionTable = initFuntionTable();
    context->symbolTableQueue = NULL;
    return context;
};