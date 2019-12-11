/*
*Implementace pøekladaèe imperativního jazyka IFJ19
*
*Dominik Nejedly (xnejed09)
*
*Hlavní tìlo programu
*
*FIT VUT BRNO
*/

#include "fileScanner.h"
#include "compiler.h"

int main(int argc, char *argv[])
{
    FILE *sourceCode;

    int returnValue;

    if(argc == 2)
    {
        sourceCode = fopen(argv[1], "r");
    }
    else
    {
        sourceCode = stdin;
    }

    setSourceCodeFile(sourceCode);

    returnValue = divideAndConquer();

    fclose(sourceCode);

    return returnValue;
}
