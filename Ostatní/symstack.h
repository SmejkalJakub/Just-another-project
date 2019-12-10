#ifndef SYMSTACK_H_INCLUDED
#define SYMSTACK_H_INCLUDED

#include "expr.h"
#include "symtable.h"

typedef struct stackItem
{
	precedenceTabSym symbol; /// Symbol of stack item.
	int type;
	struct stackItem *nextItem;
	bool isZero;
	char *id;
} symStackItem;


typedef struct
{
	symStackItem *top; /// Pointer to stack item on top of stack.
} symStack;

void symStackInit(symStack *stack);
bool symStackPush(symStack* stack, precedenceTabSym symbol, int type);
bool symStackPop(symStack* stack);
symStackItem* symStackTop(symStack* stack);
void symStackFree(symStack* stack);
void symStackPushStop(symStack *stack);
#endif
