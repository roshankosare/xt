#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbol table entry structure
typedef struct SymbolTableEntry
{
    char name[100];                    // Identifier name              // Type of the identifier (e.g., int, float)
    int scope;                     // Scope level
    struct SymbolTableEntry *next; // Pointer to the next entry (for handling collisions in the hash table)
} SymbolTableEntry;

// Symbol table structure
#define TABLE_SIZE 100

typedef struct SymbolTable
{
    SymbolTableEntry *table[TABLE_SIZE]; // Array of pointers to symbol table entries
    struct SymbolTable *next;            // Pointer to the next symbol table (for stack implementation)
} SymbolTable;

// Stack of symbol tables structure
typedef struct SymbolTableStack
{
    SymbolTable *top; 
    int scope;// Pointer to the top of the stack
} SymbolTableStack;

void initSymbolTableStack(SymbolTableStack *stack);
SymbolTable *initSymbolTable();
void pushSymbolTable(SymbolTableStack *stack);
void popSymbolTable(SymbolTableStack *stack);
SymbolTableEntry *createEntry(char *name, int scope);
unsigned int hash(char *name);
void insertSymbol(SymbolTableStack *stack, char *name, int scope);
SymbolTableEntry *lookupSymbol(SymbolTableStack *stack, char *name);

// Function to initialize the symbol table stack
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
SymbolTableEntry *createEntry(char *name, int scope)
{
    SymbolTableEntry *newEntry = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    strcpy(newEntry->name,name);
    newEntry->scope = scope;
    newEntry->next = NULL;
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
void insertSymbol(SymbolTableStack *stack, char *name, int scope)
{
    if (stack->top == NULL)
    {
        return;
    }

    unsigned int index = hash(name);
    SymbolTableEntry *newEntry = createEntry(name, scope);
    newEntry->next = stack->top->table[index];
    stack->top->table[index] = newEntry;
}

// Function to lookup an entry in the symbol table stack
SymbolTableEntry *lookupSymbol(SymbolTableStack *stack, char *name)
{
   
    SymbolTable *current = stack->top;
    while (current != NULL)
    {

        unsigned int index = hash(name);
        SymbolTableEntry *entry = current->table[index];

        while (entry != NULL)
        {
            if (strcmp(entry->name, name) == 0)
            {
                return entry;
            }
            entry = entry->next; // Move to the next node in symbol table token index hased node
        }
        current = current->next; // Move to the next symbol table in the stack
    }
    return NULL; // Not found
}

#endif