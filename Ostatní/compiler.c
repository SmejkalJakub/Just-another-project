#include "compiler.h"


int result;

#define GET_TOKEN \
        result = getToken(&compilerData->token); \
        if(result != 0) \
        { \
            return result; \
        } \

/*

TODODODO
---------


PREDELAT A DODELAT NASLEDUJICI:

    - GENEROVANI KODU. VETSINOU ZAKOMENTOVANE, BUD JESTE NEEXISTUJE (WHILE, IFY) NEBO NEVIM JAK ZAVOLAT

    - KONTROLA CHYBOVYCH STAVU, VYPIS, NAVRATOVE HODNOTY

    - ZKONTROLOVAT ZAPSANI PRAVIDEL

    - ZASOBNIK TABULEK

    - SPOJENI K VOLANI Z JEDNOHO MAINU

    - PREDAVANI PARAMETRU - VOLANI getToken V expr.c

    - NEJAKE PEKNE MAKRO PRO GETTOKEN A JEHO NASLEDNOU KONTROLU PRO ZPREHLEDNENI
*/


int compilerDataInit(CompilerData* compilerData){

    STStackInit(compilerData->tablesStack);

    STStackPush(compilerData->tablesStack);

    compilerData->globalTable = STStackTop(compilerData->tablesStack);

    //compilerData->globalTable = malloc(sizeof(symTable));
    //compilerData->localTable = malloc(sizeof(symTable));

    //inicializace indentStacku, prvotni pushnuti 0

    compilerData->numberOfIfs = 0;
    compilerData->numberOfWhiles = 0;

    compilerData->current_function = NULL;

    if((compilerData->token.stringValue = malloc(sizeof(DS))) == NULL)
    {
        return INTERNAL_ERROR;
    }

    DSInit(compilerData->token.stringValue);

    compilerData -> inFunction = NULL;
	compilerData -> inWhileOrIf = NULL;

    compilerData->varToAssign = NULL;

	return 0;
}


static int Prog (CompilerData *compilerData)
{
    printf("prog\n");

    //PROG -> def id ( PARAMS ) : eol indent PRIKAZY_SEK dedent PROG
    if((compilerData->token.tokenType == TOKEN_KEYWORD) && (compilerData->token.keyword == DEF))
    {
        compilerData->inFunction = true;
        GET_TOKEN;
    }
    //PROG -> eol PROG
    else if(compilerData->token.tokenType == TOKEN_EOL)
    {
        GET_TOKEN;
        return Prog(compilerData);
    }
    //PROG -> eof
    else if(compilerData->token.tokenType == TOKEN_EOF)
    {
        return 0;
    }
    //PROG -> PRIKAZ PROG
    else
    {
        result = Prikaz(compilerData);

        if(result != 0)
        {
            return result;
        }

        return Prog(compilerData);
    }

    //pokracovani def fce
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        symTableItem *newTabItem = STInsert(compilerData->globalTable, compilerData->token.stringValue->str);

        if(!newTabItem)
        {
            return INTERNAL_ERROR;
        }

        compilerData->current_function = newTabItem;

        newTabItem->function = true;

        generateFunctionStart(compilerData->current_function);

        GET_TOKEN;
    }
    else
    {
        return SYNTAX_ERROR;
    }

    if (compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
    {
        GET_TOKEN;
        Parametry(compilerData);
    }
    else
    {
        return SYNTAX_ERROR;
    }

    if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT)
    {
        GET_TOKEN;
    }
    else
    {
        return SYNTAX_ERROR;
    }

    if((compilerData->token.tokenType == TOKEN_EOL))
    {
        GET_TOKEN;
    }
    else
    {
        return SYNTAX_ERROR;
    }

    if(compilerData->token.tokenType == TOKEN_INDENT)
    {
        GET_TOKEN;
        prikazySekv(compilerData);
    }
    else
    {
        return SYNTAX_ERROR;
    }

    GET_TOKEN;

    generateFunctionEnd(compilerData->current_function);

    STStackPop(compilerData->tablesStack);

    compilerData->localTable = NULL;
    compilerData->current_function = NULL;

    compilerData->inFunction = false;
    return Prog(compilerData);
}


static int prikazySekv(CompilerData *compilerData)
{
    //PRIKAZY_SEK -> PRIKAZ DALSI_PRIKAZ
    printf("prikazysekv\n");

    result = Prikaz(compilerData);

    if(result != 0)
    {
        return result;
    }

    return dalsiPrikaz(compilerData);
}


