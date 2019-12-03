#include "fileScanner.h"



FILE *sourceCode;

Stack indentStack;


void initIndentationStack()
{
    initStack(&indentStack);
    stackPush(&indentStack, 0);
}

void setSourceCodeFile(FILE *sourceCodeFile)
{
    sourceCode = sourceCodeFile;
}

int checkInt(DS *word, tokenStruct *token)
{

    char *eptr;

    if(word->str[0] == '0' && word->actIndex > 1)
    {
        DSDelete(word);
        return LEX_ERROR;
    }

    int number = (int) strtol(word->str, &eptr, 10);

    if(*eptr)
    {
        DSDelete(word);
        return INTERNAL_ERROR;
    }
    token->tokenType = TOKEN_INTEGER;
    token->integerValue = number;

    DSDelete(word);
    return SCAN_OK;
}

int checkDouble(DS *word, tokenStruct *token)
{
    char *eptr;

    if(word->str[0] == '0' && word->actIndex > 1 && word->str[1] != '.')
    {
        DSDelete(word);
        return LEX_ERROR;
    }

    double number = strtod(word->str, &eptr);

    if(*eptr)
    {
        DSDelete(word);
        return INTERNAL_ERROR;
    }

    token->doubleValue = number;
    token->tokenType = TOKEN_DOUBLE;

    DSDelete(word);
    return SCAN_OK;
}

int checkKeyword(DS *word, tokenStruct *token)
{
    token->tokenType = TOKEN_KEYWORD;
    if(strcmp(word->str, "def") == 0)
    {
        token->keyword = DEF;
    }
    else if(strcmp(word->str, "else") == 0)
    {
        token->keyword = ELSE;
    }
    else if(strcmp(word->str, "if") == 0)
    {
        token->keyword = IF;
    }
    else if(strcmp(word->str, "None") == 0)
    {
        token->keyword = NONE;
    }
    else if(strcmp(word->str, "pass") == 0)
    {
        token->keyword = PASS;
    }
    else if(strcmp(word->str, "return") == 0)
    {
        token->keyword = RETURN;
    }
    else if(strcmp(word->str, "while") == 0)
    {
        token->keyword = WHILE;
    }
    else if(strcmp(word->str, "inputs") == 0)
    {
        token->keyword = INPUTS;
    }
    else if(strcmp(word->str, "inputi") == 0)
    {
        token->keyword = INPUTI;
    }
    else if(strcmp(word->str, "inputf") == 0)
    {
        token->keyword = INPUTF;
    }
    else if(strcmp(word->str, "print") == 0)
    {
        token->keyword = PRINT;
    }
    else if(strcmp(word->str, "len") == 0)
    {
        token->keyword = LEN;
    }
    else if(strcmp(word->str, "substr") == 0)
    {
        token->keyword = SUBSTR;
    }
    else if(strcmp(word->str, "ord") == 0)
    {
        token->keyword = ORD;
    }
    else if(strcmp(word->str, "chr") == 0)
    {
        token->keyword = CHR;
    }
    else
    {
        token->tokenType = TOKEN_IDENTIFIER;
    }
    if(!DSAddStr(token->stringValue, word->str))
    {
        return INTERNAL_ERROR;
    }
    DSDelete(word);
    return SCAN_OK;
}

