#include <stdbool.h>

#include "symtable.h"
#include "fileScanner.h"

typedef struct {
    symTable localTable;
    symTable globalTable;

    tokenStruct token;

    symTableItem *current_id;

    bool inFunction;
	bool inDeclaratin;	
	bool inWhileOrIf;
	bool nonDeclaredFunction;


}CompilerData;