static int dalsiPrikaz(CompilerData *compilerData)
{
    //DEDENT = konec aktualni sekvence prikazu
    //DALSI_PRIKAZ -> eps
    printf("dalsiPrikaz\n");

    if (compilerData->token.tokenType == TOKEN_DEDENT)
    {
        return 0;
    }
    //DALSI_PRIKAZ -> PRIKAZY_SEKV
    else
    {
        return prikazySekv(compilerData);
    }

}


static int volaniNeboPrirazeni(CompilerData *compilerData)
{
    printf("volaniNeboPrirazeni\n");

    bool defNewVar = false;

    compilerData->varToAssign = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &compilerData->global);

    if(compilerData->varToAssign == NULL)
    {
        if((compilerData->varToAssign = STInsert(compilerData->tablesStack->top->symTablePtr, compilerData->token.stringValue->str)) == NULL)
        {
            return INTERNAL_ERROR;
        }

        if(!compilerData->inFunction)
        {
            compilerData->global = true;
        }
        else
        {
            compilerData->global = false;
        }

        defNewVar = true;
    }

    GET_TOKEN;

    //VOLANI_NEBO_PRIRAZENI -> = FCE_DEF_NEBO_VEST
    if(compilerData->token.tokenType == TOKEN_ASSIGN_SIGN)
    {
        if(defNewVar)
        {
            if(compilerData->global)
            {
                generateVariableDef(compilerData->varToAssign->key, GLOBAL_VAR);
            }
            else
            {
                generateVariableDef(compilerData->varToAssign->key, LOCAL_VAR);
            }
        }

        GET_TOKEN;
        return fceDefNeboVest(compilerData);
    }
    //VOLANI_NEBO_PRIRAZENI -> ( HODNOTY )
    else if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
    {
        compilerData->current_function = compilerData->varToAssign;
        compilerData->varToAssign = NULL;

        GET_TOKEN;
        result = Hodnoty(compilerData);

        if(result == 0)
        {
            generateCall(compilerData->current_function->key);
        }

        return result;
    }
    else
    {
        return SYNTAX_ERROR;
    }
}

static int fceDefNeboVest(CompilerData *compilerData)
{
    printf("fceDefNeboVest\n");
    //FCE_DEF_NEBO_VEST -> id ( HODNOTY )
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER && (STSearch(compilerData->globalTable, compilerData->token.stringValue->str)->function))
    {
        GET_TOKEN;

        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
        {
            GET_TOKEN;
            return Hodnoty(compilerData);
        }
        else
        {
            return SYNTAX_ERROR;
        }
    }
    //FCE_DEF_NEBO_VEST -> NAVRAT_HODNOT
    else
    {
        return navratHodnoty(compilerData);
    }
}


