#include <stdlib.h>
#include <stdbool.h>

#include "symstack.h"


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
		newItem->id = malloc(80);

        stack->top = newItem;

        return true;
    }

}

void symStackPushStop(symStack *stack)
{
	symStackItem *prevItem = NULL;
	
	symStackItem *tempItem = stack->top;

	while (true)
	{
		if(tempItem == NULL)
		{
			return;
		}

		if(tempItem->symbol < SYM_STOP)
		{
			symStackItem *stop = (symStackItem *) malloc(sizeof(symStackItem));

			if(prevItem != NULL)
			{
				stop->nextItem = prevItem->nextItem;
				prevItem->nextItem = stop;
			}
			else
			{
				stop->nextItem = stack->top;
				stack->top = stop;
			}

			stop->symbol = SYM_STOP;
			stop->type = TYPE_NONE;
			
			return;
		}

		prevItem = tempItem;
		tempItem = tempItem->nextItem;
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
