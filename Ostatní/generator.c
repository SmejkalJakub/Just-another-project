#include "generator.h"

DS *dynamicString;

int numberOfUses = 0;

void setDynamicString(DS *dString)
{
    dynamicString = dString;
}

void addInstruction(char *instruction)
{
    if(!DSAddStr(dynamicString, instruction))
    {

    }
}


void generateHeader()
{
    addInstruction(".IFJcode19\n\n");
    addInstruction("DEFVAR GF@%convertHelpVar0\n");
    addInstruction("DEFVAR GF@%convertHelpVar1\n\n");

    addInstruction("DEFVAR GF@%lastExpresionResult\n\n");

    generateJump("afterBuildInFunctions\n");

    generateFunctionChr();
    generateFunctionLen();
    generateFunctionOrd();
    generateFunctionSubstr();

    generateLabel("afterBuildInFunctions");

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
    else if(exprRule == EXPR_SPEC_DIV)
    {
        addInstruction("POPS GF@%convertHelpVar0\n");
        addInstruction("INT2FLOATS\n");
        addInstruction("PUSHS GF@%convertHelpVar0\n");
        addInstruction("INT2FLOATS\n");
        addInstruction("DIVS\n");
        addInstruction("FLOAT2INTS\n");
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
        addInstruction("GTS\n");
    }
    else if(exprRule == EXPR_LESS)
    {
        addInstruction("LTS\n");
    }
    else if(exprRule == EXPR_LESS_EQ)
    {
        addInstruction("POPS GF@%convertHelpVar0\n");
        addInstruction("POPS GF@%convertHelpVar1\n");
        addInstruction("PUSHS GF@%convertHelpVar1\n");
        addInstruction("PUSHS GF@%convertHelpVar0\n");
        addInstruction("LTS\n");
        addInstruction("PUSHS GF@%convertHelpVar1\n");
        addInstruction("PUSHS GF@%convertHelpVar0\n");
        addInstruction("EQS\n");
        addInstruction("ORS\n");

    }
    else if(exprRule == EXPR_MORE_EQ)
    {
        addInstruction("POPS GF@%convertHelpVar0\n");
        addInstruction("POPS GF@%convertHelpVar1\n");
        addInstruction("PUSHS GF@%convertHelpVar1\n");
        addInstruction("PUSHS GF@%convertHelpVar0\n");
        addInstruction("GTS\n");
        addInstruction("PUSHS GF@%convertHelpVar1\n");
        addInstruction("PUSHS GF@%convertHelpVar0\n");
        addInstruction("EQS\n");
        addInstruction("ORS\n");
    }
}


void generateIfStart(int numberOfPrevIfs, symTableItem *functionName)
{
    char str[12];
    sprintf(str, "%d", numberOfPrevIfs);


    addInstruction("JUMPIFNEQ ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$else%");


    addInstruction(str);
    addInstruction(" GF@%lastExpresionResult bool@true\n");
}

void generateElseStart(int numberOfPrevIfs, symTableItem *functionName)
{

    char str[12];
    sprintf(str, "%d", numberOfPrevIfs);

    addInstruction("JUMP ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$end%");
    addInstruction(str);
    addInstruction("\n");


    addInstruction("LABEL ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$else%");
    addInstruction(str);
    addInstruction("\n");
}

void generateElseEnd(int numberOfPrevIfs, symTableItem *functionName)
{

    char str[12];
    sprintf(str, "%d", numberOfPrevIfs);

    addInstruction("LABEL ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$end%");
    addInstruction(str);
    addInstruction("\n");
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


void generateStackPush(tokenStruct *token, bool global)
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
    else if(token->tokenType == TOKEN_KEYWORD && strcmp(token->stringValue->str, "None") == 0)
    {
        addInstruction("nil@nil");
    }
    else if(token->tokenType == TOKEN_IDENTIFIER && !global)
    {
        addInstruction("LF@");
        addInstruction(token->stringValue->str);
    }
    else
    {
        addInstruction("GF@");
        addInstruction(token->stringValue->str);
    }

    addInstruction("\n");
}

