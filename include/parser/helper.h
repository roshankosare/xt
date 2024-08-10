#ifndef PARSER_HELPER
#define PARSER_HELPER

#include "../context/context.h"

int expect(Context *context, TokenValue t);
void consume(Context *context);
int match(Context *context,TokenValue t);
int isConditionalOperator(TokenValue t);

#endif