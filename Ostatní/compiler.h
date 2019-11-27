#include <stdbool.h>

#include "symtable.h"
#include "fileScanner.h"

typedef struct {
    symTable localTable;
    symTable globalTable;
    Stack indentationStack;

    tokenStruct token;

    symTableItem *current_id;

    bool inFunction;
	bool inDefinition;	
	bool inWhileOrIf;

}CompilerData;