void generateFunctionParamsPass(int paramNumber, tokenStruct *paramToken, bool global)
{
    char str[12];
    sprintf(str, "%d", paramNumber);

    if(paramToken->tokenType == TOKEN_INTEGER || paramToken->tokenType == TOKEN_DOUBLE
    || paramToken->tokenType == TOKEN_STRING || paramToken->tokenType == TOKEN_IDENTIFIER)
    {
        addInstruction("DEFVAR TF@%");
        addInstruction(str);
        addInstruction("\n");

        addInstruction("MOVE TF@%");
        addInstruction(str);
        addInstruction(" ");
    }
    else
    {
        return;
    }


    if(paramToken->tokenType == TOKEN_INTEGER)
    {
        char str[12];
        sprintf(str, "%d", paramToken->integerValue);
        addInstruction("int@");
        addInstruction(str);
    }
    else if(paramToken->tokenType == TOKEN_DOUBLE)
    {
        char str[30];
        sprintf(str, "%a", paramToken->doubleValue);
        addInstruction("float@");
        addInstruction(str);
    }
    else if(paramToken->tokenType == TOKEN_STRING)
    {
        addInstruction("string@");
        addInstruction(paramToken->stringValue->str);
    }
    else if(paramToken->tokenType == TOKEN_IDENTIFIER)
    {
        if(global)
        {
            addInstruction("GF@");
        }
        else
        {
            addInstruction("LF@");
        }

        addInstruction(paramToken->stringValue->str);
    }


    addInstruction("\n");

}


void generateDynamicCheckTwoNones(char *funcName, int exprRule)
{
    char str[12];
    sprintf(str, "%d", numberOfUses);


    if(strcmp("", funcName) == 0)
    {
        addInstruction("CREATEFRAME\n");
        addInstruction("PUSHFRAME\n");
    }

    addInstruction("DEFVAR LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    addInstruction("POPS GF@%convertHelpVar0\n");
    addInstruction("POPS LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");
    addInstruction("PUSHS LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");
    addInstruction("PUSHS GF@%convertHelpVar0\n");

    addInstruction("TYPE LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    addInstruction("DEFVAR LF@%1");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    addInstruction("POPS LF@%1");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    addInstruction("PUSHS LF@%1");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    addInstruction("TYPE LF@%1");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type LF@%1");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    if(exprRule == EXPR_DIV)
    {
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type$true$float LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@float\n");

        addInstruction("JUMPIFNEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$error LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@int\n");

        generateFirstOperandToDouble();

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type$true$float\n");     
        
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$same LF@%1");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@float\n");

        addInstruction("JUMPIFNEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$error LF@%1");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@int\n");

        generateThirdOperandToDouble();

        addInstruction("JUMP $");
        addInstruction(funcName);
        addInstruction("$if$%");
        addInstruction(str);
        addInstruction("$true$same\n");
    }

    else if(exprRule == EXPR_SPEC_DIV)
    {

        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type$true$int LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@int\n");

        addInstruction("EXIT int@4\n");

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type$true$int\n");     
        
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$same LF@%1");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@int\n");

        addInstruction("EXIT int@4\n");
    }
    

    else if(exprRule == EXPR_PLUS)
    {
        addInstruction("EQ GF@%convertHelpVar0 ");
        addInstruction("LF@%0%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("LF@%1%");
        addInstruction(str);
        addInstruction("$type\n");

        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$same GF@%convertHelpVar0 ");
        addInstruction("bool@true\n");

        addInstruction("EQ GF@%convertHelpVar0 ");
        addInstruction("LF@%0%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@int\n");

        addInstruction("EQ GF@%convertHelpVar1 ");
        addInstruction("LF@%1%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@float\n");

        addInstruction("AND GF@%convertHelpVar0 GF@%convertHelpVar0 GF@%convertHelpVar1\n");
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$firstToDouble GF@%convertHelpVar0 ");
        addInstruction("bool@true\n");
        
        addInstruction("EQ GF@%convertHelpVar0 ");
        addInstruction("LF@%0%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@float\n");

        addInstruction("EQ GF@%convertHelpVar1 ");
        addInstruction("LF@%1%");
        addInstruction(str);
        addInstruction("$type ");
        addInstruction("string@int\n");

        addInstruction("AND GF@%convertHelpVar0 GF@%convertHelpVar0 GF@%convertHelpVar1\n");

        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$thirdToDouble GF@%convertHelpVar0 ");
        addInstruction("bool@true\n");


        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$thirdToDouble\n");
        generateThirdOperandToDouble();
        addInstruction("JUMP $");
        addInstruction(funcName);
        addInstruction("$if$%");
        addInstruction(str);
        addInstruction("$true$same\n");

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$firstToDouble\n");
        generateFirstOperandToDouble();
        addInstruction("JUMP $");
        addInstruction(funcName);
        addInstruction("$if$%");
        addInstruction(str);
        addInstruction("$true$same\n");

    }

    addInstruction("LABEL $");
    addInstruction(funcName);
    addInstruction("$if$");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$error\n");
    addInstruction("EXIT int@4\n");

    addInstruction("LABEL $");
    addInstruction(funcName);
    addInstruction("$if$%");
    addInstruction(str);
    addInstruction("$true$same\n");

    numberOfUses++;
}


