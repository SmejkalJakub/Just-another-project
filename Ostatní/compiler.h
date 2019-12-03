#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdbool.h>

#include "stack.h"
#include "generator.h"
#include "dynamicString.h"
#include "symtable.h"
#include "fileScanner.h"
#include "expr.h"
#include "symTableStack.h"




typedef struct {
    symTable localTable;
    symTable globalTable;

    STStack *tablesStack;

    tokenStruct token;

    char varToAssign[10];

    char *current_id;

    bool global;

    bool inFunction;
	bool inDefinition;
	bool inWhileOrIf;

}CompilerData;


int compilerDataInit(CompilerData* compilerData);
static int Prog (CompilerData *compilerData);
static int Prikaz (CompilerData *compilerData);
static int Hodnoty(CompilerData *compilerData);
static int Hodnota(CompilerData *compilerData);
static int anotherCommand (CompilerData *compilerData);
static int navratHodnoty (CompilerData *compilerData);
static int Parametry(CompilerData *compilerData);
static int dalsiParametr (CompilerData *compilerData);
static int listCommandFunction();
static int volaniNeboPrirazeni(CompilerData *compilerData);
static int prikazySekv(CompilerData *compilerData);
static int dalsiPrikaz(CompilerData *compilerData);
static int fceDefNeboVest(CompilerData *compilerData);

#endif