static int Prikaz (CompilerData *compilerData)
{
    printf("prikaz %d %s\n", compilerData->token.tokenType, compilerData->token.stringValue->str);
    //PRIKAZ -> id VOLANI_NEBO_PRIRAZENI eol
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        result = volaniNeboPrirazeni(compilerData);

        if(result != 0)
        {
            return result;
        }

        if(compilerData->token.tokenType == TOKEN_EOL)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        compilerData->varToAssign = NULL;

        return 0;
    }
    //PRIKAZ -> return VYRAZ eol
    else if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == RETURN)
    {
        if(!compilerData->inFunction)
        {
            return SYNTAX_ERROR;
        }
        else
        {
            GET_TOKEN;
            if(compilerData->token.tokenType != TOKEN_EOL)
            {
                result = solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

                if(result != 0)
                {
                    return result;
                }

                generateFunctionReturn(compilerData->current_function, false);

                if(compilerData->token.tokenType == TOKEN_EOL)
                {
                    GET_TOKEN;
                }
                else
                {
                    return SYNTAX_ERROR;
                }

                return 0;
            }

            generateFunctionReturn(compilerData->current_function, true);

            GET_TOKEN;

            return 0;
        }
    }
    //PRIKAZ -> if VYRAZ : eol indent PRIKAZY_SEK dedent else : eol indent PRIKAZY_SEK dedent
    else if(compilerData->token.keyword == IF && compilerData->token.tokenType == TOKEN_KEYWORD)
    {
        GET_TOKEN;

        int numberOfIfs = compilerData->numberOfIfs;

        compilerData->numberOfIfs++;

        result = solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

        if(result != 0)
        {
            return result;
        }

        generateIfStart(numberOfIfs, compilerData->current_function);

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_EOL)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_INDENT)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        result = prikazySekv(compilerData);

        if(result != 0)
        {
            return result;
        }

        GET_TOKEN;

        if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == ELSE){

            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_EOL)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        generateElseStart(numberOfIfs, compilerData->current_function);

        if(compilerData->token.tokenType == TOKEN_INDENT)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        result = prikazySekv(compilerData);

        if(result != 0)
        {
            return result;
        }

        GET_TOKEN;

        generateElseEnd(numberOfIfs, compilerData->current_function);

        return 0;
        //generate label end
    }
    //PRIKAZ -> while VYRAZ : eol indent PRIKAZY_SEK dedent
    else if(compilerData->token.keyword == WHILE && compilerData->token.tokenType == TOKEN_KEYWORD)
    {
        GET_TOKEN;
        int numberOfWhiles = compilerData->numberOfWhiles;

        compilerData->numberOfWhiles++;

        generateWhileLabel(numberOfWhiles, compilerData->current_function);

        result = solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

        if(result != 0)
        {
            return result;
        }

        generateWhileStart(numberOfWhiles, compilerData->current_function);

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_EOL)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_INDENT)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        result = prikazySekv(compilerData);

        if(result != 0)
        {
            return result;
        }

        GET_TOKEN;

        generateWhileEnd(numberOfWhiles, compilerData->current_function);

        return 0;
    }
    //PRIKAZ -> pass eol
    else if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == PASS)
    {
        GET_TOKEN;

        if(compilerData->token.tokenType == TOKEN_EOL)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        return 0;
    }
    //PRIKAZ -> NAVRAT_HODNOT eol
    else
    {
        result = navratHodnoty(compilerData);

        if(result != 0)
        {
            return result;
        }

        if(compilerData->token.tokenType == TOKEN_EOL)
        {
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        return 0;
    }
}

static int Hodnota(CompilerData *compilerData){

    printf("hodnota\n");
    printf("%d\n", compilerData->token.tokenType);

    symTableItem *item;

    bool global = true;

    static int actParam = 0;

    char paramType = compilerData->current_function->params->str[actParam];

    //Test, zda se jedná o vestavìnou funkci.
    if(compilerData->current_function->params->actIndex == 0)
    {
        if(compilerData->token.tokenType == TOKEN_INTEGER || compilerData->token.tokenType == TOKEN_DOUBLE || compilerData->token.tokenType == TOKEN_STRING)
        {
            generateFunctionParamsPass(actParam, &compilerData->token, global);
        }
        else if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
        {
            item = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &global);

            if(item == NULL)
            {
                return 3;
            }

            generateFunctionParamsPass(actParam, &compilerData->token, global);
        }
    }
    else
    {
        switch(compilerData->token.tokenType)
        {
            case TOKEN_IDENTIFIER:
                item = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &global);

                if(item == NULL)
                {
                    return 3;
                }

                switch(item->type)
                {
                    case INT:
                        if(paramType == 'i')
                        {
                            generateFunctionParamsPass(actParam, &compilerData->token, global);
                        }
                        else if(paramType == 'f')
                        {
                            compilerData->token.tokenType = TOKEN_DOUBLE;
                            compilerData->token.doubleValue = (double)compilerData->token.integerValue;
                            generateFunctionParamsPass(actParam, &compilerData->token, global);
                        }
                        else
                        {
                            return 4;
                        }
                        break;
                    case DOUBLE:
                        if(paramType == 'f')
                        {
                            generateFunctionParamsPass(actParam, &compilerData->token, global);
                        }
                        else
                        {
                            return 4;
                        }
                        break;
                    case STRING:
                        if(paramType == 's')
                        {
                            generateFunctionParamsPass(actParam, &compilerData->token, global);
                        }
                        else
                        {
                            return 4;
                        }
                        break;
                    default:
                        return 4;
                    }
                break;
            case TOKEN_INTEGER:
                if(paramType == 'i')
                {
                    generateFunctionParamsPass(actParam, &compilerData->token, global);
                }
                else if(paramType == 'f')
                {
                    compilerData->token.tokenType = TOKEN_DOUBLE;
                    compilerData->token.doubleValue = (double)compilerData->token.integerValue;
                    generateFunctionParamsPass(actParam, &compilerData->token, global);
                }
                else
                {
                    return 4;
                }
                break;
            case TOKEN_DOUBLE:
                if(paramType == 'f')
                {
                    generateFunctionParamsPass(actParam, &compilerData->token, global);
                }
                else
                {
                    return 4;
                }
                break;
            case TOKEN_STRING:
                if(paramType == 's')
                {
                    generateFunctionParamsPass(actParam, &compilerData->token, global);
                }
                else
                {
                    return 4;
                }
                break;
            default:
                return SYNTAX_ERROR;
        }
    }

    actParam++;

    if(actParam == compilerData->current_function->numberOfParams)
    {
        actParam = 0;
    }

    return 0;
}

