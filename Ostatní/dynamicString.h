#ifndef DYNAMICSTRING_H_INCLUDED
#define DYNAMICSTRING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CHUNK 20

typedef struct dynamicStr {
    char *str;
    unsigned int maxLenght;
    unsigned int actIndex;
}DS;

void DSInit(DS *dynamicStr);
bool DSAddChar(DS *dynamicStr, char character);
bool DSAddStr(DS *dynamicStr, char *str);
//bool DSAddStr(DS *dynamicStr, char *str);
void DSDelete(DS *dynamicStr);

#endif
