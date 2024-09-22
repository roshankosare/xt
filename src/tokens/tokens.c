#include "../../include/tokens/tokens.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TokenTable *initTokenTable()
{
    TokenTable *table = (TokenTable *)malloc(sizeof(TokenTable));
    for (int i = 0; i < TOKEN_TABLE_SIZE; i++)
    {
        table->tokens[i] = NULL;
    }
    return table;
}
TokenEntry *initTokenEntry(const char *token, TokenValue value)
{
    TokenEntry *entry = (TokenEntry *)malloc(sizeof(TokenEntry));
    strcpy(entry->token, token);
    entry->value = value;
    entry->next = NULL;
    return entry;
}

unsigned int hashToken(char *name)
{
    unsigned int hash_value = 0;
    for (char *p = name; *p != '\0'; p++)
    {
        hash_value = (hash_value << 5) + *p;
    }
    return hash_value % TOKEN_TABLE_SIZE;
}

void insertTokenEntryToTokenTable(TokenTable *table, TokenEntry *entry)
{
    unsigned int index = hashToken(entry->token);
    entry->next = table->tokens[index];
    table->tokens[index] = entry;
}
TokenEntry *lookUpTokenEntry(TokenTable *table, char *token)
{

    unsigned int index = hashToken(token);
    TokenEntry *entry;
    TokenEntry *e;
    entry = table->tokens[index];
    while (entry != NULL)
    {
        // printf("\ncurrent token: %s  looking for: %s",entry->token,token);
        if (strcmp(token, entry->token) == 0)
        {
            
            e = initTokenEntry(entry->token, entry->value);
            e->next = NULL;
            return e;
        }
        entry = entry->next;
    }
    return NULL;
}

void fillEntrys(TokenTable *table)
{
    TokenEntry *entry;
    entry = initTokenEntry("if", IF);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("function", FUNCTION);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("var", VAR);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("if", IF);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("else", ELSE);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("return", RETURN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("while", WHILE);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("and", LOGICAL_AND);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("or", LOGICAL_OR);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("not", LOGICAL_NOT);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("asm", ASM);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("int", INT_T);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("char", CHAR_T);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("float", FLOAT_T);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("string", STRING_T);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("continue", CONTINUE);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("break", BREAK);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("typeof", TYPEOF);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("+", PLUS);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("-", MINUS);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("*", MUL);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("/", DIV);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("%", MODULAS);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("@", VALUE_AT);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("=", ASSIGN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("==", EQUALTO);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("(", OPEN_PAREN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry(")", CLOSE_PAREN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("{", OPEN_CURLY_PAREN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("}", CLOSE_CURLY_PAREN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("[", OPEN_SQ_PARAN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("]", CLOSE_SQ_PARAN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("\"", DOUBLE_QT);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("\'", SINGLE_QT);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry(":", COLAN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry(";", SEMI_COLAN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("<", LESS_THAN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry(">", GRATER_THAN);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("<=", LESS_THAN_EQTO);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry(">=", GRATER_THAN_EQTO);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("!=", NOT_EQLTO);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry(",", COMMA);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("++", INC);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("--", DEC);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("&", BIT_AND);
    insertTokenEntryToTokenTable(table, entry);
    entry = initTokenEntry("|", BIT_OR);
    insertTokenEntryToTokenTable(table, entry);
    
    entry = NULL;
    return;
}