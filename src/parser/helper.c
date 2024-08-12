#include "../../include/parser/helper.h"
#include "../../include/tokens/tokens.h"

void unexpected(Context *context)
{

    printf("\nERROR: unexpected token %s at line %d and col %d ", context->current.value, context->current.pos.line, context->current.pos.col);
    exit(1);
}

int expect(Context *context, TokenValue t)
{
    if (context->current.value == t)
    {
        return 1;
    }
    printf("\nERROR: expected token %s  recived token %s at line %d and col %d ", getTokenStringValue(t), context->current.lexeme, context->current.pos.line, context->current.pos.col);
    exit(1);
}
void consume(Context *context)
{
    (context->index)++;
    context->current = context->tokens[context->index];
    context->lookahed = context->tokens[context->index + 1];
    return;
}
void unconsume(Context *context)
{
    (context->index)--;
    context->current = context->tokens[context->index];
    context->lookahed = context->tokens[context->index + 1];
    return;
}

int match(Context *context, TokenValue t)
{
    if (t == EXP_TOKEN)
    {
        if (isExpToken(context->current.value))
        {
            return 1;
        }
        return 0;
    }

    if (t == CONDITIONAL_TOKEN)
    {
        if (isConditionalOperator(context->current.value))
        {
            return 1;
        }
        return 0;
    }
    if (context->current.value == t)
    {
        return 1;
    }
    return 0;
}

int isConditionalOperator(TokenValue t)
{
    switch (t)
    {
    case LESS_THAN:
        return 1;
        break;
    case GRATER_THAN:
        return 1;
        break;
    case LESS_THAN_EQTO:
        return 1;
        break;
    case GRATER_THAN_EQTO:
        return 1;
        break;
    case EQUALTO:
        return 1;
        break;

    default:
        return 0;
        break;
    }
}

int isExpToken(TokenValue t)
{
    switch (t)
    {
    case PLUS:
        return 1;
    case MINUS:
        return 1;
    case MUL:
        return 1;
    case DIV:
        return 1;
    case LOGICAL_AND:
        return 1;
    case LOGICAL_OR:
        return 1;
    case EQUALTO:
        return 1;
    case LESS_THAN:
        return 1;
    case GRATER_THAN:
        return 1;
    case LESS_THAN_EQTO:
        return 1;
    case GRATER_THAN_EQTO:
        return 1;

    default:
        return 0;
    }
}