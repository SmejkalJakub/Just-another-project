#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdbool.h>

#include "stack.h"
#include "generator.h"
#include "dynamicString.h"
#include "symtable.h"
#include "fileScanner.h"
/*
*Implementace pøekladaèe imperativního jazyka IFJ19
*
*Dominik Nejedly (xnejed09)
*Jakub Smejkal (xsmejk28)
*Adam Grunwald (xgrunw00)
*
*
*Compiler
*
*FIT VUT BRNO
*/
#include "expr.h"
#include "symTableStack.h"



/*Struktura samotneho compileru obsahujici data potrebna pro spravny beh*/
typedef struct {
    symTable *localTable;                   //Lokalni tabulka symbolu
    symTable *globalTable;                  //Globalni tabulka symbolu

    STStack *tablesStack;                   //Zasobnik tabulek symbolu

    tokenStruct token;                      //Nacteny token

    symTableItem *varToAssign;              //Promenna k prirazeni

    symTableItem *current_function;         //Nazev funkce, ve ktere se prave nachazime

                                            /*slouzi pro tvorbu unikatniho ID labelu*/
    int numberOfIfs;                        //Pocet IFu, kterymi jsme prozatim prosli
    int numberOfWhiles;                     //Pocet WHILu, kterymi jsme prozatim prosli

    bool global;                            //zdali jsme v globalnim ramci

    bool inFunction;                        //zdali jsme ve funkci
	bool inDefinition;                      //zdali jsme v definici
	bool inWhileOrIf;                       //zdali jsme v IFu ci WHILE

    Stack *indentationStack;                //Zasobnik Indentu

}CompilerData;

/*Funkce pro inicializaci dat Compileru*/
int compilerDataInit(CompilerData* compilerData);

/*FUNKCE REPREZENTUJICI JEDNOTLIVE NONTERMY*/
static int Prog (CompilerData *compilerData);
static int Prikaz (CompilerData *compilerData);
static int Hodnoty(CompilerData *compilerData);
static int Hodnota(CompilerData *compilerData);
static int navratHodnoty (CompilerData *compilerData);
static int Parametry(CompilerData *compilerData);
static int dalsiParametr (CompilerData *compilerData);
static int volaniNeboPrirazeni(CompilerData *compilerData);
static int prikazySekv(CompilerData *compilerData);
static int dalsiPrikaz(CompilerData *compilerData);
static int fceDefNeboVest(CompilerData *compilerData);

#endif

