#ifndef PARSER_HELPER
#define PARSER_HELPER

#include "../context/context.h"

int expect(Context *context, TokenValue t);
void consume(Context *context);
void unconsume(Context *context);
int match(Context *context, TokenValue t);
int isConditionalOperator(TokenValue t);
int isExpToken(TokenValue t);
void insertFuntionEntry(Context *context, Token t);
int checkFuntionEntry(Context *context, Token t);
void insertSymbolEntry(Context *context, Token t);
SymbolTableEntry *checkSymbolEntry(Context *context, Token t);
void insertParamSymbol(Context *context, Token function_name,Token identifier);

#endif