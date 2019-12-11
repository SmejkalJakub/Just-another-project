#include "expr.h"
#include "fileScanner.h"
#include "symtable.h"
#include "symstack.h"

symStack stack;

bool firstConcat = true;

char *currentFuncName;

int precedenceTable[TABLE_SIZE][TABLE_SIZE] =
{
    {R, S, R, S, S, R, R},
    {R, R, R, S, S, R, R},
    {S, S, ER, S, S, R, R},
    {R, R, R, ER, ER, R, R},
    {S, S, S, S, S, EQ, ER},
    {R, R, R, ER, ER, R, R},
    {S, S, S, S, S, ER, ER},
};
int checkAndRetype(symStackItem* operand1, symStackItem* operand2, symStackItem* operand3, int rule, int *finalType);


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
    case TOKEN_KEYWORD:
        if(strcmp("None", token->stringValue->str) == 0)
        {
            return SYM_NONE;
        }
        return SYM_DOLAR;
        break;
    default:
        return SYM_DOLAR;
        break;
    }
}

int getTokenType(tokenStruct *token, STStack *symTableStack)
{
    symTableItem *temp;

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
    else if(token->tokenType == TOKEN_KEYWORD && strcmp(token->stringValue->str, "None") == 0)
    {
        return SPEC_TYPE_NONE;
    }
    else if(token->tokenType == TOKEN_IDENTIFIER)
    {
        if((temp = STStackSearch(symTableStack, token->stringValue->str, NULL)) == NULL)
        {
            return EMPTY_TYPE;
        }
        else
        {
            return temp->type;
        }
    }
    else
    {
        return TYPE_NONE;
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
    else if(symbol == SYM_ASSIGN ||
            symbol == SYM_LESS || symbol == SYM_LESS_EQ ||
            symbol == SYM_MORE || symbol == SYM_MORE_EQ ||
            symbol == SYM_EQ || symbol == SYM_NOT_EQ)
    {
        return PREC_TAB_RELATION_OPERATOR;
    }
    else if(symbol == SYM_ID || symbol == SYM_INT || symbol == SYM_DOUBLE || symbol == SYM_STRING || symbol == SYM_NONE)
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
    else
    {
        return EXPR_ERR;
    }
}


symStackItem *getTopTerm()
{
    symStackItem *topItem = (&stack)->top;
    while (topItem != NULL)
    {
        if(topItem->symbol <= SYM_DOLAR)
            return topItem;
        topItem = topItem->nextItem;
    }

    return NULL;

}

bool getitemsBeforeStop(int *itemsBeforeStop)
{
    symStackItem *tempItem = (&stack)->top;

    while (tempItem != NULL)
    {
        if(tempItem->symbol != SYM_STOP)
        {
            (*itemsBeforeStop)++;
        }
        else
        {
            return true;
        }

        tempItem = tempItem->nextItem;
    }
    return false;
}

int checkExprRule(symStackItem *firstItem, symStackItem *secondItem, symStackItem *thirdItem, int numberOfItems)
{
    if(numberOfItems == 1)
    {
        if(symbolToType(firstItem->symbol) == PREC_TAB_ID)
        {
            return EXPR_ID;
        }
        else
        {
            return EXPR_ERR;
        }
    }
    else if(numberOfItems == 3)
    {
        if(firstItem->symbol == SYM_LEFT_BRACKET && secondItem->symbol == SYM_NON_TERM && thirdItem->symbol == SYM_RIGHT_BRACKET){
            return BRACKET_NONTERM_BRACKET;
        }

        if(firstItem->symbol == SYM_NON_TERM && thirdItem->symbol == SYM_NON_TERM)
        {
            if(secondItem->symbol == SYM_PLUS)
            {
                return EXPR_PLUS;
            }
            else if(secondItem->symbol == SYM_MINUS)
            {
                return EXPR_MINUS;
            }
            else if(secondItem->symbol == SYM_MUL)
            {
                return EXPR_MUL;
            }
            else if(secondItem->symbol == SYM_DIV)
            {
                return EXPR_DIV;
            }
            else if(secondItem->symbol == SYM_SPEC_DIV)
            {
                return EXPR_SPEC_DIV;
            }
            else if(secondItem->symbol == SYM_LESS)
            {
                return EXPR_LESS;
            }
            else if(secondItem->symbol == SYM_MORE)
            {
                return EXPR_MORE;
            }
            else if(secondItem->symbol == SYM_LESS_EQ)
            {
                return EXPR_LESS_EQ;
            }
            else if(secondItem->symbol == SYM_MORE_EQ)
            {
                return EXPR_MORE_EQ;
            }
            else if(secondItem->symbol == SYM_NOT_EQ)
            {
                return EXPR_NOT_EQ;
            }
            else if(secondItem->symbol == SYM_EQ)
            {
                return EXPR_EQ;
            }
            else if(secondItem->symbol == SYM_ASSIGN)
            {
                return EXPR_ASSIGN;
            }
        }
    }

    return EXPR_ERR;
}

int reduce()
{
        bool hasStop = false;

        int itemsBeforeStop = 0;
        int successState;

        bool isFinalZero = false;

        hasStop = getitemsBeforeStop(&itemsBeforeStop);

        symStackItem* operand1 = NULL;
        symStackItem* operand2 = NULL;
        symStackItem* operand3 = NULL;


        int rule;
        int finalType;


        if(hasStop)
        {
            if(itemsBeforeStop == 1)
            {
                operand1 = (&stack)->top;
                rule = checkExprRule(operand1, NULL, NULL, 1);
            }
            else if(itemsBeforeStop == 3)
            {
                operand1 = (&stack)->top->nextItem->nextItem;
                operand2 = (&stack)->top->nextItem;
                operand3 = (&stack)->top;

                rule = checkExprRule(operand1, operand2, operand3, 3);
            }
            else
            {
                return SYNTAX_ERROR;
            }

        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(rule == EXPR_ID && operand1->isZero)
        {
            isFinalZero = true;
        }
        else if(rule == EXPR_MUL && (operand1->isZero || operand3->isZero))
        {
            isFinalZero = true;
        }

        if (rule == EXPR_ERR){
            return SYNTAX_ERROR;
        }
        else
        {
            successState = checkAndRetype(operand1, operand2, operand3, rule, &finalType);

            if(successState != 0){
                return successState;
            }

            if(rule == EXPR_PLUS && finalType == STRING){
                generateConcatenateString(firstConcat);
                firstConcat = false;
            }
            else
            {
                generateExpresion(rule);
            }
        }

        for(int i = 0; i <= itemsBeforeStop; i++)
        {
            symStackPop(&stack);
        }

        symStackPush(&stack, SYM_NON_TERM, finalType);

        if(isFinalZero)
        {
            symStackTop(&stack)->isZero = true;
        }
        else
        {
            symStackTop(&stack)->isZero = false;
        }
        
        return 0;
}

int checkAndRetype(symStackItem* operand1, symStackItem* operand2, symStackItem* operand3, int rule, int *finalType){

    bool firstOperandToInteger = false;
    bool thirdOperandToInteger = false;
    bool firstOperandToDouble = false;
    bool thirdOperandToDouble = false;


    switch(rule){

        case BRACKET_NONTERM_BRACKET:
            *finalType = operand2->type;
            break;

        case EXPR_ID:
            *finalType = operand1->type;
            break;

        case EXPR_PLUS:
        case EXPR_MINUS:
        case EXPR_MUL:

            if(operand1->type == TYPE_NONE && operand3->type == TYPE_NONE)
            {
                *finalType = TYPE_NONE;
                generateDynamicCheckTwoNones(currentFuncName, EXPR_PLUS);
                break;
            }
            else if(operand1->type == TYPE_NONE)
            {
                *finalType = TYPE_NONE;
                generateDynamicCheck(currentFuncName, operand3->type, 1, EXPR_PLUS);
                if(operand3->type != STRING)
                {
                    break;
                }
                else
                {
                    operand1->type = STRING;
                }
            }
            else if(operand3->type == TYPE_NONE)
            {
                *finalType = TYPE_NONE;
                generateDynamicCheck(currentFuncName, operand1->type, 3, EXPR_PLUS);
                if(operand1->type != STRING)
                {
                    break;
                }
                else
                {
                    operand3->type = STRING;
                }
                
            }
             //dva retezce jde jen scitat - konkatenace
            if(operand1->type == STRING && operand3->type == STRING && rule == EXPR_PLUS){
                *finalType = STRING;
                break;
            }
            //pokud byl string jen jeden, nastava chyba
            else if(operand1->type == STRING || operand3->type == STRING){
                return SEM_ERROR_COMPATIBILITY;
            }

            //pokud jsou oba inty, pretypovani neni nutne
            else if((operand1->type == INT) && (operand3->type == INT)){
                *finalType = INT;
                break;
            }
            else if(operand1->type == SPEC_TYPE_NONE || operand3->type == SPEC_TYPE_NONE)
            {
                return SEM_ERROR_COMPATIBILITY;
            }


            //alespon jeden byl double - vysledny bude tez
            *finalType = DOUBLE;

            //a ten, co double nebyl, pretypujeme
            if(operand1->type == INT){
                firstOperandToDouble = true;
            }
            else if (operand3->type == INT){
                thirdOperandToDouble = true;
            }
            break;

        //; klasickem deleni je vzdy vysledek double
        case EXPR_DIV:
            *finalType = DOUBLE;

        
            if(operand1->type == TYPE_NONE && operand3->type == TYPE_NONE)
            {
                generateDynamicCheckTwoNones(currentFuncName, EXPR_DIV);
                break;
            }
            else if(operand1->type == TYPE_NONE)
            {
                if(operand3->type == STRING)
                {
                    return SEM_ERROR_COMPATIBILITY;
                }
                generateDynamicCheck(currentFuncName, operand3->type, 1, EXPR_DIV);
                break;

            }
            else if(operand3->type == TYPE_NONE)
            {
                if(operand1->type == STRING)
                {
                    return SEM_ERROR_COMPATIBILITY;
                }
                generateDynamicCheck(currentFuncName, operand1->type, 3, EXPR_DIV);
                break;
            }

            //stringy delit nelze :-)
            if(operand1->type == STRING || operand3->type == STRING){
                return SEM_ERROR_COMPATIBILITY;
            }
            else if(operand1->type == SPEC_TYPE_NONE || operand3->type == SPEC_TYPE_NONE)
            {
                return SEM_ERROR_COMPATIBILITY;
            }
            else if(operand3->isZero)
            {
                return SEM_ERROR_DIV_ZERO;
            }
            


            if(operand1->type == INT){
                firstOperandToDouble = true;
            }
            if(operand3->type == INT){
                thirdOperandToDouble = true;
            }

            break;

        //pri celosielnem je vysledek typu int
        case EXPR_SPEC_DIV:
            *finalType = INT;
            if(operand1->type == TYPE_NONE && operand3->type == TYPE_NONE)
            {
                generateDynamicCheckTwoNones(currentFuncName, EXPR_SPEC_DIV);
                break;
            }

            if(operand1->type == TYPE_NONE)
            {
                if(operand3->type == STRING)
                {
                    return SEM_ERROR_COMPATIBILITY;
                }
                generateDynamicCheck(currentFuncName, operand3->type, 1, EXPR_SPEC_DIV);
                break;


            }
            else if(operand3->type == TYPE_NONE)
            {
                if(operand1->type == STRING)
                {
                    return SEM_ERROR_COMPATIBILITY;
                }
                generateDynamicCheck(currentFuncName, operand1->type, 3, EXPR_SPEC_DIV);
                break;
            }

            //stringy opet nepodelime
            if(operand1->type == STRING || operand3->type == STRING){
                return SEM_ERROR_COMPATIBILITY;
            }
            else if(operand1->type == SPEC_TYPE_NONE || operand3->type == SPEC_TYPE_NONE)
            {
                return SEM_ERROR_COMPATIBILITY;
            }
            else if(operand3->isZero)
            {
                return SEM_ERROR_DIV_ZERO;
            }

            //pokud byl nejaky z ciselnych operandu double, prevedeme jej na int
            if(operand1->type == DOUBLE){
                firstOperandToInteger = true;
            }
            if(operand3->type == DOUBLE){
                thirdOperandToInteger = true;
            }

            break;

        case EXPR_LESS:
        case EXPR_MORE:
        case EXPR_LESS_EQ:
        case EXPR_MORE_EQ:
        case EXPR_NOT_EQ:
        case EXPR_EQ:
            *finalType = BOOL;

            if(operand1->type == TYPE_NONE)
            {
                generateDynamicCheck(currentFuncName, operand3->type, 1, EXPR_LESS);
                break;
            }
            else if(operand3->type == TYPE_NONE)
            {
                generateDynamicCheck(currentFuncName, operand1->type, 3, EXPR_LESS);
                break;
            }

            
            //pokud jsou oba operandy cisla, udelame z nich doubly
            if (operand1->type == INT && operand3->type == DOUBLE){
                firstOperandToDouble = true;
            }
            else if(operand3->type == INT && operand1->type == DOUBLE){
                thirdOperandToDouble = true;
            }
            else if(operand3->type == SPEC_TYPE_NONE)
            {
                break;
            }
            

            //pokud chceme porovnavat dva operandy, z nichz jeden je jineho typu nez druhy(neplati u cisel) - chyba
            else if(operand1->type != operand3->type){
                return SEM_ERROR_COMPATIBILITY;
            }

            break;

        default:
            break;

    }

    if(firstOperandToDouble){
        generateFirstOperandToDouble();
    }

    if(firstOperandToInteger){
        generateFirstOperandToInteger();
    }

    if(thirdOperandToDouble){
        generateThirdOperandToDouble();
    }
    if(thirdOperandToInteger){
        generateThirdOperandToInteger();
    }

    return 0;

}

void shift(precedenceTabSym currentSym, tokenStruct *token, STStack *symTableStack)
{
    symStackPushStop(&stack);

    symStackPush(&stack, currentSym, getTokenType(token, symTableStack));
    if(token->tokenType == TOKEN_INTEGER && token->integerValue == 0)
    {
        symStackTop(&stack)->isZero = true;
    }
    else if(token->tokenType == TOKEN_DOUBLE && token->doubleValue == 0.0f)
    {
        symStackTop(&stack)->isZero = true;
    }
    else
    {
        symStackTop(&stack)->isZero = false;
    }
    
    

    bool global;


    if(symbolToType(currentSym) == PREC_TAB_ID)
    {
        symStackTop(&stack)->id = malloc(80);
        if(currentSym == SYM_ID)
        {
            STStackSearch(symTableStack, token->stringValue->str, &global);
            strcpy(symStackTop(&stack)->id, token->stringValue->str);
            if(global)
            {
                generateStackPush(token, true);
            }
            else
            {
                generateStackPush(token, false);

            }
        }
        else
        {
            generateStackPush(token, false);
        }

    }
}


int solveExpr(tokenStruct *token, STStack *symTableStack, symTableItem *assignVar, symTableItem *currentFunction)
{
    int returnValue;

    if(currentFunction == NULL)
    {
        currentFuncName = "";
    }
    else
    {
        currentFuncName = currentFunction->key;    
    }
    

    bool end = false;
    precedenceTabSym currentSym;

    symStackItem *symStackTopSym;

    symStackInit(&stack);

    if(!symStackPush(&stack, SYM_DOLAR, NONE))
    {
        return INTERNAL_ERROR;
    }

    int currentSymType;
    int symStackTopSymType;

    while(!end)
    {
        currentSym = tokenToSymbol(token);
        symStackTopSym = getTopTerm();

        currentSymType = symbolToType(currentSym);
        symStackTopSymType = symbolToType(symStackTopSym->symbol);



        switch (precedenceTable[symStackTopSymType][currentSymType])
        {
            case ER:
                {
                    if(currentSym == SYM_DOLAR && symStackTopSym->symbol == SYM_DOLAR)
                    {
                        end = true;
                    }
                    else
                    {
                        return SYNTAX_ERROR;
                    }
                }
                break;
            case EQ:
                {
                    if(!symStackPush(&stack, currentSym, getTokenType(token, symTableStack)))
                    {
                        return INTERNAL_ERROR;
                    }
                    returnValue = getToken(token);
                    if(returnValue != 0)
                    {
                        return returnValue;
                    }
                }
                break;
            case S:
                {
                    shift(currentSym, token, symTableStack);

                    returnValue = getToken(token);
                    if(returnValue != 0)
                    {
                        return returnValue;
                    }
                }
                break;
            case R:
                {
                    returnValue = reduce();
                    if(returnValue != 0)
                    {
                        return returnValue;
                    }
                }
                break;

            default:
                break;
        }
        
    }

    if(stack.top == NULL)
    {
        return INTERNAL_ERROR;
    }
    if(stack.top->symbol != SYM_NON_TERM)
    {
        return SYNTAX_ERROR;
    }

    else if(assignVar != NULL)
    {
        if(assignVar->type == EMPTY_TYPE)
        {
            return 0;
        }
        assignVar->type = stack.top->type;
    }
    else
    {
        if(stack.top->type != BOOL)
        {
            return SYNTAX_ERROR;
        }
    }
    if(stack.top->type != STRING || firstConcat)
    {
        generateSaveLastExpresionValue();
    }
    return 0;
}
