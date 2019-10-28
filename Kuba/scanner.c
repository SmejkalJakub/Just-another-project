#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define ERROR_LEX 1
#define ERROR_INTERNAL 99

#define ERROR_STATE -1
#define EOL_STATE 0
#define START_TOKEN_STATE 1
#define BACKSLASH_STATE 2
#define ONE_LINE_COMMENT_STATE 3
#define LONG_COMMENT_START_FIRST_STATE 4
#define LONG_COMMENT_START_SECOND_STATE 5
#define LONG_COMMENT_STATE 6
#define LONG_COMMENT_END_FIRST_STATE 7
#define LONG_COMMENT_END_SECOND_STATE 8
#define LONG_COMMENT_END_STATE 9
#define KEYWORD_ID_STATE 10
#define DIGIT_STATE 11
#define MORE_THAM_STATE 12
#define LESS_THAN_STATE 13
#define DECIMAL_POINT_STATE 14
#define DOUBLE_NUMBER_STATE 15
#define DOUBLE_EXPONENT_STATE 16
#define REST_OF_DOUBLE_NUMBER_STATE 17
#define DOUBLE_NUMBER_SIGN_STATE 18
#define STRING_STATE 19
#define STRING_ESCAPE_SEQ_STATE 20
#define STRING_ESCAPE_SEQ_FIRST_HEX_STATE 21
#define STRING_ESCAPE_SEQ_SECOND_HEX_STATE 22


#define TOKEN_LEX_ERROR -1
#define TOKEN_EOL 0
#define TOKEN_DIGIT 1
#define TOKEN_PLUS_SIGN 2
#define TOKEN_MINUS_SIGN 3
#define TOKEN_MULTIPY_SIGN 4
#define TOKEN_DIVIDE_SIGN 5
#define TOKEN_ASSIGN_SIGN 6
#define TOKEN_LEFT_BRACKET 7
#define TOKEN_RIGHT_BRACKET 8
#define TOKEN_KEYWORD 9
#define TOKEN_IDENTIFIER 10
#define TOKEN_INTEGER 11
#define TOKEN_DOUBLE 12
#define TOKEN_STRING 13



#define TOKEN_EOF 255

int getToken();
bool addCharToString(char c);


FILE *sourceCode;

char *helpString;

char hexSeq[] = "0x00";

bool addCharToString(char c)
{
    strncat(helpString, &c, sizeof(char));
}

int checkInt(char *word)
{
    char *eptr;

    if(word[0] == '0')
    {
        return TOKEN_LEX_ERROR;
    }

    int number = (int) strtol(word, &eptr, 10);
    if(*eptr)
    {
        printf("error");
    }
    return TOKEN_INTEGER;
}

int checkDouble(char *word)
{
    char *eptr;

    if(word[0] == '0')
    {
        return TOKEN_LEX_ERROR;
    }

    double number = strtod(word, &eptr);

    if(*eptr)
    {
        printf("error");
    }
    return TOKEN_DOUBLE;
}

int checkKeyword(char *word)
{
    int tokenType = TOKEN_KEYWORD;
    if(strcmp(word, "def") == 0)
    {
        printf("def");
    }
    else if(strcmp(word, "else") == 0)
    {
        printf("else");
    }
    else if(strcmp(word, "if") == 0)
    {
        printf("if");
    }
    else if(strcmp(word, "None") == 0)
    {
        printf("None");
    }
    else if(strcmp(word, "pass") == 0)
    {
        printf("pass");
    }
    else if(strcmp(word, "return") == 0)
    {
        printf("return");
    }
    else if(strcmp(word, "while") == 0)
    {
        printf("while");
    }
    else
    {
        //printf("identifier");
        tokenType = TOKEN_IDENTIFIER;
    }
    return tokenType;
}


int main()
{
    int pom = 0;

    sourceCode = fopen("test.txt", "r");

    while((pom = getToken()) != TOKEN_EOF)
    {
        if(pom == TOKEN_EOL)
        {
            printf("\n");
            continue;
        }
        if(pom == TOKEN_LEX_ERROR)
        {
            printf("\nLEX ERROR\n");
            return TOKEN_LEX_ERROR;
        }
        printf("%d ", pom);
    }


}