static int dalsiHodnota (CompilerData *compilerData)
{
    printf("dalsiHodnota\n");

    static int numOfParams = 1;

    if(compilerData->token.tokenType == TOKEN_COLON)
    {
        if(numOfParams == compilerData->current_function->numberOfParams)
        {
            return 5;
        }

        GET_TOKEN;

        result = Hodnota(compilerData);

        if(result != 0)
        {
            return result;
        }

        numOfParams++;

        GET_TOKEN;

        return dalsiHodnota(compilerData);
    }
    else if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
    {
        if(numOfParams == compilerData->current_function->numberOfParams)
        {
            GET_TOKEN;
            numOfParams = 1;
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else
    {
        return SYNTAX_ERROR;
    }
}

static int Hodnoty(CompilerData *compilerData)
{
    printf("hodnoty\n");

    addInstruction("CREATEFRAME\n");

    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
    {
        if(compilerData->current_function->numberOfParams != 0)
        {
            return 5;
        }
        else
        {   GET_TOKEN;
            return 0;
        }
    }
    else
    {
        result = Hodnota(compilerData);

        if(result != 0)
        {
            return SYNTAX_ERROR;
        }

        GET_TOKEN;

         return dalsiHodnota(compilerData);
    }
}


static int navratHodnoty (CompilerData *compilerData)
{
    printf("navratHodnoty %d %s\n", compilerData->token.tokenType, compilerData->token.stringValue->str);

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER || compilerData->token.tokenType == TOKEN_INTEGER
    || compilerData->token.tokenType == TOKEN_DOUBLE || compilerData->token.tokenType == TOKEN_STRING || (compilerData->token.tokenType == TOKEN_KEYWORD
     && strcmp(compilerData->token.stringValue->str, "None") == 0) || compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
    {
        if(compilerData->varToAssign != NULL)
        {
            result = solveExpr(&compilerData->token, compilerData->tablesStack, STStackSearch(compilerData->tablesStack, compilerData->varToAssign->key, NULL));
        }
        else
        {
            symTableItem temp;
            temp.type = EMPTY_TYPE;
            result = solveExpr(&compilerData->token, compilerData->tablesStack, &temp);
        }

        if(result != 0)
        {
            return result;
        }

        if(compilerData->varToAssign != NULL)
        {
            if(compilerData->global)
            {
                generateMoveVariableToVariable(compilerData->varToAssign->key, "%lastExpresionResult", GLOBAL_VAR, GLOBAL_VAR);
            }
            else
            {
                generateMoveVariableToVariable(compilerData->varToAssign->key, "%lastExpresionResult", LOCAL_VAR, GLOBAL_VAR);
            }
        }
    }
    else if(compilerData->token.tokenType == TOKEN_KEYWORD)
    {
        //NAVRAT_HODNOT -> inputs ( )
        if(compilerData->token.keyword == INPUTS)
        {
            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateRead(compilerData->varToAssign->key, GLOBAL_VAR, STRING);
                }
                else
                {
                    generateRead(compilerData->varToAssign->key, LOCAL_VAR, STRING);
                }

                compilerData->varToAssign->type = STRING;
            }
        }
        //NAVRAT_HODNOT -> inputf ( )
        else if(compilerData->token.keyword == INPUTF)
        {
            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateRead(compilerData->varToAssign->key, GLOBAL_VAR, DOUBLE);
                }
                else
                {
                    generateRead(compilerData->varToAssign->key, LOCAL_VAR, DOUBLE);
                }

                compilerData->varToAssign->type = DOUBLE;
            }
        }
        //NAVRAT_HODNOT -> inputi ( )
        else if(compilerData->token.keyword == INPUTI)
        {
            //CALL INPUTI
            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateRead(compilerData->varToAssign->key, GLOBAL_VAR, INT);
                }
                else
                {
                    generateRead(compilerData->varToAssign->key, LOCAL_VAR, INT);
                }

                compilerData->varToAssign->type = INT;
            }
        }
        //NAVRAT_HODNOT -> print ( HODNOTY )
        else if(compilerData->token.keyword == PRINT)
        {
            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                while (compilerData->token.tokenType != TOKEN_RIGHT_BRACKET)
                {
                    GET_TOKEN;

                    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
                    {
                        symTableItem *temp;
                        bool global;
                        temp = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &global);
                        if(temp == NULL)
                        {
                            return SEM_ERROR_DEF;
                        }
                        else
                        {
                            if(global)
                            {
                                generateWrite(temp->key, GLOBAL_VAR);
                            }
                            else
                            {
                                generateWrite(temp->key, LOCAL_VAR);
                            }
                        }
                    }
                    else if(compilerData->token.tokenType == TOKEN_INTEGER || compilerData->token.tokenType == TOKEN_DOUBLE || compilerData->token.tokenType == TOKEN_STRING)
                    {
                        generateWriteValue(&compilerData->token);
                    }
                    else if((compilerData->token.tokenType == TOKEN_KEYWORD) && (compilerData->token.keyword == NONE))
                    {
                        addInstruction("WRITE string@None\n");
                    }
                    else
                    {
                        return SYNTAX_ERROR;
                    }

                    GET_TOKEN;

                    if(compilerData->token.tokenType != TOKEN_RIGHT_BRACKET && compilerData->token.tokenType != TOKEN_COLON)
                    {
                        return SYNTAX_ERROR;
                    }
                    if(compilerData->token.tokenType == TOKEN_COLON)
                    {
                        addInstruction("WRITE string@\\032\n");
                    }

                }
                addInstruction("WRITE string@\\010\n");
                GET_TOKEN;
                return 0;
            }
        }
        //NAVRAT_HODNOT -> len ( HODNOTY )
        else if(compilerData->token.keyword == LEN)
        {
            compilerData->current_function = STInsert(compilerData->globalTable, "len");

            if(compilerData->current_function == NULL)
            {
                return INTERNAL_ERROR;
            }

            if(compilerData->current_function->defined == false)
            {
                compilerData->current_function->numberOfParams = 1;
                compilerData->current_function->function = true;
                compilerData->current_function->defined = true;

                if(!STAddParam(compilerData->current_function, STRING))
                {
                    return INTERNAL_ERROR;
                }
            }

            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            result = Hodnoty(compilerData);

            if(result != 0)
            {
                return result;
            }

            generateCall("len");

            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", GLOBAL_VAR, TEMP_VAR);
                }
                else
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", LOCAL_VAR, TEMP_VAR);
                }

                compilerData->varToAssign->type = INT;
            }
        }
        //NAVRAT_HODNOT -> substr ( HODNOTY )
        else if(compilerData->token.keyword == SUBSTR)
        {
            compilerData->current_function = STInsert(compilerData->globalTable, "substr");

            if(compilerData->current_function == NULL)
            {
                return INTERNAL_ERROR;
            }

            if(compilerData->current_function->defined == false)
            {
                compilerData->current_function->numberOfParams = 3;
                compilerData->current_function->function = true;
                compilerData->current_function->defined = true;

                if(!STAddParam(compilerData->current_function, STRING))
                {
                    return INTERNAL_ERROR;
                }

                if(!STAddParam(compilerData->current_function, INT))
                {
                    return INTERNAL_ERROR;
                }

                if(!STAddParam(compilerData->current_function, INT))
                {
                    return INTERNAL_ERROR;
                }
            }

            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            result = Hodnoty(compilerData);

            if(result != 0)
            {
                return result;
            }

            generateCall("substr");

            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", GLOBAL_VAR, TEMP_VAR);
                }
                else
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", LOCAL_VAR, TEMP_VAR);
                }

                compilerData->varToAssign->type = STRING;
            }
        }
        //NAVRAT_HODNOT -> chr ( HODNOTY )
        else if(compilerData->token.keyword == CHR)
        {
            compilerData->current_function = STInsert(compilerData->globalTable, "chr");

            if(compilerData->current_function == NULL)
            {
                return INTERNAL_ERROR;
            }

            if(compilerData->current_function->defined == false)
            {
                compilerData->current_function->numberOfParams = 1;
                compilerData->current_function->function = true;
                compilerData->current_function->defined = true;

                if(!STAddParam(compilerData->current_function, INT))
                {
                    return INTERNAL_ERROR;
                }
            }

            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            result = Hodnoty(compilerData);

            if(result != 0)
            {
                return result;
            }

            generateCall("chr");
            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", GLOBAL_VAR, TEMP_VAR);
                }
                else
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", LOCAL_VAR, TEMP_VAR);
                }

                compilerData->varToAssign->type = STRING;
            }

        }
        //NAVRAT_HODNOT -> ord ( HODNOTY )
        else if(compilerData->token.keyword == ORD)
        {
            compilerData->current_function = STInsert(compilerData->globalTable, "ord");

            if(compilerData->current_function == NULL)
            {
                return INTERNAL_ERROR;
            }

            if(compilerData->current_function->defined == false)
            {
                compilerData->current_function->numberOfParams = 1;
                compilerData->current_function->function = true;
                compilerData->current_function->defined = true;

                if(!STAddParam(compilerData->current_function, STRING))
                {
                    return INTERNAL_ERROR;
                }

                if(!STAddParam(compilerData->current_function, INT))
                {
                    return INTERNAL_ERROR;
                }
            }

            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else
            {
                return SYNTAX_ERROR;
            }

            result = Hodnoty(compilerData);

            if(result != 0)
            {
                return result;
            }

            generateCall("ord");

            if(compilerData->varToAssign != NULL)
            {
                if(compilerData->global)
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", GLOBAL_VAR, TEMP_VAR);
                }
                else
                {
                    generateMoveVariableToVariable(compilerData->varToAssign->key, "%retval", LOCAL_VAR, TEMP_VAR);
                }

                compilerData->varToAssign->type = INT;
            }
        }
    }
    else
    {
        return SYNTAX_ERROR;
    }

    return 0;
}



