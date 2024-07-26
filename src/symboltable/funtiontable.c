#include "../../include/symboltable/functiontable.h"
#include "../../include/symboltable/symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FunctionTable *initFuntionTable()
{
    FunctionTable *funtionTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        funtionTable->table[i] = NULL;
    }

    return funtionTable;
}
FunctionTableEntry *createFuntionEntry(char *name)
{
    FunctionTableEntry *newEntry = (FunctionTableEntry *)malloc(sizeof(FunctionTableEntry));
    strcpy(newEntry->name, name);
    newEntry->next = NULL;
    return newEntry;
}
void insertFuntionSymbol(FunctionTable *funtionTable, char *name)
{
    unsigned int index = hash(name);
    SymbolTableEntry *newEntry = createFuntionEntry(name);
    funtionTable->table[index] = newEntry;
}
SymbolTableEntry *lookupFuntionSymbol(FunctionTable *funtionTable, char *name)
{
    unsigned int index = hash(name);
    FunctionTableEntry *entry = funtionTable->table[index];

    while (entry != NULL)
    {
        if (strcmp(entry->name, name) == 0)
        {
            return entry;
        }
        entry = entry->next; // Move to the next node in symbol table token index hased node
    }
    return NULL;
}