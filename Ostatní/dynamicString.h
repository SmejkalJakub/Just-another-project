/*
*Implementace pøekladaèe imperativního jazyka IFJ19
*
*Dominik Nejedly (xnejed09)
*
*
*Dynamic String
*
*FIT VUT BRNO
*/
#ifndef DYNAMICSTRING_H_INCLUDED
#define DYNAMICSTRING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CHUNK 20

typedef struct dynamicStr {
    char *str;
    unsigned int maxLength;
    unsigned int actIndex;
}DS;

void DSInit(DS *dynamicStr);
bool DSAddChar(DS *dynamicStr, char character);
bool DSAddStr(DS *dynamicStr, char *str);
bool DSCopyStr(DS *dest, DS *src);
int DSCmpToDS(DS *dynamicStr1, DS *dynamicStr2);
int DSCmpToStr(DS *dynamicStr, char *str);
char *DSGetStr(DS *dynamicStr);
unsigned int DSGetLength(DS *dynamicStr);
void DSClear(DS *dynamicStr);
void DSDelete(DS *dynamicStr);

#endif
