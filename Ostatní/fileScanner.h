#ifndef FILESCANNER_H_INCLUDED
#define FILESCANNER_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "dynamicString.h"

#define SCAN_OK 0

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
#define MORE_THAN_STATE 12
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
#define NOT_EQUAL_START_STATE 23
#define DIVIDE_STATE 24
#define ASSIGN_STATE 25
#define REST_OF_LONG_COMMENT_STATE 26
#define NEW_LINE_STATE 27
#define NEW_LINE_START_STATE 28
#define LONG_COMMENT_STATE_ESC 29

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
#define TOKEN_NOT_EQUAL 14
#define TOKEN_LESS 15
#define TOKEN_MORE 16
#define TOKEN_LESS_EQUAL 17
#define TOKEN_MORE_EQUAL 18
#define TOKEN_WHOLE_DIVISION_SIGN 19
#define TOKEN_EQUALS 20
#define TOKEN_DOUBLE_DOT 21
#define TOKEN_COLON 22
#define TOKEN_INDENT 23
#define TOKEN_DEDENT 24

#define TOKEN_EOF 255


#define DEF 0
#define ELSE 1
#define IF 2
#define NONE 3
#define PASS 4
#define RETURN 5
#define WHILE 6
#define INPUTS 7
#define INPUTI 8
#define INPUTF 9
#define PRINT 10
#define LEN 11
#define SUBSTR 12
#define ORD 13
#define CHR 14


#define LEX_ERROR 1
#define SYNTAX_ERROR 2
#define SEM_ERROR_DEF 3
#define SEM_ERROR_COMPATIBILITY 4
#define SEM_ERROR_PARAMS 5
#define SEM_ERROR 6
#define SEM_ERROR_DIV_ZERO 9
#define INTERNAL_ERROR 99

typedef struct
{
    int tokenType;
    int integerValue;
    double doubleValue;
    DS *stringValue;
    int keyword;

}tokenStruct;

int getToken(tokenStruct *token);
void setSourceCodeFile(FILE *sourceCodeFile);
void initIndentationStack();

#endif
