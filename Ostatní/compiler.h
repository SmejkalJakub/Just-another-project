#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdbool.h>

#include "stack.h"
#include "generator.h"
#include "dynamicString.h"
#include "symtable.h"
#include "expr.h"


#define LEX_ERROR 1
#define SYNTAX_ERROR 2
#define SEM_ERROR_DEF 3
#define SEM_ERROR_COMPATIBILITY 4
#define SEM_ERROR_PARAMS 5
#define SEM_ERROR 6
#define SEM_ERROR_DIV_ZERO 9
#define INTERNAL_ERROR 99


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

