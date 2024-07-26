#ifndef FUNTION_TABLE
#define FUNTION_TABLE
#include "symboltable.h"

typedef struct FunctionTableEntry
{
    char name[100];
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
FunctionTableEntry *createFuntionEntry(char *name);
void insertFuntionSymbol(FunctionTable *funtionTable, char *name);
SymbolTableEntry *lookupFuntionSymbol(FunctionTable *funtionTable, char *name);

#endif
