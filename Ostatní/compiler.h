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
    symTable *localTable;
    symTable *globalTable;

    STStack *tablesStack;

    tokenStruct token;

    symTableItem *varToAssign;

    symTableItem *current_function;

    int numberOfIfs;
    int numberOfWhiles;

    bool global;

    bool inFunction;
	bool inDefinition;
	bool inWhileOrIf;

    Stack *indentationStack;

}CompilerData;


bool compilerDataInit(CompilerData* compilerData);
int divideAndConquer();

#endif

