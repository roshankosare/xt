#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

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
    char name[100]; // Identifier name              // Type of the identifier (e.g., int, float)
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
SymbolTableEntry *createEntry(char *name, int scope);
unsigned int hash(char *name);
void insertSymbol(SymbolTableStack *stack, char *name, int scope);
SymbolTableEntry *lookupSymbol(SymbolTableStack *stack, char *name);

// Function to initialize the symbol table stack

#endif