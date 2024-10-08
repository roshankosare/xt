
#include "../../include/context/context.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../include/parser/ast.h"

Context *initContext()
{
    Context *context = (Context *)malloc(sizeof(Context));
    context->symbolTableStack = (SymbolTableStack *)malloc(sizeof(SymbolTableStack));
    context->tokenTable = initTokenTable();
    // context->tokens = tokens;
    context->index = 0;
    context->current.value = START;
    // context->lookahed = tokens[1];
    context->loopStack = -1;
    context->tokenizerStateStack = initTokenizerStateStack();
    context->asmContext = initASMContext();

    initSymbolTableStack(context->symbolTableStack);
    context->functionTable = initFuntionTable();
    return context;
};