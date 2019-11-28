#include "stack.h"



void initStack(Stack *stack){
    stack->top = -1;
}

int stackFull(Stack *stack)
{
    return stack->top == MAX_STACK - 1;
}

bool stackPush(Stack *stack, int numberOfSpaces)
{
    if(stackFull(stack))
    {
        return false;
    }
    stack->top++;
    stack->arr[stack->top] = numberOfSpaces;
    return true;
}

bool stackPop(Stack *stack){

    if(stackEmpty(stack))
    {
        return false;
    }

    stack->top--;
    return true;
}

int stackEmpty(Stack *stack)
{
    return stack->top == -1;
}


int stackTop(Stack* stack){

    //return stack->arr[stack->top];
    return stack->top;
}
