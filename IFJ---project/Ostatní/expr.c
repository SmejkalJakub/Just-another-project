#include "expr.h"
#include "fileScanner.h"


#define EQ 0
#define L 1
#define B 2
#define ER 3


#define TSIZE 7



#define EXPR_PLUS 0
#define EXPR_MINUS 1
#define EXPR_MUL 2
#define EXPR_DIV 3
#define EXPR_SPEC_DIV 4
#define EXPR_LESS 5
#define EXPR_MORE 6
#define EXPR_LESS_EQ 7
#define EXPR_MORE_EQ 8
#define EXPR_NOT_EQ 9
#define EXPR_EQ 10
#define EXPR_ASSIGN 11
#define EXPR_E 12
#define EXPR_ID 13

typedef enum
{
    SYM_PLUS,
    SYM_MINUS,
    SYM_MUL,
    SYM_DIV,
    SYM_SPEC_DIV,
    SYM_ASSIGN,
    SYM_LESS,
    SYM_MORE,
    SYM_LESS_EQ,
    SYM_MORE_EQ,
    SYM_EQ,
    SYM_NOT_EQ,
    SYM_LEFT_BRACKET,
    SYM_RIGHT_BRACKET,
    SYM_DOLAR

} precedenceTabSym;



/*E -> E + E
E = E
E – E
E < E
E > E
E * E
E / E
E // E
E <= E
E >= E
E != E
E == E
E
id
*/

/*	
"+/-"	"*\/"	r	i	"("	")"	$
"+/-"	>	<	>	<	<	>	>
"*\/"	>	>	>	<	<	>	>
r	<	<		<	<	>	>
i	>	>	>			>	>
"("	<	<	<	<	<	"="	
")"	>	>	>			>	>
$	<	<	<	<	<		
*/


int precedenceTable[TSIZE][TSIZE] = 
{
    {B, L, B, L, L, B, B},
    {B, B, B, L, L, B, B},
    {L, L, ER, L, L, B, B},
    {B, B, B, ER, ER, B, B},
    {L, L, L, L, L, EQ, ER},
    {B, B, B, ER, ER, B, B},
    {L, L, L, L, L, ER, ER},
};





precedenceTabSym getTokenSymbol(tokenStruct *token)
{
    switch (token->tokenType)
    {
    case TOKEN_PLUS_SIGN:
        return SYM_PLUS;
        break;
    case TOKEN_MINUS_SIGN:
        return SYM_MINUS;
        break;
    case TOKEN_MULTIPY_SIGN:
        return SYM_MUL;
        break;
    case TOKEN_DIVIDE_SIGN:
        return SYM_DIV;
        break;
    case TOKEN_WHOLE_DIVISION_SIGN:
        return SYM_SPEC_DIV;
        break;
    case TOKEN_ASSIGN_SIGN:
        return SYM_ASSIGN;
        break;
    case TOKEN_LESS:
        return SYM_LESS;
        break;
    case TOKEN_MORE:
        return SYM_MORE;
        break;
    case TOKEN_LESS_EQUAL:
        return SYM_LESS_EQ;
        break;
    case TOKEN_MORE_EQUAL:
        return SYM_MORE_EQ;
        break;
    case TOKEN_EQUALS:
        return SYM_EQ;
        break;
    case TOKEN_NOT_EQUAL:
        return SYM_NOT_EQ;
        break;
    case TOKEN_LEFT_BRACKET:
        return SYM_LEFT_BRACKET;
        break;
    case TOKEN_RIGHT_BRACKET:
        return SYM_RIGHT_BRACKET;
        break;
    case :
        return SYM_NOT_EQ;
        break;
    case TOKEN_NOT_EQUAL:
        return SYM_NOT_EQ;
        break;

    
    
    default:
        return SYM_DOLAR; 
        break;
    }
}
