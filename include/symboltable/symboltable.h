
#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE
#include "../tokens/tokens.h"

typedef enum Type
{
    INT,
    STRING,
    FlOAT,
    UNDEFINED,

} Type;

// Symbol table entry structure
typedef struct SymbolTableEntry
{
    Token token; // Identifier name              // Type of the identifier (e.g., int, float)
    int scope;
    int isDefined; // Scope level
    struct SymbolTableEntry *next;
    Type type // Pointer to the next entry (for handling collisions in the hash table)
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
    int scope; // Pointer to the top of the stack
} SymbolTableStack;

void initSymbolTableStack(SymbolTableStack *stack);
SymbolTable *initSymbolTable();
void pushSymbolTable(SymbolTableStack *stack);
void popSymbolTable(SymbolTableStack *stack);
SymbolTableEntry *createEntry(Token t,int scope);
unsigned int hash(char *name);
void insertSymbol(SymbolTableStack *stack,Token t);
SymbolTableEntry *lookupSymbol(SymbolTableStack *stack, Token t);
SymbolTableEntry *lookupSymbolInSymbolTable(SymbolTable *table, Token t);
void insertSymbolInSymbolTable(SymbolTable *symbolTable, Token t);

// Function to initialize the symbol table stack

#endif