void generateDynamicCheck(char *funcName, int nextOperatorType, int operandNumber, int exprRule)
{

    numberOfUses++;

    char str[12];
    sprintf(str, "%d", numberOfUses);
    printf("NUMBEROFUSES %s\n", str);

    if(strcmp("", funcName) == 0)
    {
        addInstruction("CREATEFRAME\n");
        addInstruction("PUSHFRAME\n");
    }
    addInstruction("DEFVAR LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");

    if(operandNumber == 1)
    {
        addInstruction("POPS GF@%convertHelpVar0\n");
        addInstruction("POPS LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type\n");
        addInstruction("PUSHS LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type\n");
        addInstruction("PUSHS GF@%convertHelpVar0\n");
    }
    else if(operandNumber == 3)
    {
        addInstruction("POPS LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type\n");

        addInstruction("PUSHS LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type\n");
    }

    addInstruction("TYPE LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type LF@%0");
    addInstruction("%");
    addInstruction(str);
    addInstruction("$type\n");
    

    if(nextOperatorType == INT)
    {
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$float LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type string@float\n");

        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$int LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type string@int\n");

        addInstruction("EXIT int@4\n");

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$float\n");

        if(exprRule == EXPR_SPEC_DIV)
        {
            addInstruction("EXIT int@4");
        }
        else if(exprRule == EXPR_PLUS || exprRule == EXPR_LESS)
        {
            if(operandNumber == 1)
            {
                generateThirdOperandToDouble();
            }
            else if(operandNumber == 3)
            {
                generateFirstOperandToDouble();
            }
        }
        

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$int\n");
        if(exprRule == EXPR_DIV)
        {
            if(operandNumber == 1)
            {
                generateFirstOperandToDouble();
            }
            else if(operandNumber == 3)
            {
                generateThirdOperandToDouble();
            }
        }
    }

    else if(nextOperatorType == STRING && exprRule == EXPR_PLUS)
    {
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$string LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type string@string\n");

        addInstruction("EXIT int@4\n");

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$string\n");
    }

    else if(nextOperatorType == DOUBLE)
    {
        if(exprRule == EXPR_SPEC_DIV)
        {
            addInstruction("EXIT int@4\n");
            return;
        }
        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$float LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type string@float\n");

        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$int LF@%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$type string@int\n");

        addInstruction("EXIT int@4\n");

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$int\n");

        if(operandNumber == 1)
        {
            generateFirstOperandToDouble();
        }
        else if(operandNumber == 3)
        {
            generateThirdOperandToDouble();
        }
        


        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$%0");
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$float\n");

    }
    if(strcmp("", funcName) == 0)
    {
        addInstruction("POPFRAME\n");
    }
}

void generateFunctionDeclarePassedParams(int paramNumber, char *paramName)
{
    char str[12];
    sprintf(str, "%d", paramNumber);

	addInstruction("DEFVAR LF@"); 
    addInstruction(paramName); 
    addInstruction("\n");

	addInstruction("MOVE LF@"); 
    addInstruction(paramName); 
    addInstruction(" LF@%"); 
    addInstruction(str);
    addInstruction("\n");
}

void generateFunctionLen()
{
    addInstruction("\n\nLABEL $len\n");
    addInstruction("PUSHFRAME\n");

    addInstruction("DEFVAR LF@%retval\n");

    addInstruction("STRLEN LF@%retval LF@%0\n");

    addInstruction("POPFRAME\n");
    addInstruction("RETURN\n\n\n");
}

void generateFunctionChr()
{
    addInstruction("\n\nLABEL $chr\n");
    addInstruction("PUSHFRAME\n");

    addInstruction("DEFVAR LF@%retval\n");

    addInstruction("INT2CHAR LF@%retval LF@%0\n");

    addInstruction("POPFRAME\n");
    addInstruction("RETURN\n\n\n");
}

