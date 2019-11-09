#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

/*ZÁSOBNÍK
NEVIM, JAKE VSECHNY FUNKCE BY MEL UMET,
 PROTO ASPON TY ZAKLADNI. STEJNE TAK STRUKTURA POLOZKY, JELIKOZ NEVIM, CO JE POTREBA ZNAT*/



typedef struct stackItem{                         
    //
    //
    //
    struct stackItem* next;
} StackItem;

typedef struct stack{
    StackItem * top;
}Stack;

void initStack(Stack *stack){
    stack->top = NULL;
}

bool stackPush(Stack *stack,   ,   ,    ){
    StackItem* newItem = (*StackItem) malloc (sizeof(StackItem));
    if (newItem == NULL){
        return false;
    }

    newItem->next = stack->top;

    return true;
}

bool stackPop(Stack *stack){
    if (stack->top == NULL){
        return false;
    }

    StackItem* popedItem = stack->top;
    stack->top = popedItem->next;
    free(popedItem);
    return true;
}


void stackFree(Stack* stack){
    while (stack->top != NULL){
        stackPop(stack);
    }
}

StackItem* stackTop(Stack* stack){
    return stack->top;
}
