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
    addInstruction(".IFJcode19\n\n");
    addInstruction("DEFVAR GF@%convertHelpVar0\n");
    addInstruction("DEFVAR GF@%convertHelpVar1\n\n");

    addInstruction("DEFVAR GF@%conditionRes\n");
    addInstruction("DEFVAR GF@%lastExpresionResult\n\n");

}

void generateThirdOperandToDouble()
{
    addInstruction("INT2FLOATS\n");
}

void generateThirdOperandToInteger()
{
    addInstruction("FLOAT2INTS\n");
}

void generateFirstOperandToDouble()
{
    addInstruction("POPS GF@%convertHelpVar0\n");
    addInstruction("INT2FLOATS\n");
    addInstruction("PUSHS GF@%convertHelpVar0\n");
}

void generateFirstOperandToInteger()
{
    addInstruction("POPS GF@%convertHelpVar0\n");
    addInstruction("FLOAT2INTS\n");
    addInstruction("PUSHS GF@%convertHelpVar0\n");    
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
    else if(exprRule == EXPR_DIV)
    {
        addInstruction("DIVS\n");
    }
    else if(exprRule == EXPR_EQ)
    {
        addInstruction("EQS\n");
    }
    else if(exprRule == EXPR_NOT_EQ)
    {
        addInstruction("EQS\n");
        addInstruction("NOTS\n");
    }
    else if(exprRule == EXPR_MORE)
    {
        addInstruction("GTS");
    }
    else if(exprRule == EXPR_LESS)
    {
        addInstruction("LTS");
    }
    
    
}

void generateConcatenateString(bool firstConcat)
{
    if(firstConcat)
    {
        addInstruction("POPS GF@%convertHelpVar0\n");
        addInstruction("POPS GF@%convertHelpVar1\n");
        
        addInstruction("CONCAT GF@%lastExpresionResult GF@%convertHelpVar1 GF@%convertHelpVar0\n");
    }
    else
    {
        addInstruction("POPS GF@%convertHelpVar0\n");
        addInstruction("CONCAT GF@%lastExpresionResult GF@%lastExpresionResult GF@%convertHelpVar0\n");
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
        sprintf(str, "%a", token->doubleValue);
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
        sprintf(str, "%a", token->doubleValue);
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

void generateSaveLastExpresionValue()
{
    addInstruction("POPS GF@%lastExpresionResult\n");
}

void generateWriteValue(int type, char *value)
{
    char *typeStr;
    addInstruction("WRITE ");
    if(type == INT)
    {
        typeStr = "int@";
    }
    else if(type == DOUBLE)
    {
        typeStr = "float@";
    }
    else if(type == STRING)
    {
        typeStr = "string@";
    }
    addInstruction(typeStr);
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

