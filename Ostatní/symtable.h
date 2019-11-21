#include <stdio.h>
#include "stack.h"
#include <stdbool.h>
#include <dynamicString.h>


typedef enum
{
    typeUndefined,
    typeIntiger,
    typeDouble, 
    typeString,
} DataType; 

typedef struct{
    dataType Type;
    bool defined;
    DS *parametrs;
    char *nameID;
    bool globalVariable;
}SymbolTableData;

typedef struct{
    char *itemKey;
    TData Data;
    itemSymtable *next;

}ItemOfSymboltable;

typedef ItemOfSymboltable *symbolTable [66666];