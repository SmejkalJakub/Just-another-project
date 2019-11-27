#include <stdlib.h>
#include <stdbool.h>

#include "symstack.h"
#include "expr.h"


void symStackInit(symStack *stack)
{
    stack->top = NULL;
}

bool symStackPush(symStack* stack, precedenceTabSym symbol, int type)
{
	symStackItem* newItem = (symStackItem*)malloc(sizeof(symStackItem));

	if (newItem == NULL)
		return false;

    else
    {
        newItem->symbol = symbol;
        newItem->type = type;
        newItem->nextItem = stack->top;

        stack->top = newItem;

        return true;
    }
    
}

bool symStackPop(symStack* stack)
{
	if (stack->top != NULL)
	{
		symStackItem* popedItem = stack->top;
		stack->top = popedItem->nextItem;
		free(popedItem);

		return true;
	}
    else
    {
	    return false;
        
    }
    
}


symStackItem* symStackTop(symStack* stack)
{
	return stack->top;
}


void symStackFree(symStack* stack)
{
	while (symStackPop(stack));
}