int getToken()
{
    helpString = malloc(50 * sizeof(char));
    helpString[0] = '\0';

    int state = START_TOKEN_STATE;
    char c = 0;
    char *eptr;


    while(true)
    {
        c = (char)(getc(sourceCode));
        switch(state)
        {
            case START_TOKEN_STATE:
                if(c == '\n')
                {
                    state = EOL_STATE;
                }
                else if(c == '#')
                {
                    state = ONE_LINE_COMMENT_STATE;
                }
                else if(c == '"')
                {
                    state = LONG_COMMENT_START_FIRST_STATE;
                }
                else if(c == '\'')
                {
                    state = STRING_STATE;
                }
                else if(isalpha(c) || c == '_')
                {
                    addCharToString(c);
                    state = KEYWORD_ID_STATE;
                }
                else if(isdigit(c))
                {
                    addCharToString(c);
                    state = DIGIT_STATE;
                }
                else if(c == '(')
                {
                    return TOKEN_LEFT_BRACKET;
                }
                else if(c == ')')
                {
                    return TOKEN_RIGHT_BRACKET;
                }
                else if(c == '=')
                {
                    return TOKEN_ASSIGN_SIGN;
                }
                else if(c == '+')
                {
                    return TOKEN_PLUS_SIGN;
                }
                else if(c == '*')
                {
                    return TOKEN_MULTIPY_SIGN;
                }
                else if(c == '/')
                {
                    return TOKEN_DIVIDE_SIGN;
                }
                else if(c == '-')
                {
                    return TOKEN_MINUS_SIGN;
                }
                else if(c == EOF)
                {
                    return TOKEN_EOF;
                }
                else if(isspace(c))
                {
                    state = START_TOKEN_STATE;
                }
                else
                {
                    printf("lex error");
                    return TOKEN_LEX_ERROR;
                }
                break;
            case STRING_STATE:
                if(c < 32)
                {
                    return TOKEN_LEX_ERROR;
                }
                else if(c == '\'')
                {
                    printf("%s", helpString);
                    return TOKEN_STRING;
                }
                else if(c == '\\')
                {
                    state = STRING_ESCAPE_SEQ_STATE;
                }
                else
                {
                    state = STRING_STATE;
                    addCharToString(c);
                }
                break;
            case STRING_ESCAPE_SEQ_STATE:
                if(c < 32)
                {
                    return TOKEN_LEX_ERROR;
                }
                else if(c == '\\')
                {
                    addCharToString('\\');
                    state = STRING_STATE;
                }
                else if(c == '\'')
                {
                    addCharToString('\'');
                    state = STRING_STATE;
                }
                else if(c == '\"')
                {
                    addCharToString('\"');
                    state = STRING_STATE;
                }
                else if(c == 'n')
                {
                    addCharToString('\n');
                    state = STRING_STATE;
                }
                else if(c == 't')
                {
                    addCharToString('\t');
                    state = STRING_STATE;
                }
                else if(c == 'x')
                {
                    state = STRING_ESCAPE_SEQ_FIRST_HEX_STATE;
                }
                else
                {
                    addCharToString('\\');
                    addCharToString(c);
                    state = STRING_STATE;
                }
                break;
            case STRING_ESCAPE_SEQ_FIRST_HEX_STATE:
                if(isxdigit(c))
                {
                    hexSeq[2] = c;
                    state = STRING_ESCAPE_SEQ_SECOND_HEX_STATE;
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case STRING_ESCAPE_SEQ_SECOND_HEX_STATE:
                if(isxdigit(c))
                {
                    hexSeq[3] = c;
                    addCharToString((char)strtol(hexSeq, &eptr, 16));
                    state = STRING_STATE;
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case LONG_COMMENT_START_FIRST_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_START_SECOND_STATE;
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case LONG_COMMENT_START_SECOND_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_STATE;
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case LONG_COMMENT_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_END_FIRST_STATE;
                }
                else if(c == EOF)
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case LONG_COMMENT_END_FIRST_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_END_SECOND_STATE;
                }
                else
                {
                    state = LONG_COMMENT_STATE;
                }
                break;
            case LONG_COMMENT_END_SECOND_STATE:
                if(c == '"')
                {
                    state = START_TOKEN_STATE;
                }
                else
                {
                    state = LONG_COMMENT_STATE;
                }
                break;
            case KEYWORD_ID_STATE:
                if(isalpha(c) || c == '_')
                {
                    addCharToString(c);
                }
                else
                {
                    ungetc(c, sourceCode);
                    return checkKeyword(helpString);
                }
                break;
            case DIGIT_STATE:
                if(isdigit(c))
                {
                    addCharToString(c);
                }
                else if(c == '.')
                {
                    addCharToString(c);
                    state = DECIMAL_POINT_STATE;
                }
                else if (tolower(c) == 'e')
				{
					state = DOUBLE_EXPONENT_STATE;
					addCharToString(c);
				}
                else
                {
                    ungetc(c, sourceCode);
                    return checkInt(helpString);
                }
                break;
            case DECIMAL_POINT_STATE:
                if(isdigit(c))
                {
                    addCharToString(c);
                    state = DOUBLE_NUMBER_STATE;
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case DOUBLE_NUMBER_STATE:
                if(isdigit(c))
                {
                    addCharToString(c);
                }
                else if(tolower(c) == 'e')
                {
                    addCharToString(c);
                    state = DOUBLE_EXPONENT_STATE;
                }
                else
                {
                    ungetc(c, sourceCode);
                    return checkDouble(helpString);
                }
                break;
            case DOUBLE_EXPONENT_STATE:
                if(isdigit(c))
                {
                    addCharToString(c);
                    state = REST_OF_DOUBLE_NUMBER_STATE;
                }
                else if(c == '-' || c == '+')
                {
                    addCharToString(c);
                    state = DOUBLE_NUMBER_SIGN_STATE;
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case DOUBLE_NUMBER_SIGN_STATE:
                if(isdigit(c))
                {
                    state = REST_OF_DOUBLE_NUMBER_STATE;
                    addCharToString(c);
                }
                else
                {
                    return TOKEN_LEX_ERROR;
                }
                break;
            case REST_OF_DOUBLE_NUMBER_STATE:
                if(isdigit(c))
                {
                    addCharToString(c);
                }
                else
                {
                    ungetc(c, sourceCode);
                    return checkDouble(helpString);
                }
                break;
            case ONE_LINE_COMMENT_STATE:
                {
                    if(c == EOF || c == '\n')
                    {
                        ungetc(c, sourceCode);
                        state = START_TOKEN_STATE;
                    }
                }
                break;
            case EOL_STATE:
                {
                    if(isspace(c))
                    {
                        state = START_TOKEN_STATE;
                    }
                    ungetc(c, sourceCode);
                    return TOKEN_EOL;
                }
                break;
        }
    }
    printf("freeing");
    free(helpString);
}



