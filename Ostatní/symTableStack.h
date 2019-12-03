#ifndef SYMTABLESTACK_H_INCLUDED
#define SYMTABLESTACK_H_INCLUDED

#include "symtable.h"

typedef struct symTabStackItem{
    symTable *symTablePtr;
    int index;
    struct symTabStackItem *next;
} STStackItem;

typedef struct symTabStack{
    STStackItem *top;
} STStack;


void STStackInit(STStack *stack);
symTable *STStackPush(STStack *stack);
void STStackPop(STStack *stack);
bool STStackEmpty(STStack *stack);
symTable *STStackTop(STStack *stack);
void STStackDelete(STStack *stack);
symTableItem *STStackSearch(STStack *stack, char *key, int *index);


#endif
