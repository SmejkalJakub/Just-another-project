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
void generateWrite(char *variable, int frame);
void generateWriteValue(tokenStruct *token);
void generateConcatenateString(bool firstConcat);
void generateRead(char *var, int frame, int type);
void generateLabel(char *labelName);
void generateCall(char *labelName);
void generateFunctionEnd(char *functionName);
void generateFunctionStart(char *functionName);
void generateFunctionReturn(char *functionName, bool empty);
void generateJump(char *functionName);
void generateStackPush(tokenStruct *token, bool global);
void generateExpresion(int exprRule);
void generateIfStart(int numberOfPrevIfs, char *functionName);
void generateElseStart(int numberOfPrevIfs, char *functionName);
void generateElseEnd(int numberOfPrevIfs, char *functionName);
void generateWhileLabel(int numberOfPrevWhiles, char *functionName);
void generateWhileStart(int numberOfPrevWhiles, char *functionName);
void generateWhileEnd(int numberOfPrevWhiles, char *functionName);
void generateFunctionParamsPass(int paramNumber, tokenStruct *paramToken);
void generateSaveLastExpresionValue();

void generateThirdOperandToDouble();
void generateThirdOperandToInteger();
void generateFirstOperandToDouble();
void generateFirstOperandToInteger();

void generateFunctionLen();
void generateFunctionOrd();
void generateFunctionChr();
void generateFunctionSubstr();


#endif
