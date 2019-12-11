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

/*Struktura Dynamic Stringu*/
typedef struct dynamicStr {
    char *str;                  //samotny string
    unsigned int maxLength;     //maximalni delka
    unsigned int actIndex;      //aktualni index
}DS;

/*Inicializace DS*/
void DSInit(DS *dynamicStr);

/*Pridani znaku do DS*/
bool DSAddChar(DS *dynamicStr, char character);

/*Pridani celeho stringu do DS*/
bool DSAddStr(DS *dynamicStr, char *str);

/*Kopirovani jednoho DS do druheho DS*/
bool DSCopyStr(DS *dest, DS *src);

/*Porovnani dvou DS*/
int DSCmpToDS(DS *dynamicStr1, DS *dynamicStr2);

/*Porovnani DS s retezcem*/
int DSCmpToStr(DS *dynamicStr, char *str);

/*Ziskani retezce ze struktury DS*/
char *DSGetStr(DS *dynamicStr);

/*Ziskani delky retezce DS*/
unsigned int DSGetLength(DS *dynamicStr);

/*Reinicializase hodnot DS*/
void DSClear(DS *dynamicStr);

/*Smazani DS*/
void DSDelete(DS *dynamicStr);

#endif
