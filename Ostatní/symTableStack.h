/*
*Implementace p�eklada�e imperativn�ho jazyka IFJ19
*
*Dominik Nejedly (xnejed09)
*
*
*Zasobnik tabulek symbolu
*
*FIT VUT BRNO
*/


#ifndef SYMTABLESTACK_H_INCLUDED
#define SYMTABLESTACK_H_INCLUDED

#include "symtable.h"

/*
*Struktura polozky zasobniku tabulek symbolu
*Obsahuje ukazatel na tabulku, index
*a ukazatel na dalsi tabulku
*/
typedef struct symTabStackItem{
    symTable *symTablePtr;
    int index;
    struct symTabStackItem *next;
} STStackItem;

/*
*Struktura zasobniku tabulek s ukazatelem na vrchni polozku
*/
typedef struct symTabStack{
    STStackItem *top;
} STStack;

/*Inicializuje zasobnik*/
void STStackInit(STStack *stack);

/*Vklada tabulku na vrchol zasobniku*/
symTable *STStackPush(STStack *stack);

/*Vyjima tabulku z vrchu zasobniku*/
void STStackPop(STStack *stack);

/*Funkce kontrolujici prazdnost zasobniku*/
bool STStackEmpty(STStack *stack);

/*Funkce vracejici tabulku z vrcholu zasobniku*/
symTable *STStackTop(STStack *stack);

/*Maze cely zasobnik*/
void STStackDelete(STStack *stack);

/*Funkce hledajici tabulku v zasobniku podle klice*/
symTableItem *STStackSearch(STStack *stack, char *key, bool *global);


#endif
