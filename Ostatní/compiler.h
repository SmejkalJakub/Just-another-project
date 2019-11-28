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
=======
>>>>>>> 7c7e470c45f818eba2fb2caec290fdf2dc5a5f1b

}CompilerData;

<<<<<<< HEAD
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

>>>>>>> 7c7e470c45f818eba2fb2caec290fdf2dc5a5f1b