int getToken(tokenStruct *token)
{
    DS DString;

    char hexSeq[] = "0x00";

    static bool newLine = true;
    static bool returningDedent = false;

    bool CommentStr = newLine;

    static int numOfSpaces = 0;

    DSInit(&DString);

    DSDelete(token->stringValue);

    int state = START_TOKEN_STATE;
    char c = 0;
    char *eptr;

    if(newLine || returningDedent)
    {
        state = NEW_LINE_START_STATE;
    }

    if(!returningDedent)
    {
        numOfSpaces = 0;
    }

    while(true)
    {
        c = (char)(getc(sourceCode));
        switch(state)
        {
            case NEW_LINE_START_STATE:

                newLine = false;
                if(returningDedent)
                {
                    if(numOfSpaces != stackTop(&indentStack))
                    {
                        if(stackTop(&indentStack) == 0)
                        {
                            DSDelete(&DString);
                            return LEX_ERROR;
                        }
                        else
                        {
                            stackPop(&indentStack);
                        }

                        ungetc(c, sourceCode);

                        DSDelete(&DString);
                        token->tokenType = TOKEN_DEDENT;

                        return SCAN_OK;
                    }
                    else
                    {
                        returningDedent = false;
                        state = START_TOKEN_STATE;
                    }
                }
                if(isspace(c) && (c != '\n' && c != '\r'))
                {
                    numOfSpaces++;

                }
                else if(c != '#')
                {
                    ungetc(c, sourceCode);
                    if(numOfSpaces > stackTop(&indentStack))
                    {
                        if(!stackPush(&indentStack, numOfSpaces))
                        {
                            DSDelete(&DString);
                            return INTERNAL_ERROR;
                        }
                        token->tokenType = TOKEN_INDENT;
                        DSDelete(&DString);
                        return SCAN_OK;
                    }
                    else if(numOfSpaces == stackTop(&indentStack))
                    {
                        state = START_TOKEN_STATE;
                    }
                    else if(numOfSpaces < stackTop(&indentStack))
                    {
                        returningDedent = true;
                        stackPop(&indentStack);
                        DSDelete(&DString);
                        token->tokenType = TOKEN_DEDENT;
                        return SCAN_OK;
                    }
                }
                else if(c == '#')
                {
                    state = ONE_LINE_COMMENT_STATE;
                }

                break;
            case START_TOKEN_STATE:
                if(c == '\n')
                {
                    state = EOL_STATE;
                    newLine = true;
                }
                else if(c == '#')
                {
                    state = ONE_LINE_COMMENT_STATE;
                }
                else if(c == '"')
                {
                    state = LONG_COMMENT_START_FIRST_STATE;
                }
                else if(c == '!')
                {
                    state = NOT_EQUAL_START_STATE;
                }
                else if(c == '\'')
                {
                    state = STRING_STATE;
                }
                else if(isalpha(c) || c == '_')
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = KEYWORD_ID_STATE;
                }
                else if(isdigit(c))
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = DIGIT_STATE;
                }
                else if(c == ':')
                {
                    token->tokenType = TOKEN_DOUBLE_DOT;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == ',')
                {
                    token->tokenType = TOKEN_COLON;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == '(')
                {
                    token->tokenType = TOKEN_LEFT_BRACKET;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == ')')
                {
                    token->tokenType = TOKEN_RIGHT_BRACKET;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == '=')
                {
                    state = ASSIGN_STATE;
                }
                else if(c == '+')
                {
                    token->tokenType = TOKEN_PLUS_SIGN;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == '*')
                {
                    token->tokenType = TOKEN_MULTIPY_SIGN;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == '/')
                {
                    state = DIVIDE_STATE;
                }
                else if(c == '<')
                {
                    state = LESS_THAN_STATE;
                }
                else if(c == '>')
                {
                    state = MORE_THAN_STATE;
                }
                else if(c == '-')
                {
                    token->tokenType = TOKEN_MINUS_SIGN;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == EOF)
                {
                    token->tokenType = TOKEN_EOF;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(isspace(c))
                {
                    state = START_TOKEN_STATE;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case ASSIGN_STATE:
                if(c == '=')
                {
                    token->tokenType = TOKEN_EQUALS;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else
                {
                    ungetc(c, sourceCode);
                    token->tokenType = TOKEN_ASSIGN_SIGN;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                break;
            case DIVIDE_STATE:
                if(c == '/')
                {
                    token->tokenType = TOKEN_WHOLE_DIVISION_SIGN;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else
                {
                    ungetc(c, sourceCode);
                    token->tokenType = TOKEN_DIVIDE_SIGN;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                break;
            case LESS_THAN_STATE:
                if(c == '=')
                {
                    token->tokenType = TOKEN_LESS_EQUAL;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else
                {
                    ungetc(c, sourceCode);
                    token->tokenType = TOKEN_LESS;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                break;
            case MORE_THAN_STATE:
                if(c == '=')
                {
                    token->tokenType = TOKEN_MORE_EQUAL;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else
                {
                    ungetc(c, sourceCode);
                    token->tokenType = TOKEN_MORE;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                break;
            case STRING_STATE:
                if(c < 32)
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                else if(c == '\'')
                {
                    token->tokenType = TOKEN_STRING;
                    DSAddStr(token->stringValue, (&DString)->str);
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else if(c == '\\')
                {
                    state = STRING_ESCAPE_SEQ_STATE;
                }
                else if(c == '\n')
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                else
                {
                    state = STRING_STATE;
                    if(c != ' ')
                    {
                        if(!DSAddChar(&DString, c))
                        {
                            DSDelete(&DString);
                            return INTERNAL_ERROR;
                        }
                    }
                    else
                    {
                        if(!DSAddStr(&DString, "\\032"))
                        {
                            DSDelete(&DString);
                            return INTERNAL_ERROR;
                        }
                    }

                }
                break;
            case NOT_EQUAL_START_STATE:
                if(c == '=')
                {
                    token->tokenType = TOKEN_NOT_EQUAL;
                    DSDelete(&DString);
                    return SCAN_OK;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case STRING_ESCAPE_SEQ_STATE:
                if(c < 32)
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                else if(c == '\\')
                {
                    if(!DSAddChar(&DString, '\\'))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = STRING_STATE;
                }
                else if(c == '\'')
                {
                    if(!DSAddChar(&DString, '\''))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = STRING_STATE;
                }
                else if(c == '\"')
                {
                    if(!DSAddChar(&DString, '\"'))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = STRING_STATE;
                }
                else if(c == 'n')
                {
                    if(!DSAddChar(&DString, '\n'))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = STRING_STATE;
                }
                else if(c == 't')
                {
                    if(!DSAddChar(&DString, '\t'))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = STRING_STATE;
                }
                else if(c == 'x')
                {
                    state = STRING_ESCAPE_SEQ_FIRST_HEX_STATE;
                }
                else
                {
                    if(!DSAddChar(&DString, '\\'))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
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
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case STRING_ESCAPE_SEQ_SECOND_HEX_STATE:
                if(isxdigit(c))
                {
                    hexSeq[3] = c;
                    if(!DSAddChar(&DString, (char)strtol(hexSeq, &eptr, 16)))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = STRING_STATE;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case LONG_COMMENT_START_FIRST_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_START_SECOND_STATE;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case LONG_COMMENT_START_SECOND_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_STATE;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case LONG_COMMENT_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_END_FIRST_STATE;
                }
                else if(c == EOF)
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                else if(!CommentStr)
                {
                    if(c == '\\')
                    {
                        state = LONG_COMMENT_STATE_ESC;
                    }
                    else if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                }
                break;
            case LONG_COMMENT_STATE_ESC:
                if(c < 32)
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                else if(c == '"')
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = LONG_COMMENT_STATE;
                }
                else
                {
                    if(!DSAddChar(&DString, '\\'))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = LONG_COMMENT_STATE;
                }
                break;
            case LONG_COMMENT_END_FIRST_STATE:
                if(c == '"')
                {
                    state = LONG_COMMENT_END_SECOND_STATE;
                }
                else
                {
                    if(!CommentStr)
                    {
                        if(!DSAddChar(&DString, '"'))
                        {
                            DSDelete(&DString);
                            return INTERNAL_ERROR;
                        }

                        if(!DSAddChar(&DString, c))
                        {
                            DSDelete(&DString);
                            return INTERNAL_ERROR;
                        }
                    }

                    state = LONG_COMMENT_STATE;
                }
                break;
            case LONG_COMMENT_END_SECOND_STATE:
                if(c == '"')
                {
                    if(!CommentStr)
                    {
                        token->tokenType = TOKEN_STRING;
                        DSAddStr(token->stringValue, (&DString)->str);
                        DSDelete(&DString);
                        return SCAN_OK;
                    }

                    state = REST_OF_LONG_COMMENT_STATE;
                }
                else
                {
                    if(!CommentStr)
                    {
                        for(int i = 0; i < 2; i++)
                        {
                            if(!DSAddChar(&DString, '"'))
                            {
                                DSDelete(&DString);
                                return INTERNAL_ERROR;
                            }
                        }

                        if(!DSAddChar(&DString, c))
                        {
                            DSDelete(&DString);
                            return INTERNAL_ERROR;
                        }
                    }

                    state = LONG_COMMENT_STATE;
                }
                break;
            case REST_OF_LONG_COMMENT_STATE:
                if(c == '\n')
                {
                    state = START_TOKEN_STATE;
                }
                else if(!isspace(c))
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case KEYWORD_ID_STATE:
                if(isalnum(c) || c == '_')
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                }
                else
                {
                    ungetc(c, sourceCode);
                    return checkKeyword(&DString, token);
                }
                break;
            case DIGIT_STATE:
                if(isdigit(c))
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                }
                else if(c == '.')
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = DECIMAL_POINT_STATE;
                }
                else if (tolower(c) == 'e')
				{
					state = DOUBLE_EXPONENT_STATE;
					if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
				}
                else
                {
                    ungetc(c, sourceCode);
                    return checkInt(&DString, token);
                }
                break;
            case DECIMAL_POINT_STATE:
                if(isdigit(c))
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = DOUBLE_NUMBER_STATE;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case DOUBLE_NUMBER_STATE:
                if(isdigit(c))
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                }
                else if(tolower(c) == 'e')
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = DOUBLE_EXPONENT_STATE;
                }
                else
                {
                    ungetc(c, sourceCode);
                    return checkDouble(&DString, token);
                }
                break;
            case DOUBLE_EXPONENT_STATE:
                if(isdigit(c))
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = REST_OF_DOUBLE_NUMBER_STATE;
                }
                else if(c == '-' || c == '+')
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                    state = DOUBLE_NUMBER_SIGN_STATE;
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case DOUBLE_NUMBER_SIGN_STATE:
                if(isdigit(c))
                {
                    state = REST_OF_DOUBLE_NUMBER_STATE;
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                }
                else
                {
                    DSDelete(&DString);
                    return LEX_ERROR;
                }
                break;
            case REST_OF_DOUBLE_NUMBER_STATE:
                if(isdigit(c))
                {
                    if(!DSAddChar(&DString, c))
                    {
                        DSDelete(&DString);
                        return INTERNAL_ERROR;
                    }
                }
                else
                {
                    ungetc(c, sourceCode);
                    return checkDouble(&DString, token);
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
                    ungetc(c, sourceCode);
                    DSDelete(&DString);

                    if(!CommentStr)
                    {
                        token->tokenType = TOKEN_EOL;
                        return SCAN_OK;
                    }
                    state = NEW_LINE_START_STATE;
                }
                break;
        }
    }
}
