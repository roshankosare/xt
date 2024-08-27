#include "../../include/litrals/litrals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

LitralTable *initLitralTable()
{

    LitralTable *table = (LitralTable *)malloc(sizeof(LitralTable));
    if (table == NULL)
    {
        printf("\ cannot allocated memory for litral table");
        exit(1);
    }
    table->top = NULL;
    return table;
}
void pushLitralTable(LitralTable *table, char *label, char *value)
{

    
    Litral *entry = (Litral *)malloc(sizeof(Litral));
    if (entry == NULL)
    {
        printf("\ cannot allocated memory for litral");
        exit(1);
    }
    entry->next = table->top;
    strcpy(entry->value,value);
    strcpy(entry->label,label);
    table->top = entry;
    return;
}
Litral *popLitlralTable(LitralTable *table)
{
    if (table->top == NULL)
    {
        return NULL;
    }
    Litral *entry = table->top;
    table->top = entry->next;
    return entry;
}