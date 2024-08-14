#ifndef FUNTION_TABLE
#define FUNTION_TABLE
#include "symboltable.h"

typedef struct FunctionTableEntry
{
    Token token;
    char returnType;
    SymbolTable *parameterTable;
    struct FunctionTableEntry *next;

} FunctionTableEntry;

#define FUNTION_TABLE_SIZE 100

typedef struct FunctionTable
{
    FunctionTableEntry *table[FUNTION_TABLE_SIZE]

} FunctionTable;

FunctionTable *initFuntionTable();
FunctionTableEntry *createFuntionEntry(Token t);
void insertFuntionSymbol(FunctionTable *funtionTable, Token t);
FunctionTableEntry *lookupFuntionSymbol(FunctionTable *funtionTable, Token t);

#endif
