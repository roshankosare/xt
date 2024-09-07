#include "../../include/asm/asmcontext.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASMContext *initASMContext()
{

    ASMContext *context = (ASMContext *)malloc(sizeof(ASMContext));
    context->looplabelStack = initLoopLabelStack();
    return context;
}
LoopLabelStack *initLoopLabelStack()
{
    LoopLabelStack *stack = (LoopLabelStack *)malloc(sizeof(LoopLabelStack));
    stack->top = NULL;
    return stack;
}
void pushLoopLabelStack(ASMContext *asmContext, char *label)
{
    LoopLabelEntry *entry = (LoopLabelEntry *)malloc(sizeof(LoopLabelEntry));
    entry->label = (char *)malloc(sizeof(char) * 50);
    strcpy(entry->label, label);
    entry->next = asmContext->looplabelStack->top;
    asmContext->looplabelStack->top = entry;
    return;
}
LoopLabelEntry *popLoopLabelEntry(ASMContext *asmContext)
{
    if (asmContext->looplabelStack->top == NULL)
    {
        return NULL;
    }
    LoopLabelEntry *entry = asmContext->looplabelStack->top;
    asmContext->looplabelStack->top = entry;
    entry->next = NULL;
    return entry;
}
