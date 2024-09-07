#ifndef ASM_CONTEXT
#define ASM_CONTEXT
typedef struct LoopLabelEntry
{
    char *label;
    struct LoopLabelEntry *next;

} LoopLabelEntry;
typedef struct LoopLabelStack
{
    LoopLabelEntry *top;

} LoopLabelStack;

typedef struct ASMContext
{

    LoopLabelStack *looplabelStack;

} ASMContext;

ASMContext *initASMContext();
LoopLabelStack *initLoopLabelStack();
void pushLoopLabelStack(ASMContext *asmContext, char *label);
LoopLabelEntry *popLoopLabelEntry(ASMContext *asmContext);


#endif