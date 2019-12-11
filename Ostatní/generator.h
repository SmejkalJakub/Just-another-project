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

void generateHeader();
void addInstruction(char *instruction);
void generateVariableDef(char *varName, int frame);
void setDynamicString(DS *dString);
void generateMoveVariableToVariable(char *toVar, char *fromVar, int toFrame, int fromFrame);
void generateMoveValueToVariable(char *toVar, int toFrame, char *type, char *value);
void generateWrite(symTableItem *variable, int frame, char *funcName);
void generateWriteValue(tokenStruct *token);
void generateConcatenateString(bool firstConcat);
void generateRead(char *var, int frame, int type);
void generateLabel(char *labelName);
void generateCall(char *labelName);
void generateFunctionEnd(symTableItem *functionName);
void generateFunctionStart(symTableItem *functionName);
void generateFunctionReturn(symTableItem *functionName, bool empty);
void generateJump(char *functionName);
void generateStackPush(tokenStruct *token, bool global);
void generateExpresion(int exprRule, char *functionName);
void generateIfStart(int numberOfPrevIfs, symTableItem *functionName);
void generateElseStart(int numberOfPrevIfs, symTableItem *functionName);
void generateElseEnd(int numberOfPrevIfs, symTableItem *functionName);
void generateWhileLabel(int numberOfPrevWhiles, symTableItem *functionName);
void generateWhileStart(int numberOfPrevWhiles, symTableItem *functionName);
void generateWhileEnd(int numberOfPrevWhiles, symTableItem *functionName);
void generateFunctionParamsPass(int paramNumber, tokenStruct *paramToken, bool global);
void generateFunctionDeclarePassedParams(int paramNumber, char *paramName);
void generateSaveLastExpresionValue();
void generateDynamicCheck(char *funcName, int nextOperatorType, int operandNumber, int exprRule);
void generateDynamicCheckTwoNones(char *funcName, int exprRule);


void generateThirdOperandToDouble();
void generateThirdOperandToInteger();
void generateFirstOperandToDouble();
void generateFirstOperandToInteger();

void generateFunctionLen();
void generateFunctionOrd();
void generateFunctionChr();
void generateFunctionSubstr();



#endif
