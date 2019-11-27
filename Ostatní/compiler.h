#include <stdbool.h>

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

}CompilerData;
