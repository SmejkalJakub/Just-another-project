#include "generator.h"

DS *dynamicString;

void setDynamicString(DS *dString)
{
    dynamicString = dString;
}

void addInstruction(char *instruction)
{
    DSAddStr(dynamicString, instruction);
}


void generateHeader()
{
    addInstruction(".IFJcode19\n");
    addInstruction("DEFVAR GF@%conditionRes\n");
    addInstruction("DEFVAR GF@%lastExpresionResult\n\n\n");

}

void generateExpresion(int exprRule)
{
    if(exprRule == EXPR_PLUS)
    {
        addInstruction("ADDS\n");
    }
    else if(exprRule == EXPR_MINUS)
    {
        addInstruction("SUBS\n");
    }
    else if(exprRule == EXPR_MUL)
    {
        addInstruction("MULS\n");
    }
    
}

void generateStackPush(tokenStruct *token)
{
    addInstruction("PUSHS ");
    if(token->tokenType == TOKEN_INTEGER)
    {
        char str[12];
        sprintf(str, "%d", token->integerValue);
        addInstruction("int@");
        addInstruction(str);
    }
    else if(token->tokenType == TOKEN_DOUBLE)
    {
        char str[30];
        sprintf(str, "%f", token->doubleValue);
        addInstruction("float@");
        addInstruction(str);
    }
    else if(token->tokenType == TOKEN_STRING)
    {
        addInstruction("string@");
        addInstruction(token->stringValue->str);
    }
    else if(token->tokenType == TOKEN_IDENTIFIER)
    {
        addInstruction("LF@");
        addInstruction(token->stringValue->str);
    }

    addInstruction("\n");
}

void generateFunctionStart(char *functionName)
{
    addInstruction("LABEL $");
    addInstruction(functionName);
    addInstruction("\n");
    addInstruction("PUSHFRAME\n");
}

void generateFunctionEnd(char *functionName)
{
    addInstruction("LABEL $");
    addInstruction(functionName);
    addInstruction("$exitFunction");
    addInstruction("\n");
    addInstruction("POPFRAME\n");
    addInstruction("RETURN\n");
}

void generateFunctionReturn(char *functionName, tokenStruct *token)
{
    addInstruction("MOVE LF@%retval ");
    if(token->tokenType == TOKEN_INTEGER)
    {
        char str[12];
        sprintf(str, "%d", token->integerValue);
        addInstruction("int@");
        addInstruction(str);
    }
    else if(token->tokenType == TOKEN_DOUBLE)
    {
        char str[30];
        sprintf(str, "%f", token->doubleValue);
        addInstruction("float@");
        addInstruction(str);
    }
    else if(token->tokenType == TOKEN_STRING)
    {
        addInstruction("string@");
        addInstruction(token->stringValue->str);
    }
    else if(token->tokenType == TOKEN_IDENTIFIER)
    {
        addInstruction("LF@");
        addInstruction(token->stringValue->str);
    }

    addInstruction("\nJUMP $");
    addInstruction(functionName);
    addInstruction("$exitFunction\n");

}

void generateConvert(char *inVar, char *outVar)
{

}

void generateVariableDef(char *varName, int frame)
{
    char *varFrame = "GF@";
    if(frame == LOCAL_VAR)
    {
        varFrame = "LF@";
    }
    else if(frame == TEMP_VAR)
    {
        varFrame = "TF@";
    }

    addInstruction("DEFVAR ");
    addInstruction(varFrame);
    addInstruction(varName);
    addInstruction("\n");
}

void generateMoveVariableToVariable(char *toVar, char *fromVar, int toFrame, int fromFrame)
{
    char *fromVarFrame = "";
    char *toVarFrame = "";

    if(fromFrame == LOCAL_VAR)
    {
        fromVarFrame = "LF@";
    }
    else
    {
        fromVarFrame = "GF@";
    }

    if(toFrame == LOCAL_VAR)
    {
        toVarFrame = "LF@";
    }
    else
    {
        toVarFrame = "GF@";
    }

    addInstruction("MOVE ");
    addInstruction(toVarFrame);
    addInstruction(toVar);
    addInstruction(" ");
    addInstruction(fromVarFrame);
    addInstruction(fromVar);
    addInstruction("\n");
}

void generateMoveValueToVariable(char *toVar, int toFrame, char *type, char *value)
{
    char *toVarFrame = "";

    if(toFrame == LOCAL_VAR)
    {
        toVarFrame = "LF@";
    }
    else
    {
        toVarFrame = "GF@";
    }

    addInstruction("MOVE ");
    addInstruction(toVarFrame);
    addInstruction(toVar);
    addInstruction(" ");
    addInstruction(type);
    addInstruction("@");
    addInstruction(value);
    addInstruction("\n");
}

void generateWrite(char *variable, int frame)
{
    char *frameVar = "";

    if(frame == LOCAL_VAR)
    {
        frameVar = "LF@";
    }
    else
    {
        frameVar = "GF@";
    }

    addInstruction("WRITE ");
    addInstruction(frameVar);
    addInstruction(variable);
    addInstruction("\n");
}

void generatePrint(char *string)
{
    addInstruction("WRITE string@");
    addInstruction(string);
    addInstruction("\n");
}

void generateWriteValue(char *type, char *value)
{
    addInstruction("WRITE ");
    addInstruction(type);
    addInstruction("@");
    addInstruction(value);
    addInstruction("\n");
}

void generateRead(char *var, int frame, char *type)
{
    addInstruction("WRITE string@Hodnota: \n");
    char *frameVar = "";

    if(frame == LOCAL_VAR)
    {
        frameVar = "LF@";
    }
    else
    {
        frameVar = "GF@";
    }

    if(strcmp("i", type) == 0)
    {
        type = "int";
    }
    else if(strcmp("f", type) == 0)
    {
        type = "float";
    }
    else if(strcmp("s", type) == 0)
    {
        type = "string";
    }
    else
    {
        type = "bool";
    }

    addInstruction("READ ");
    addInstruction(frameVar);
    addInstruction(var);
    addInstruction(" ");
    addInstruction(type);
    addInstruction("\n");
}

void generateLabel(char *labelName)
{
    addInstruction("LABEL $");
    addInstruction(labelName);
    addInstruction("\n");
}

void generateCall(char *labelName)
{
    addInstruction("CALL $");
    addInstruction(labelName);
    addInstruction("\n");
}

void generateReturn()
{
    addInstruction("RETURN");
    addInstruction("\n");
}

void generateJump(char *functionName)
{
    addInstruction("JUMP $");
    addInstruction(functionName);
    addInstruction("\n");
}

