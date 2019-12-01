#include "symTableStack.h"


void STStackInit(STStack *stack)
{
    stack->top = NULL;
}


symTable *STStackPush(STStack *stack)
{
    STStackItem *newItem = malloc(sizeof(STStackItem));

    if(!newItem)
    {
        return NULL;
    }

    newItem->symTablePtr = malloc(sizeof(symTable));

    if(!(newItem->symTablePtr))
    {
        free(newItem);
        return NULL;
    }

    STInit(newItem->symTablePtr);

    newItem->next = stack->top;
    stack->top = newItem;

    return newItem->symTablePtr;
}


void STStackPop(STStack *stack)
{
    STStackItem *delItem = stack->top;

    if(delItem)
    {
        stack->top = delItem->next;

        STDelete(delItem->symTablePtr);

        free(delItem->symTablePtr);
        free(delItem);
    }
}


bool STStackEmpty(STStack *stack)
{
    return (stack->top == NULL);
}


symTable *STStackTop(STStack *stack)
{
    if(stack->top)
    {
        return stack->top->symTablePtr;
    }

    return NULL;
}


void STStackDelete(STStack *stack)
{
    while(!STStackEmpty(stack))
    {
        STStackPop(stack);
    }
}


