/*
*Implementace p�eklada�e imperativn�ho jazyka IFJ19
*
*Jakub Smejkal (xsmejk28)
*Adam Grunwald (xgrunw00)
*
*
*Zasobnik
*
*FIT VUT BRNO
*/
#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include <stdbool.h>
#define MAX_STACK 60


/*Zasobnik*/
typedef struct stack{
    char arr[MAX_STACK];
	int top;
}Stack;

/*Funkce pro inicializaci zasobniku*/
void initStack(Stack *stack);

/*Funkce pro vlozeni poctu mezer na zasobnik*/
bool stackPush(Stack *stack, int numberOfSpaces);

/*Funkce pro vyjmuti vrchni polozky ze zasobniku*/
bool stackPop(Stack *stack);

/*Funkce uvolnujiic zasobnik*/
void stackFree(Stack* stack);

/*Funkce vracejici hodnotu na vrcholu zasobniku*/
int stackTop(Stack* stack);

/*Funkce kontrolujiic prazdnost zasobniku*/
int stackEmpty(Stack *stack);
#endif
