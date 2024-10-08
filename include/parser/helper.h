#ifndef PARSER_HELPER
#define PARSER_HELPER

#include "../context/context.h"

int expect(Context *context, TokenValue t);
int match(Context *context, TokenValue t);
int isConditionalOperator(TokenValue t);
int isExpToken(TokenValue t);
void insertFuntionEntry(Context *context, Token t);
FunctionTableEntry *checkFuntionEntry(Context *context, Token t);
SymbolTableEntry *insertSymbolEntry(Context *context, Token t);
SymbolTableEntry *checkSymbolEntry(Context *context, Token t);
void insertParamSymbol(Context *context, Token function_name, Token identifier);
int getSymbolOffset(Context *context, SymbolTableEntry *entry);
char *label_generate();
char* remove_quotes(const char* input);

#endif