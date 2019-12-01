#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdbool.h>

#include "stack.h"
#include "generator.h"
#include "dynamicString.h"
#include "symtable.h"
#include "expr.h"



typedef struct {
    symTable localTable;
    symTable globalTable;


    Stack* IndentationStack;

    tokenStruct token;

    char *current_id;

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
static int prikazyFunkce(CompilerData * compilerData);
static int prikazySekv(CompilerData *compilerData);
static int dalsiPrikaz(CompilerData *compilerData);

#endif

