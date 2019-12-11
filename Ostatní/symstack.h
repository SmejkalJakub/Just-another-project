/*
*Implementace p�eklada�e imperativn�ho jazyka IFJ19
*
*
*Dominik Nejedly (xnejed09)
*Jakub Smejkal (xsmejk28)
*
*
*Zasobnik symbolu
*
*FIT VUT BRNO
*/

#ifndef SYMSTACK_H_INCLUDED
#define SYMSTACK_H_INCLUDED

#include "expr.h"
#include "symtable.h"


/*Polozka zasobniku symbolu*/
typedef struct stackItem
{
	precedenceTabSym symbol;        //Symbol polozky zasobniku
	int type;                       //typ polozky
	struct stackItem *nextItem;     //ukazatel na dalsi polozku
	bool isZero;
	char *id;                       //identifikator
} symStackItem;


/*Zasobnik s ukazatelem na vrchni polozku*/
typedef struct
{
	symStackItem *top;

} symStack;


/*Funkce inicializace zasobniku symbolu*/
void symStackInit(symStack *stack);

/*Funkce pro vlozeni polozky na vrchol zasobniku*/
bool symStackPush(symStack* stack, precedenceTabSym symbol, int type);

/*Funkce pro vyjmuti polozky z vrcholu zasobniku*/
bool symStackPop(symStack* stack);

/*Funkce vracejiici ukazatel na polozku na vrcholu zasobniku*/
symStackItem* symStackTop(symStack* stack);

/*Funkce pro uvolneni zasobniku*/
void symStackFree(symStack* stack);

/*Funkce pushujici po zarazku do zasobniku*/
void symStackPushStop(symStack *stack);
#endif