void generateFunctionSubstr()
{
    addInstruction("\n\nLABEL $substr\n");
    addInstruction("PUSHFRAME\n");

    addInstruction("DEFVAR LF@%retval\n");
    addInstruction("MOVE LF@%retval string@\n");

    addInstruction("DEFVAR LF@result\n");
    addInstruction("DEFVAR LF@strLen\n");

    addInstruction("STRLEN LF@strLen LF@%0\n");

    addInstruction("LT LF@result LF@%2 int@0\n");
    addInstruction("JUMPIFEQ $substr$end LF@result bool@true\n");

    addInstruction("LT LF@result LF@%1 int@0\n");
    addInstruction("JUMPIFEQ $substr$end LF@result bool@true\n");

    addInstruction("GT LF@result LF@%1 LF@strLen\n");
    addInstruction("JUMPIFEQ $substr$end LF@result bool@true\n");

    addInstruction("EQ LF@result LF@%2 int@0\n");
    addInstruction("JUMPIFEQ $substr$end LF@result bool@true\n");

    addInstruction("EQ LF@result LF@strLen int@0\n");
    addInstruction("JUMPIFEQ $substr$end LF@result bool@true\n");

    addInstruction("DEFVAR LF@tempChar\n");
    addInstruction("DEFVAR LF@tempVar\n");
    addInstruction("SUB LF@tempVar LF@strLen LF@%1\n");

    addInstruction("LT LF@result LF@%2 LF@tempVar\n");
    addInstruction("JUMPIFEQ $substr$concat LF@result bool@true\n");

    addInstruction("SUB LF@%2 LF@strLen LF@%1\n");

    addInstruction("LABEL $substr$concat\n");

    addInstruction("EQ LF@result LF@%2 int@0\n");
    addInstruction("JUMPIFEQ $substr$end LF@result bool@true\n");

    addInstruction("GETCHAR LF@tempChar LF@%0 LF@%1\n");
    addInstruction("CONCAT LF@%retval LF@%retval LF@tempChar\n");

    addInstruction("ADD LF@%1 LF@%1 int@1\n");
    addInstruction("SUB LF@%2 LF@%2 int@1\n");

    addInstruction("JUMP $substr$concat\n");


    addInstruction("LABEL $substr$end\n");
    addInstruction("POPFRAME\n");
    addInstruction("RETURN\n\n\n");
}

void generateFunctionOrd()
{
    addInstruction("\n\nLABEL $ord\n");
    addInstruction("PUSHFRAME\n");

    addInstruction("DEFVAR LF@%retval\n");
    addInstruction("MOVE LF@%retval string@\n\n");

    addInstruction("DEFVAR LF@result\n");
    addInstruction("DEFVAR LF@strLen\n");

    addInstruction("STRLEN LF@strLen LF@%0\n");


    addInstruction("LT LF@result LF@%1 int@0\n");
    addInstruction("JUMPIFEQ $ord$end LF@result bool@true\n");

    addInstruction("SUB LF@strLen LF@strLen int@1\n");

    addInstruction("GT LF@result LF@%1 LF@strLen\n");
    addInstruction("JUMPIFEQ $ord$end LF@result bool@true\n");

    addInstruction("STRI2INT LF@%retval LF@%0 LF@%1\n");

    addInstruction("LABEL $ord$end\n");

    addInstruction("POPFRAME\n");
    addInstruction("RETURN\n\n\n");
}

void generateFunctionStart(symTableItem *functionName)
{
    static int labelCounter = 0;

    char str[13];
    sprintf(str, "l%d", labelCounter);

    generateJump(str);
    addInstruction("LABEL $");
    addInstruction(functionName->key);
    addInstruction("\n");
    addInstruction("PUSHFRAME\n");
    addInstruction("DEFVAR LF@%retval\n");
    addInstruction("MOVE LF@%retval nil@nil\n\n");

    labelCounter++;
}

void generateFunctionEnd(symTableItem *functionName)
{
    static int labelCounter = 0;

    char str[13];
    sprintf(str, "l%d", labelCounter);

    addInstruction("LABEL $");
    addInstruction(functionName->key);
    addInstruction("$exitFunction");
    addInstruction("\n");
    addInstruction("POPFRAME\n");
    addInstruction("RETURN\n");
    generateLabel(str);

    labelCounter++;
}

