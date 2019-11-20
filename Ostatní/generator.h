#include <stdlib.h>
#include <stdio.h>
#include "dynamicString.h"
#include "fileScanner.h"

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
void generateWriteValue(char *type, char *value);
void generateRead(char *var, int frame, char *type);
void generateLabel(char *labelName);
void generateCall(char *labelName);
void generateFunctionEnd(char *functionName);
void generateFunctionStart(char *functionName);
void generateFunctionReturn(char *functionName, tokenStruct *token);
void generateJump(char *functionName);

