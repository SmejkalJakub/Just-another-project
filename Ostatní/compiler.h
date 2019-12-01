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
    DS* printedValues;

    Stack* IndentationStack;

    tokenStruct token;

    char *current_id;

    bool inFunction;
	bool inDefinition;
	bool inWhileOrIf;

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
static int callOrAssign(CompilerData *compilerData);

#endif

