#include <stdio.h>
#include "fileScanner.h"

int main(int argc , char *argv[])
{
    tokenStruct *t;

    t = malloc(sizeof(tokenStruct));

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

    while(t->tokenType != TOKEN_EOF)
    {
        if(getToken(t) == SCAN_LEX_ERR)
        {
            printf("LEX ERROR");
            free(t);
            fclose(sourceCode);
            return -1;
        }
        if(t->tokenType == TOKEN_EOL)
        {
            printf("\n");
        }
        if(t->tokenType == TOKEN_INTEGER)
        {
            printf("%d ", t->integerValue);
        }
        else if(t->tokenType == TOKEN_DOUBLE)
        {
            printf("%f ", t->doubleValue);

        }
        else
        {
            printf("%d ", t->tokenType);
        }
    }
    free(t);
    fclose(sourceCode);

}


