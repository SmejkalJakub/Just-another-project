/*
*Implementace p�eklada�e imperativn�ho jazyka IFJ19
*
*
*Dominik Nejedly (xnejed09)
*Jakub Smejkal (xsmejk28)
*
*
*Generator vysledneho kodu
*
*FIT VUT BRNO
*/
#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "dynamicString.h"
#include "fileScanner.h"
#include "expr.h"
#include "symtable.h"

#define LOCAL_VAR 0
#define GLOBAL_VAR 1
#define TEMP_VAR 2


/*Funkce pro vygenerovani hlavicky*/
void generateHeader();

/*Funkce pro pridani instrukce*/
void addInstruction(char *instruction);

/*Funkce pro zadefinovani nove promenne*/
void generateVariableDef(char *varName, int frame);

/*Funkce pro nastaveni DS*/
void setDynamicString(DS *dString);

/*Funkce pro presun promenne do druhe*/
void generateMoveVariableToVariable(char *toVar, char *fromVar, int toFrame, int fromFrame);

/*Funkce pro presunuti hodnoty z jedne promenne do druhe*/
void generateMoveValueToVariable(char *toVar, int toFrame, char *type, char *value);

/*Funkce pro vypis*/
void generateWrite(symTableItem *variable, int frame, char *funcName);

/*Funkce pro vygenerovani zapisovane hodnoty ziskane z tokenu*/
void generateWriteValue(tokenStruct *token);

/*Funkce pro spojeni dvou retezcu*/
void generateConcatenateString(bool firstConcat);

/*Funkce pro cteni*/
void generateRead(char *var, int frame, int type);

/*Funkce pro vygenerovani labelu*/
void generateLabel(char *labelName);

/*Funkce pro vygenerovani instrukce CALL*/
void generateCall(char *labelName);

/*Funkce pro vygenerovani konce funkce*/
void generateFunctionEnd(symTableItem *functionName);

/*Funkce pro vygenerovani zacatku funkce*/
void generateFunctionStart(symTableItem *functionName);

/*Funkce pro vygenerovani RETURNu funkce*/
void generateFunctionReturn(symTableItem *functionName, bool empty);

/*Funkce pro vygenerovani JUMPu*/
void generateJump(char *functionName);

/*Vygenerovani PUSHnuti na zasobnik*/
void generateStackPush(tokenStruct *token, bool global);

/*Vygenerovani vyrazu podle pravidla*/
void generateExpresion(int exprRule, char *functionName);

/*Začatek vetve IF*/
void generateIfStart(int numberOfPrevIfs, symTableItem *functionName);

/*Funkce pro vygenerovani zacatku WHILE*/
void generateElseStart(int numberOfPrevIfs, symTableItem *functionName);

/*Funkce pro vygenerovani konce ELSE*/
void generateElseEnd(int numberOfPrevIfs, symTableItem *functionName);

/*Generovani LABELu WHILE*/
void generateWhileLabel(int numberOfPrevWhiles, symTableItem *functionName);

/*Generovani zacatku while*/
void generateWhileStart(int numberOfPrevWhiles, symTableItem *functionName);

/*Generovani konce WHILE*/
void generateWhileEnd(int numberOfPrevWhiles, symTableItem *functionName);

/*PASSnuti parametru funkce*/
void generateFunctionParamsPass(int paramNumber, tokenStruct *paramToken, bool global);

/*Funkce pro parametry deklarovanych u funkce */
void generateFunctionDeclarePassedParams(int paramNumber, char *paramName);

/*Generovani ulozeni vysledku posledniho vyrazu*/
void generateSaveLastExpresionValue();

/*Generovani dynamicke kontroly*/
void generateDynamicCheck(char *funcName, int nextOperatorType, int operandNumber, int exprRule);

/*Generovani dynamicke kontroly tvou None*/
void generateDynamicCheckTwoNones(char *funcName, int exprRule);

/*Funkce pretypovani tretiho operandu*/
void generateThirdOperandToDouble();

/*Funkce pretypovani prvniho operandu*/
void generateFirstOperandToDouble();

/*Vestavene funkce*/
void generateFunctionLen();
void generateFunctionOrd();
void generateFunctionChr();
void generateFunctionSubstr();



#endif
