#include "expr.h"
#include "fileScanner.h"
#include "symtable.h"
#include "symstack.h"

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

symStack stack;

symTable *dummyTable;

int precedenceTable[TABLE_SIZE][TABLE_SIZE] = 
{
    {B, L, B, L, L, B, B},
    {B, B, B, L, L, B, B},
    {L, L, ER, L, L, B, B},
    {B, B, B, ER, ER, B, B},
    {L, L, L, L, L, EQ, ER},
    {B, B, B, ER, ER, B, B},
    {L, L, L, L, L, ER, ER},
};


precedenceTabSym tokenToSymbol(tokenStruct *token)
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
    case TOKEN_IDENTIFIER:
        return SYM_ID;
        break;
    case TOKEN_INTEGER:
        return SYM_INT;
        break;
    case TOKEN_DOUBLE:
        return SYM_DOUBLE;
        break;
    case TOKEN_STRING:
        return SYM_STRING;
        break;    
    default:
        return SYM_DOLAR; 
        break;
    }
}

int getTokenType(tokenStruct *token, symTable *localTable)
{
    if(token->tokenType == TOKEN_INTEGER)
    {
        return INT;
    }
    else if(token->tokenType == TOKEN_DOUBLE)
    {
        return DOUBLE;
    }
    else if(token->tokenType == TOKEN_STRING)
    {
        return STRING;
    }
    else if(token->tokenType == TOKEN_IDENTIFIER)
    {
        symTableItem *sym;
        if((sym = STSearch(localTable, token->stringValue->str)) == NULL)
        {
            return NONE;
        }
        else
        {
            return sym->type;
        }
        
    }
    
    
    
}

int symbolToType(precedenceTabSym symbol)
{
    if(symbol == SYM_PLUS || symbol == SYM_MINUS)
    {
        return PREC_TAB_PLUS_MINUS;
    }
    else if(symbol == SYM_MUL || symbol == SYM_DIV || symbol == SYM_SPEC_DIV)
    {
        return PREC_TAB_MUL_DIV_IDIV;
    }
    else if(symbol == SYM_LESS || symbol == SYM_LESS_EQ ||
            symbol == SYM_MORE || symbol == SYM_MORE_EQ ||
            symbol == SYM_EQ || symbol == SYM_NOT_EQ)
    {
        return PREC_TAB_RELATION_OPERATOR;
    }
    else if(symbol == SYM_ID || symbol == SYM_INT || symbol == SYM_DOUBLE || symbol == SYM_STRING)
    {
        return PREC_TAB_ID;
    }
    else if(symbol == SYM_LEFT_BRACKET)
    {
        return PREC_TAB_LEFT_BRACKET;
    }
    else if(symbol == SYM_RIGHT_BRACKET)
    {
        return PREC_TAB_RIGHT_BRACKET;
    }
    else if(symbol == SYM_DOLAR)
    {
        return PREC_TAB_DOLAR;
    }
}


symStackItem *getTopTerm()
{
    symStackItem *topItem = (&stack)->top;
    while (topItem != NULL)
    {
        if(topItem->symbol <= SYM_DOLAR)
            return topItem;
    }

    return NULL;
    
}


int solveExpr(tokenStruct *token)
{

    int returnValue;

    bool end = false;
    precedenceTabSym currentSym;

    symStackItem *symStackTopSym;

    symStackInit(&stack);

    symStackPush(&stack, SYM_DOLAR, NONE);

    int currentSymType;
    int symStackTopSymType;


    while(!end)
    {
        currentSym = tokenToSymbol(token);
        symStackTopSym = getTopTerm();

        currentSymType = symbolToType(currentSym);
        symStackTopSymType = symbolToType(symStackTopSym);



        switch (precedenceTable[symStackTopSymType][currentSymType])
        {
        case ER:
            {
                if(currentSym == SYM_DOLAR && symStackTopSym == SYM_DOLAR)
                {
                    end = true;
                }
                else 
                {
                    return 2;
                }
            }
            break;
        case EQ:
            {
                symStackPush(&stack, currentSym, getTokenType(token, dummyTable));

                returnValue = getToken(token);

            }
        
        default:
            break;
        }
        
    }

}