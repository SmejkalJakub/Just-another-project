#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#include <stdbool.h>

#include "stack.h"
#include "generator.h"
#include "dynamicString.h"
#include "symtable.h"

typedef struct {
    symTable localTable;
    symTable globalTable;
    DS* printedValues;

    tokenStruct token;

    symTableItem *current_id;

    bool inFunction;
	bool inDefinition;
	bool inWhileOrIf;

<<<<<<< HEAD
}CompilerData;


int compilerDataInit(CompilerData* compilerData);
static int Prog (CompilerData *compilerData);
static int Commands (CompilerData *compilerData);
static int Values(CompilerData *compilerData);
static int Value(CompilerData *compilerData);
static int anotherCommand (CompilerData *compilerData);
static int commandValue (CompilerData *compilerData);
static int Params(CompilerData *compilerData);
static int anotherParam (CompilerData *compilerData);
static int listCommandFunction();
#endif
=======
}CompilerData;


int compilerDataInit(CompilerData* compilerData);
static int Prog (CompilerData *compilerData);
static int Commands (CompilerData *compilerData);
static int Values(CompilerData *compilerData);
static int Value(CompilerData *compilerData);
static int anotherCommand (CompilerData *compilerData);
static int commandValue (CompilerData *compilerData);
static int Params(CompilerData *compilerData);
static int anotherParam (CompilerData *compilerData);
static int listCommandFunction();
#endif

>>>>>>> dc3f2f60b3df455f05dd4682dd80057919fc85ab