static int Parametry(CompilerData *compilerData)
{
    printf("parametry\n");

    compilerData->localTable = STStackPush(compilerData->tablesStack);

    if(compilerData->localTable == NULL)
    {
        return INTERNAL_ERROR;
    }

    if (compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        if(STInsert(compilerData->localTable, compilerData->token.stringValue->str) == NULL)
        {
            return INTERNAL_ERROR;
        }

        compilerData->current_function->numberOfParams++;

        GET_TOKEN;
        return dalsiParametr(compilerData);
    }
    else if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
    {
        GET_TOKEN;
        return 0;
    }
    else
    {
        return SYNTAX_ERROR;
    }
}


static int dalsiParametr (CompilerData *compilerData)
{
    printf("dalsiParametr\n");

    if(compilerData->token.tokenType == TOKEN_COLON)
    {
        GET_TOKEN;

        if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
        {
            if(STInsert(compilerData->localTable, compilerData->token.stringValue->str) == NULL)
            {
                return INTERNAL_ERROR;
            }

            compilerData->current_function->numberOfParams++;

            GET_TOKEN;
            return dalsiParametr(compilerData);
        }
        else
        {
            return SYNTAX_ERROR;
        }
    }
    else if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
    {
        GET_TOKEN;
        return 0;
    }
    else
    {
        return SYNTAX_ERROR;
    }
}


