#include <stdio.h>
#include "stack.h"
#include "generator.h"

int main(int argc, char *argv[])
{
    tokenStruct *t;

    DS dynamicString;

    DSInit(&dynamicString);

    setDynamicString(&dynamicString);

    bool newLine = true;

    Stack indentationStack;

    initStack(&indentationStack);
    stackPush(&indentationStack, 0);


    t = malloc(sizeof(tokenStruct));
    t->stringValue = malloc(sizeof(DS));
    DSInit(t->stringValue);


    FILE *sourceCode;

    if(argc == 2)
    {
        sourceCode = fopen(argv[1], "r");
    }
    else
    {
        sourceCode = stdin;
    }

    setSourceCodeFile(sourceCode);
    
    generateHeader();

    while(t->tokenType != TOKEN_EOF)
    {
        if(getToken(t, newLine, &indentationStack) == ERROR_LEX)
        {
            printf("LEX ERROR");
            free(t);
            fclose(sourceCode);

            return -1;
        }
        if(t->tokenType == TOKEN_KEYWORD && t->keyword == PRINT)
        {
            getToken(t, false, &indentationStack);
            if(t->tokenType == TOKEN_LEFT_BRACKET)
            {
                getToken(t, false, &indentationStack);
                generateWriteValue("s", t->stringValue->str);
            }
            else
            {
                printf("SYNTAX ERROR");
                return -6;
            }
            
        }
        if(t->tokenType == TOKEN_KEYWORD && t->keyword == DEF)
        {
            getToken(t, false, &indentationStack);
            if(t->tokenType == TOKEN_IDENTIFIER)
            {
                generateFunctionStart(t->stringValue->str);
            }
            else
            {
                printf("SYNTAX ERROR");
                return -6;
            }

        }
        if(t->tokenType == TOKEN_KEYWORD && t->keyword == RETURN)
        {
            getToken(t, false, &indentationStack);
            generateFunctionReturn("test", t);
        }
    }

    free(t);
    fclose(sourceCode);

    generateFunctionEnd("test");

    printf("%s", dynamicString.str);

}