void generateFunctionReturn(symTableItem *functionName, bool empty)
{
    if(!empty)
    {
        addInstruction("MOVE LF@%retval GF@%lastExpresionResult");
    }
    else
    {
        addInstruction("MOVE LF@%retval string@");

    }

    /*if(token->tokenType == TOKEN_INTEGER)
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
    }*/

    addInstruction("\nJUMP $");
    addInstruction(functionName->key);
    addInstruction("$exitFunction\n");

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
    else if(fromFrame == GLOBAL_VAR)
    {
        fromVarFrame = "GF@";
    }
    else
    {
        fromVarFrame = "TF@";
    }


    if(toFrame == LOCAL_VAR)
    {
        toVarFrame = "LF@";
    }
    else if(toFrame == GLOBAL_VAR)
    {
        toVarFrame = "GF@";
    }
    else
    {
        toVarFrame = "TF@";
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

void generateWrite(symTableItem *variable, int frame, char *funcName)
{

    char *frameVar = "";

    char str[12];
    sprintf(str, "%d", numberOfUses);

    if(frame == LOCAL_VAR)
    {
        frameVar = "LF@";
    }
    else
    {
        frameVar = "GF@";
    }

    if(variable->type == TYPE_NONE)
    {
        if(strcmp(funcName, "") == 0)
        {
            addInstruction("CREATEFRAME\n");
            addInstruction("PUSHFRAME\n");
        }
        addInstruction("DEFVAR LF@");
        addInstruction(variable->key);
        addInstruction("$type\n");

        addInstruction("TYPE LF@");
        addInstruction(variable->key);
        addInstruction("$type ");
        addInstruction(frameVar);
        addInstruction(variable->key);
        addInstruction("\n");

        addInstruction("JUMPIFEQ $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction(variable->key);
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$nil LF@");
        addInstruction(variable->key);
        addInstruction("$type string@nil\n");

        addInstruction("JUMP $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction(variable->key);
        addInstruction("%");
        addInstruction(str);
        addInstruction("$false$nil\n");
 

        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$if$");
        addInstruction(variable->key);
        addInstruction("%");
        addInstruction(str);
        addInstruction("$true$nil\n");

        addInstruction("WRITE string@None\n");

        addInstruction("JUMP $");
        addInstruction(funcName);
        addInstruction("$end$");
        addInstruction(variable->key);
        addInstruction("%");
        addInstruction(str);
        addInstruction("\n");
    }

    addInstruction("LABEL $");
    addInstruction(funcName);
    addInstruction("$if$");
    addInstruction(variable->key);
    addInstruction("%");
    addInstruction(str);
    addInstruction("$false$nil\n");

    addInstruction("WRITE ");
    addInstruction(frameVar);
    addInstruction(variable->key);
    addInstruction("\n");

    if(variable->type == TYPE_NONE)
    {
        addInstruction("LABEL $");
        addInstruction(funcName);
        addInstruction("$end$");
        addInstruction(variable->key);
        addInstruction("%");
        addInstruction(str);
        addInstruction("\n");

        if(strcmp(funcName, "") == 0)
        {
            addInstruction("POPFRAME\n");
        }
        numberOfUses++;
    }
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


void generateWriteValue(tokenStruct *token)
{
    addInstruction("WRITE ");

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
        if(token->stringValue->actIndex !=0)
        {
            addInstruction(token->stringValue->str);
        }
    }
    addInstruction("\n");
}

void generateRead(char *var, int frame, int type)
{
    char *typeChar;
    char *frameVar = "";

    if(frame == LOCAL_VAR)
    {
        frameVar = "LF@";
    }
    else if(frame == GLOBAL_VAR)
    {
        frameVar = "GF@";
    }
    else
    {
        frameVar = "TF@";
    }

    if(type == INT)
    {
        typeChar = "int";
    }
    else if(type == DOUBLE)
    {
        typeChar = "float";
    }
    else if(type == STRING)
    {
        typeChar = "string";
    }
    else
    {
        typeChar = "bool";
    }

    addInstruction("READ ");
    addInstruction(frameVar);
    addInstruction(var);
    addInstruction(" ");
    addInstruction(typeChar);
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

void generateWhileLabel(int numberOfPrevWhiles, symTableItem *functionName)
{
    char str[12];
    sprintf(str, "%d", numberOfPrevWhiles);

    addInstruction("LABEL ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$while%");
    addInstruction(str);
    addInstruction("\n");
}

void generateWhileStart(int numberOfPrevWhiles, symTableItem *functionName)
{
    char str[12];
    sprintf(str, "%d", numberOfPrevWhiles);

    addInstruction("JUMPIFEQ ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$while$end%");
    addInstruction(str);
    addInstruction(" GF@%lastExpresionResult bool@false\n");

}

void generateWhileEnd(int numberOfPrevWhiles, symTableItem *functionName)
{
    char str[12];
    sprintf(str, "%d", numberOfPrevWhiles);

    addInstruction("JUMP ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$while%");
    addInstruction(str);
    addInstruction("\n");

    addInstruction("\n");

    addInstruction("LABEL ");

    if(functionName != NULL)
    {
        addInstruction(functionName->key);
    }

    addInstruction("$while$end%");
    addInstruction(str);
    addInstruction("\n");
}

