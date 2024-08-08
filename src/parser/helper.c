#include "../../include/parser/helper.h"
#include "../../include/tokens/tokens.h"

int expect(Context *context, TokenValue t)
{
    if (context->current.value == t)
    {
        return 1;
    }
    printf("\nERROR: expected token %s  recived token %s at line %d and col %d ", getTokenStringValue(t),context->current.lexeme, context->current.pos.line, context->current.pos.col);
    exit(1);
}
void consume(Context *context)
{
    (context->index)++;
    context->current = context->tokens[context->index];
    context->lookahed = context->tokens[context->index + 1];
    return;
}

int match(Context *context, TokenValue t)
{
    if (context->current.value == t)
    {
        return 1;
    }
    return 0;
}