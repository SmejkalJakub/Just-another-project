/*
*Implementace pøekladaèe imperativního jazyka IFJ19
*
*Dominik Nejedly (xnejed09)
*
*
*Rozptylena tabulka symbolu
*
*FIT VUT BRNO
*/


#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include "dynamicString.h"

#define TYPE_NONE 0
#define INT 1
#define DOUBLE 2
#define STRING 3
#define BOOL 4
#define SPEC_TYPE_NONE 5
#define RETURN_TYPE 6
#define EMPTY_TYPE -99

#define ST_SIZE 65003

/*
*Struktura polozky tabulky
*/
typedef struct hashTableItem{
    char *key;                      //klic polozky
    int type;                       //typ polozky
    int numberOfParams;             //pocet parametru
    DS *params;                     //DS obsahujici vycet parametru
    bool function;                  //zda se jedna o funkci
    bool defined;                   //zda je polozka definovana
    struct hashTableItem *next;     //ukazatel na dalsi polozku
} symTableItem;


/*Tabulka*/
typedef symTableItem *symTable[ST_SIZE];

/*Funkce pro inicializaci tabulky*/
void STInit(symTable *STptr);

/*Funkce vracejici ukazatel na polozku z tabulky symbolu nalezenou podle klice*/
symTableItem *STSearch(symTable* STptr, char *key);

/*Funkce pro vlozeni nove polozky do tabulky*/
symTableItem *STInsert(symTable* STptr, char *key);

/*Funkce pro pridani parametru k polozce*/
bool STAddParam(symTableItem *item, int type);

/*Funkce pro odstraneni polozky z tabulek symbolu*/
void STDeleteItem(symTable *STptr, char *key);

/*Funkce pro smazani tabulky symbolu*/
void STDelete(symTable *STptr);

#endif
