#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include "dynamicString.h"

#define TYPE_NONE 0
#define INT 1
#define DOUBLE 2
#define STRING 3
#define BOOL 4

#define ST_SIZE 65003

typedef struct hashTableItem{
    char *key;
    int type;
    int numberOfParams;
    DS *params;
    bool global;
    struct hashTableItem *next;
} symTableItem;

typedef symTableItem *symTable[ST_SIZE];

void STInit(symTable *STptr);
symTableItem *STSearch(symTable* STptr, char *key);
symTableItem *STInsert(symTable* STptr, char *key);
bool STAddParam(symTableItem *item, int type);

#endif
