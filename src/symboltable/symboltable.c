
#include "../../include/symboltable/symboltable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initSymbolTableStack(SymbolTableStack *stack)
{
    stack->scope = 0;
    stack->top = NULL;
}

// Function to initialize a new symbol table
SymbolTable *initSymbolTable()
{
    SymbolTable *symbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        symbolTable->table[i] = NULL;
    }
    symbolTable->next = NULL;
    return symbolTable;
}

// Function to push a new symbol table onto the stack
void pushSymbolTable(SymbolTableStack *stack)
{
    SymbolTable *symbolTable = initSymbolTable();
    stack->scope++;
    symbolTable->next = stack->top;
    stack->top = symbolTable;
}

// Function to pop the top symbol table from the stack
void popSymbolTable(SymbolTableStack *stack)
{
    if (stack->top != NULL)
    {
        SymbolTable *top = stack->top;
        stack->top = stack->top->next;
        free(top);
    }
}

// Function to create a new symbol table entry
SymbolTableEntry *createEntry(Token t, int scope)
{
    SymbolTableEntry *newEntry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    newEntry->token = t;
    newEntry->scope = scope;
    newEntry->next = NULL;
    newEntry->isDefined = 0;
    return newEntry;
}

// Hash function to calculate the index for a given identifier name
unsigned int hash(char *name)
{
    unsigned int hash_value = 0;
    for (char *p = name; *p != '\0'; p++)
    {
        hash_value = (hash_value << 5) + *p;
    }
    return hash_value % TABLE_SIZE;
}

// Function to insert an entry into the current symbol table
void insertSymbol(SymbolTableStack *stack, Token t)
{
    if (stack->top == NULL)
    {
        return;
    }

    unsigned int index = hash(t.lexeme);
    SymbolTableEntry *newEntry = createEntry(t, stack->scope);
    newEntry->next = stack->top->table[index];
    stack->top->table[index] = newEntry;
}

// Function to lookup an entry in the symbol table stack
SymbolTableEntry *lookupSymbol(SymbolTableStack *stack, Token t)
{
    SymbolTable *current = stack->top;
    while (current != NULL)
    {
        SymbolTableEntry *entry = lookupSymbolInSymbolTable(current, t);
        if (entry != NULL)
        {
            return entry;
        }
        current = current->next; // Move to the next symbol table in the stack
    }
    return NULL; // Not found
}

SymbolTableEntry *lookupSymbolInSymbolTable(SymbolTable *table, Token t)
{
    unsigned int index = hash(t.lexeme);
    SymbolTableEntry *entry = table->table[index];

    while (entry != NULL)
    {
        if (strcmp(entry->token.lexeme, t.lexeme) == 0)
        {
            return entry;
        }
        entry = entry->next; // Move to the next node in symbol table token index hased node
    }
    return NULL; // Not found
}

void insertSymbolInSymbolTable(SymbolTable *symbolTable, Token t)
{

    unsigned int index = hash(t.lexeme);
    SymbolTableEntry *newEntry = createEntry(t, 0);
    newEntry->next = symbolTable->table[index];
    symbolTable->table[index] = newEntry;
}