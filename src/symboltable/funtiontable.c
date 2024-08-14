#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FunctionTable *initFuntionTable()
{
    FunctionTable *funtionTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    for (int i = 0; i < FUNTION_TABLE_SIZE; i++)
    {
        funtionTable->table[i] = NULL;
    }

    return funtionTable;
}
FunctionTableEntry *createFuntionEntry(Token t)
{
    FunctionTableEntry *newEntry = (FunctionTableEntry *)malloc(sizeof(FunctionTableEntry));
    newEntry->token = t;
    newEntry->parameterTable = initSymbolTable();
    newEntry->next = NULL;
    return newEntry;
}
void insertFuntionSymbol(FunctionTable *funtionTable, Token t)
{
    unsigned int index = hash(t.lexeme);
    SymbolTableEntry *newEntry = createFuntionEntry(t);
    funtionTable->table[index] = newEntry;
}

FunctionTableEntry *lookupFuntionSymbol(FunctionTable *funtionTable, Token t)
{
    unsigned int index = hash(t.lexeme);
    FunctionTableEntry *entry = funtionTable->table[index];

    while (entry != NULL)
    {
        if (strcmp(entry->token.lexeme, t.lexeme) == 0)
        {
            return entry;
        }
        entry = entry->next; // Move to the next node in symbol table token index hased node
    }
    return NULL;
}

