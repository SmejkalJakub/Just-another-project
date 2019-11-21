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
} dataType; 

typedef struct{
    dataType Type;
    bool defined;
    DS *parametrs;
    char *nameID;
    bool globalVariable;
}TData;

typedef struct{
    char *itemKey;
    TData Data;
    itemSymtable *next;

}itemSymtable