#include "../../include/parser/helper.h"
#include "../../include/tokens/tokens.h"
#include "../../include/symboltable/symboltable.h"
#include "../../include/symboltable/functiontable.h"
#include "../../include/tokens/tokenizer.h"
#include "ast.h"

void unexpected(Context *context)
{

    printf("\nERROR: unexpected token %s at line %d and col %d ", context->current.value, context->current.pos.line, context->current.pos.col);
    exit(1);
}

int expect(Context *context, TokenValue t)
{
    if (context->current.value == t)
    {
        return 1;
    }
    printf("\nERROR: expected token %s  recived token %s at line %d and col %d ", getTokenStringValue(t), context->current.lexeme, context->current.pos.line, context->current.pos.col);
    exit(1);
}


int match(Context *context, TokenValue t)
{
    if (t == EXP_TOKEN)
    {
        if (isExpToken(context->current.value))
        {
            return 1;
        }
        return 0;
    }

    if (t == CONDITIONAL_TOKEN)
    {
        if (isConditionalOperator(context->current.value))
        {
            return 1;
        }
        return 0;
    }
    if (context->current.value == t)
    {
        return 1;
    }
    return 0;
}

int isConditionalOperator(TokenValue t)
{
    switch (t)
    {
    case LESS_THAN:
        return 1;
        break;
    case GRATER_THAN:
        return 1;
        break;
    case LESS_THAN_EQTO:
        return 1;
        break;
    case GRATER_THAN_EQTO:
        return 1;
        break;
    case EQUALTO:
        return 1;
        break;

    default:
        return 0;
        break;
    }
}

int isExpToken(TokenValue t)
{
    switch (t)
    {
    case PLUS:
        return 1;
    case MINUS:
        return 1;
    case MUL:
        return 1;
    case DIV:
        return 1;
    case LOGICAL_AND:
        return 1;
    case LOGICAL_OR:
        return 1;
    case EQUALTO:
        return 1;
    case LESS_THAN:
        return 1;
    case GRATER_THAN:
        return 1;
    case LESS_THAN_EQTO:
        return 1;
    case GRATER_THAN_EQTO:
        return 1;
    case NOT_EQLTO:
        return 1;
    case BIT_AND:
        return 1;
    case BIT_OR:
        return 1;
    case MODULAS:
        return 1;

    default:
        return 0;
    }
}

void insertFuntionEntry(Context *context, Token t)
{
    if (t.value != IDENTIFIER)
    {
        printf("\nERROR : funtion or symbol entry must be identifer\n");
        exit(1);
    }
    SymbolTableEntry *symbolEntry = lookupSymbol(context->symbolTableStack, t);
    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->functionTable, t);
    if (symbolEntry == NULL && functionEntry == NULL)
    {
        insertFuntionSymbol(context->functionTable, t);
        return;
    }
    if (symbolEntry)
    {
        printf("\nERROR : identifer is already used at line at line %d and col %d", symbolEntry->token.pos.line, symbolEntry->token.pos.col);
        exit(1);
    }

    if (functionEntry)
    {
        printf("\nERROR : identifer is already used at line at line %d and col %d",
               functionEntry->token.pos.line, functionEntry->token.pos.col);
        exit(1);
    }
    return;
}

FunctionTableEntry *checkFuntionEntry(Context *context, Token t)
{

    if (t.value != IDENTIFIER)
    {
        printf("\nERROR : funtion or symbol entry must be identifer\n");
        exit(1);
    }
    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->functionTable, t);
    if (functionEntry)
    {
        return functionEntry;
    }
    printf("\nERROR : undefined function `%s` at line at line %d and col %d", t.lexeme, t.pos.line, t.pos.col);
    exit(1);
}

