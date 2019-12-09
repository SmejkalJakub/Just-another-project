#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include "dynamicString.h"

#define TYPE_NONE 0
#define INT 1
#define DOUBLE 2
#define STRING 3
#define BOOL 4
#define SPEC_TYPE_NONE 5
#define EMPTY_TYPE -99

#define ST_SIZE 65003

typedef struct hashTableItem{
    char *key;
    int type;
    int numberOfParams;
    DS *params;
    bool function;
    bool defined;
    struct hashTableItem *next;
} symTableItem;

typedef symTableItem *symTable[ST_SIZE];

void STInit(symTable *STptr);
symTableItem *STSearch(symTable* STptr, char *key);
symTableItem *STInsert(symTable* STptr, char *key);
bool STAddParam(symTableItem *item, int type);
void STDeleteItem(symTable *STptr, char *key);
void STDelete(symTable *STptr);

#endif
