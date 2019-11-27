#include "expr.h"
typedef struct stackItem
{
	precedenceTabSym symbol; /// Symbol of stack item.
	int type;
	struct stackItem *nextItem;
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
