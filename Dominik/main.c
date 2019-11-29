#include <stdio.h>
#include <stdlib.h>
#include "fileScanner.h"

int main(int argc, char *argv[])
{
    Stack indentStack;

    initStack(&indentStack);

    tokenStruct token;

    token.stringValue = malloc(sizeof(DS));
    DSInit(token.stringValue);

    FILE *f = fopen(argv[1], "r");

    if(f == NULL)
    {
        return 1;
    }

    setSourceCodeFile(f);

    int error;

    while(token.tokenType != TOKEN_EOF)
    {
        error = getToken(&indentStack, &token);

        if(error == ERROR_LEX)
        {
            printf("\nErrorLex\n\n");
        }
        else if(error == ERROR_INTERNAL)
        {
            printf("\nErrorInternal\n\n");
        }
        else
        {
            switch(token.tokenType)
            {
                case TOKEN_EOL:
                    printf("TOKEN_EOL\n");
                    break;
                case TOKEN_DIGIT:
                    printf("TOKEN_DIGIT\n");
                    break;
                case TOKEN_PLUS_SIGN:
                    printf("TOKEN_PLUS_SIGN\n");
                    break;
                case TOKEN_MINUS_SIGN:
                    printf("TOKEN_MINUS_SIGN\n");
                    break;
                case TOKEN_MULTIPY_SIGN:
                    printf("TOKEN_MULTIPY_SIGN\n");
                    break;
                case TOKEN_DIVIDE_SIGN:
                    printf("TOKEN_DIVIDE_SIGN\n");
                    break;
                case TOKEN_ASSIGN_SIGN:
                    printf("TOKEN_ASSIGN_SIGN\n");
                    break;
                case TOKEN_LEFT_BRACKET:
                    printf("TOKEN_LEFT_BRACKET\n");
                    break;
                case TOKEN_RIGHT_BRACKET:
                    printf("TOKEN_RIGHT_BRACKET\n");
                    break;
                case TOKEN_KEYWORD:
                    printf("TOKEN_KEYWORD\t\t%s\n", token.stringValue->str);
                    break;
                case TOKEN_IDENTIFIER:
                    printf("TOKEN_IDENTIFIER\t\t%s\n", token.stringValue->str);
                    break;
                case TOKEN_INTEGER:
                    printf("TOKEN_INTEGER\t\t%d\n", token.integerValue);
                    break;
                case TOKEN_DOUBLE:
                    printf("TOKEN_DOUBLE\t\t%f\n", token.doubleValue);
                    break;
                case TOKEN_STRING:
                    printf("TOKEN_STRING\t\t%s\n", token.stringValue->str);
                    break;
                case TOKEN_NOT_EQUAL:
                    printf("TOKEN_NOT_EQUAL\n");
                    break;
                case TOKEN_LESS:
                    printf("TOKEN_LESS\n");
                    break;
                case TOKEN_MORE:
                    printf("TOKEN_MORE\n");
                    break;
                case TOKEN_LESS_EQUAL:
                    printf("TOKEN_LESS_EQUAL\n");
                    break;
                case TOKEN_MORE_EQUAL:
                    printf("TOKEN_MORE_EQUAL\n");
                    break;
                case TOKEN_WHOLE_DIVISION_SIGN:
                    printf("TOKEN_WHOLE_DIVISION_SIGN\n");
                    break;
                case TOKEN_EQUALS:
                    printf("TOKEN_EQUALS\n");
                    break;
                case TOKEN_DOUBLE_DOT:
                    printf("TOKEN_DOUBLE_DOT\n");
                    break;
                case TOKEN_COLON:
                    printf("TOKEN_COLON\n");
                    break;
                case TOKEN_INDENT:
                    printf("TOKEN_INDENT\n");
                    break;
                case TOKEN_DEDENT:
                    printf("TOKEN_DEDENT\n");
                    break;
            }
        }
    }
}
