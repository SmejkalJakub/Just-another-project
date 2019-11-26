#include <stdbool.h>

#include "symtable.h"
#include "fileScanner.h"

typedef struct {
    symbolTable localTable;
    symbolTable globalTable;

    tokenStruct token;

    ItemOfSymboltable *current_id;

    bool inFunction;
	bool inDeclaratin;	
	bool inWhileOrIf;
	bool nonDeclaredFunction;


}CompilerData;