#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include <stdbool.h>
#define MAX_STACK 60


typedef struct stack{
    char arr[MAX_STACK];
	int top;
}Stack;


void initStack(Stack *stack);
bool stackPush(Stack *stack, int numberOfSpaces);
bool stackPop(Stack *stack);
void stackFree(Stack* stack);
int stackTop(Stack* stack);
int stackEmpty(Stack *stack);
#endif