SymbolTableEntry *insertSymbolEntry(Context *context, Token t)
{
    SymbolTableEntry *symbolEntry = lookupSymbol(context->symbolTableStack, t);
    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->functionTable, t);

    if (functionEntry != NULL)
    {
        printf("\nERROR : identifer `%s`is already used at line at line %d and col %d",
               t.lexeme, functionEntry->token.pos.line, functionEntry->token.pos.col);
        exit(1);
    }
    if (symbolEntry)
    {
        if (symbolEntry->scope == context->symbolTableStack->scope)
        {
            printf("\nERROR : identifer `%s`is already used at line at line %d and col %d", t.lexeme, symbolEntry->token.pos.line, symbolEntry->token.pos.col);
            exit(1);
        }
    }

    insertSymbol(context->symbolTableStack, t);
    SymbolTableEntry *entry = lookupSymbol(context->symbolTableStack, t);
    if (entry)
    {
        return entry;
    }
    return NULL;
}

SymbolTableEntry *checkSymbolEntry(Context *context, Token t)
{

    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->functionTable, t);
    if (functionEntry != NULL)
    {
        printf("\nERROR : invalid function call  at line at line %d and col %d", t.pos.line, t.pos.col);
        exit(1);
    }

    SymbolTableEntry *symbolEntry = lookupSymbol(context->symbolTableStack, t);
    if (symbolEntry != NULL)
    {
        if (!symbolEntry->isDefined)
        {
            printf("\nERROR : variable is used before its decleration  `%s` at line at line %d and col %d", t.lexeme, t.pos.line, t.pos.col);
            exit(1);
        }
        return symbolEntry;
    }

    printf("\nERROR : undefined variable  `%s` at line at line %d and col %d", t.lexeme, t.pos.line, t.pos.col);
    exit(1);
}

void insertParamSymbol(Context *context, Token function_name, Token identifier)
{

    FunctionTableEntry *functionEntry = lookupFuntionSymbol(context->functionTable, function_name);
    if (functionEntry)
    {
        SymbolTableEntry *symbolEntry = lookupSymbolInSymbolTable(functionEntry->parameterTable, identifier);
        if (symbolEntry)
        {
            printf("\nERROR : identifer is already used at line at line %d and col %d", symbolEntry->token.pos.line, symbolEntry->token.pos.col);
            exit(1);
        }
        insertSymbolInSymbolTable(functionEntry->parameterTable, identifier);
        return;
    }

    printf("\nERROR : undefined function at line at line %d and col %d", function_name.pos.line, function_name.pos.col);
    exit(1);
}


int getSymbolOffset(Context *context, SymbolTableEntry *entry)
{

    SymbolTable *table = context->symbolTableStack->top;

    int scope = context->symbolTableStack->scope;
    int offset = 0;
    if (scope == entry->scope)
    {
        return entry->symbolOffset * -1;
    }
    while (entry->scope < scope && table->next->next != NULL)
    {
        offset = table->scopeOffset + offset;
        table = table->next;
        scope--;
        if (entry->scope == scope)
        {
            offset = offset  - entry->symbolOffset + 4;
            return offset;
        }
        offset += 4;
    }
    return offset;
}

char *label_generate()
{
    char *label = malloc(20 * sizeof(char)); // Allocate memory for 20 characters (adjust size as needed)

    if (label == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    snprintf(label, 20, "label_%d", rand());

    return label;
}

char *remove_quotes(const char *input)
{
    // Ensure input is valid
    if (input == NULL || strlen(input) < 2)
    {
        return NULL;
    }

    // Check if the string starts and ends with the same type of quote
    char start_quote = input[0];
    char end_quote = input[strlen(input) - 1];

    if ((start_quote != '"' && start_quote != '\'') ||
        (end_quote != '"' && end_quote != '\'') ||
        (start_quote != end_quote))
    {
        return NULL;
    }

    // Calculate the length of the new string (excluding the quotes)
    size_t length = strlen(input) - 2;
    char *result = (char *)malloc(length + 1);

    if (result == NULL)
    {
        return NULL;
    }

    // Copy the string without the quotes
    strncpy(result, input + 1, length);
    result[length] = '\0'; // Null-terminate the string

    return result;
}