int main(int argc, char *argv[])
{
    DS dynamicString;

    DSInit(&dynamicString);
    setDynamicString(&dynamicString);

    CompilerData compilerData;

    compilerData.indentationStack = malloc(sizeof(Stack));

    initStack(compilerData.indentationStack);
    stackPush(compilerData.indentationStack, 0);
    setIndentationStack(compilerData.indentationStack);

    compilerData.tablesStack = malloc(sizeof(STStack));

    compilerData.token.stringValue = malloc(sizeof(DS));
    DSInit(compilerData.token.stringValue);

    compilerDataInit(&compilerData);

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

    getToken(&compilerData.token);

    result = Prog(&compilerData);

    if(result != 0)
    {
        switch (result)
        {
            case LEX_ERROR:
                printf("LEX ERROR");
                break;
            case SYNTAX_ERROR:
                printf("SYNTAX ERROR");
                break;
            case SEM_ERROR_DEF:
                printf("SEMANTIC ERROR UNDEFINED");
            case SEM_ERROR_COMPATIBILITY:
                printf("SEMANTIC ERROR TYPES NOT COMPATIBLE");
                break;
            case SEM_ERROR_PARAMS:
                printf("SEMANTIC ERROR WRONG NUMBER OF PARAMS");
                break;
            case SEM_ERROR:
                printf("SEMANTIC ERROR");
                break;
            case SEM_ERROR_DIV_ZERO:
                printf("ZERO DIVISION ERROR");
                break;
            case INTERNAL_ERROR:
                printf("ERROR INTERNAL");
                break;
            default:
                break;
        }
        return result;
    }

    printf("%s", dynamicString.str);
    fclose(sourceCode);
}
