/*
*Implementace pøekladaèe imperativního jazyka IFJ19
*
*Jakub Smejkal (xsmejk09)
*Adam Grunwald (xgrunw00)
*
*
*Expression
*
*FIT VUT BRNO
*/


#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED

#include <stdbool.h>
#include "generator.h"
#include "symtable.h"
#include "symTableStack.h"

#define EQ 0
#define S 1
#define R 2
#define ER 3


#define TABLE_SIZE 7


#define EXPR_ERR -1
#define EXPR_PLUS 0
#define EXPR_MINUS 1
#define EXPR_MUL 2
#define EXPR_DIV 3
#define EXPR_SPEC_DIV 4
#define EXPR_LESS 5
#define EXPR_MORE 6
#define EXPR_LESS_EQ 7
#define EXPR_MORE_EQ 8
#define EXPR_NOT_EQ 9
#define EXPR_EQ 10
#define EXPR_ASSIGN 11
#define EXPR_E 12
#define EXPR_ID 13
#define BRACKET_NONTERM_BRACKET 14


#define PREC_TAB_PLUS_MINUS 0
#define PREC_TAB_MUL_DIV_IDIV 1
#define PREC_TAB_RELATION_OPERATOR 2
#define PREC_TAB_ID 3
#define PREC_TAB_LEFT_BRACKET 4
#define PREC_TAB_RIGHT_BRACKET 5
#define PREC_TAB_DOLAR 6

/*Vycet moznych typu operandu ve vyrazu*/
typedef enum
{
    SYM_PLUS,
    SYM_MINUS,
    SYM_MUL,
    SYM_DIV,
    SYM_SPEC_DIV,
    SYM_ASSIGN,
    SYM_LESS,
    SYM_MORE,
    SYM_LESS_EQ,
    SYM_MORE_EQ,
    SYM_EQ,
    SYM_NOT_EQ,
    SYM_LEFT_BRACKET,
    SYM_RIGHT_BRACKET,
    SYM_ID,
    SYM_INT,
    SYM_DOUBLE,
    SYM_STRING,
    SYM_NONE,
    SYM_DOLAR,
    SYM_STOP,
    SYM_NON_TERM

} precedenceTabSym;

/*Vraci polozky po zarazku*/
bool getitemsBeforeStop(int *itemsBeforeStop);

/*Funkce resici vyraz*/
int solveExpr(tokenStruct *token, STStack *symTableStack, symTableItem *assignVar, symTableItem *currentFunction);

/*Zkontroluje operand a pripadne pretypuje*/
int checkAndRetype(symStackItem* operand1, symStackItem* operand2, symStackItem* operand3, int rule, int *finalType);

/*Funkce vracejici typ tokenu*/
int getTokenType(tokenStruct *token, STStack *symTableStack);

/*Funkce prevadejici symbol na typ symbolu z precedencni tabulky*/
int symbolToType(precedenceTabSym symbol);

/*Funkce pro ziskani termu z vrcholu zasobniku*/
symStackItem *getTopTerm();

/*Funcke vracejici polozky po zarazku*/
bool getitemsBeforeStop(int *itemsBeforeStop);

/*Funkce kontrolujici pravidlo vyrazu*/
int checkExprRule(symStackItem *firstItem, symStackItem *secondItem, symStackItem *thirdItem, int numberOfItems);

/*Funkce redukce vyrazu*/
int reduce();

/*Funkce shiftnuti*/
void shift(precedenceTabSym currentSym, tokenStruct *token, STStack *symTableStack);

#endif
