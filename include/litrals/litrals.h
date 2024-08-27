#ifndef LITRALS_H
#define LITRALS_H

typedef struct Litral
{
    char label[50];
    char *value[100];
    struct Litral *next;
} Litral;

typedef struct LitralTable
{
    Litral *top;
} LitralTable;

LitralTable *initLitralTable();
void pushLitralTable(LitralTable *table, char *label, char *value);
Litral *popLitlralTable(LitralTable *table);

#endif