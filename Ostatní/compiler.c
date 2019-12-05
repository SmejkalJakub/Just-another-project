#include "compiler.h"


int result;

#define GET_TOKEN                                                                 \
        getToken(&compilerData->token)


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

    compilerData->current_id = NULL;

    if((compilerData->token.stringValue = malloc(sizeof(DS))) == NULL)
    {
        return INTERNAL_ERROR;
    }

    DSInit(compilerData->token.stringValue);

    compilerData -> inFunction = NULL;
	compilerData -> inWhileOrIf = NULL;

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

        compilerData->current_id = newTabItem->key;

        newTabItem->function = true;

        generateFunctionStart(compilerData->current_id);

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

    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
    {
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
        //vytvoreni tabulky symbolu na zasobnik tabulek symbolu

        prikazySekv(compilerData);
    }
    else
    {
        return SYNTAX_ERROR;
    }

    //popnuti tabuky symbolu ze zasobniku

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
    //VOLANI_NEBO_PRIRAZENI -> = FCE_DEF_NEBO_VEST
    if(compilerData->token.tokenType == TOKEN_ASSIGN_SIGN)
    {
        GET_TOKEN;
        return fceDefNeboVest(compilerData);
    }
    //VOLANI_NEBO_PRIRAZENI -> ( HODNOTY )
    else if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
    {
        GET_TOKEN;
        return Hodnoty(compilerData);
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
    printf("prikaz\n");
    bool global;
    //PRIKAZ -> id VOLANI_NEBO_PRIRAZENI eol
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        compilerData->varToAssign = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &global);
        if(compilerData->varToAssign == NULL)
        {
            if((compilerData->varToAssign = STInsert(compilerData->tablesStack->top->symTablePtr, compilerData->token.stringValue->str)) == NULL)
            {
                return INTERNAL_ERROR;
            }

            if(!compilerData->inFunction && !compilerData->inWhileOrIf)
            {
                compilerData->global = true;
            }
            else
            {
                compilerData->global = false;
            }

            if(compilerData->global)
            {
                generateVariableDef(compilerData->varToAssign->key, GLOBAL_VAR);
            }
            else
            {
                generateVariableDef(compilerData->varToAssign->key, LOCAL_VAR);
            }
        }
        else
        {
            compilerData->global = global;
        }

        GET_TOKEN;

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

                generateFunctionReturn(compilerData->current_id, false);

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

            generateFunctionReturn(compilerData->current_id, true);

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

        generateIfStart(numberOfIfs, compilerData->current_id);

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

        generateElseStart(numberOfIfs, compilerData->current_id);

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

        generateElseEnd(numberOfIfs, compilerData->current_id);

        return 0;
        //generate label end
    }
    //PRIKAZ -> while VYRAZ : eol indent PRIKAZY_SEK dedent
    else if(compilerData->token.keyword == WHILE && compilerData->token.tokenType == TOKEN_KEYWORD)
    {
        GET_TOKEN;

        int numberOfWhiles = compilerData->numberOfWhiles;

        compilerData->numberOfWhiles++;

        generateWhileLabel(numberOfWhiles, compilerData->current_id);

        result = solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

        if(result != 0)
        {
            return result;
        }

        generateWhileStart(numberOfWhiles, compilerData->current_id);

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

        generateWhileEnd(numberOfWhiles, compilerData->current_id);

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
    int loadedParametrs = 0;

    while(compilerData->token.tokenType != TOKEN_RIGHT_BRACKET)
    {
        loadedParametrs++;

        GET_TOKEN;

        //Hodnoty ->  VAL Hodnoty
        switch(compilerData->token.tokenType)
        {
            //VAL ->  id
            case TOKEN_IDENTIFIER:
                //pristup na polozku v symTablu
                //pristup na typ polozky a opet stejny case jako tento, akorat bez case ID
                switch (STSearch(&compilerData->localTable, compilerData->token.stringValue->str)->type)
                {
                    //VAL ->  integer
                    case TOKEN_INTEGER:
                        //kontrola, zda-li odpovida typ parametru fce
                        //pokud string - chyba
                        //pokud double - nutno pretypovat
                        break;
                    //VAL ->  double
                    case TOKEN_DOUBLE:
                        //kontrola, zda-li odpovida typ parametru fce
                        //pokud string - chyba
                        //pokud int - nutno pretypovat
                        break;
                    //VAL ->  string
                    case TOKEN_STRING:
                        //kontrola, zda li odpovida parametru fce
                        //pokud ne - chyba
                        break;
                    default:
                        return 3;
                    }
                break;
            //VAL ->  integer
            case TOKEN_INTEGER:
                //kontrola, zda-li odpovida typ parametru fce
                //pokud string - chyba
                //pokud double - nutno pretypovat
                break;
            //VAL ->  double
            case TOKEN_DOUBLE:
                //kontrola, zda-li odpovida typ parametru fce
                //pokud string - chyba
                //pokud int - nutno pretypovat
                break;
            //VAL ->  string
            case TOKEN_STRING:
                //kontrola, zda li odpovida parametru fce
                //pokud ne - chyba
                break;
            //Hodnoty ->  eps
            case TOKEN_LEFT_BRACKET:
                    break;
            default:
                return SYNTAX_ERROR;
        }
    }

    //kontrola poctu parametru
    if(loadedParametrs != STSearch(compilerData->globalTable, compilerData->current_id)->numberOfParams)
    {
        return SYNTAX_ERROR;
    }

    return 0;
}

static int dalsiHodnota (CompilerData *compilerData)
{
    printf("dalsiHodnota\n");
    if(compilerData->token.tokenType == TOKEN_COLON){
        result = Hodnota(compilerData);

        if(result != 0)
        {
            return result;
        }

        return dalsiHodnota(compilerData);
    }
    else
    {
        return 0;
    }
}

static int Hodnoty(CompilerData *compilerData)
{
    printf("hodnoty\n");
    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
    {
        return 0;
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
    printf("navratHodnoty\n");
    if(compilerData->token.tokenType == TOKEN_KEYWORD)
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

            if(compilerData->global)
            {
                generateRead(compilerData->varToAssign->key, GLOBAL_VAR, STRING);
            }
            else
            {
                generateRead(compilerData->varToAssign->key, LOCAL_VAR, STRING);
            }

            STSearch(compilerData->tablesStack->top->symTablePtr, compilerData->varToAssign->key)->type = STRING;
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

            if(compilerData->global)
            {
                generateRead(compilerData->varToAssign->key, GLOBAL_VAR, DOUBLE);
            }
            else
            {
                generateRead(compilerData->varToAssign->key, LOCAL_VAR, DOUBLE);
            }

            STSearch(compilerData->tablesStack->top->symTablePtr, compilerData->varToAssign->key)->type = DOUBLE;
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

            if(compilerData->global)
            {
                generateRead(compilerData->varToAssign->key, GLOBAL_VAR, INT);
            }
            else
            {
                generateRead(compilerData->varToAssign->key, LOCAL_VAR, INT);
            }

            STSearch(compilerData->tablesStack->top->symTablePtr, compilerData->varToAssign->key)->type = INT;
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
            /*
            len(s) – Vrátí délku (počet znaků) řetězce zadaného jediným parametrem 𝑠. Např.
            len('x\nz') vrací 3.
            */
        }
        //NAVRAT_HODNOT -> substr ( HODNOTY )
        else if(compilerData->token.keyword == SUBSTR)
        {
            /*
            substr(s, i, n) – Vrátí podřetězec zadaného řetězce 𝑠. Druhým parametrem 𝑖 je dán
            začátek požadovaného podřetězce (počítáno od nuly) a třetí parametr 𝑛 určuje délku
            podřetězce. Je-li index 𝑖 mimo meze 0 až len(s) nebo 𝑛 < 0, vrací funkce None.
            Jeli 𝑛 > len(s)−𝑖, jsou jako řetězec vráceny od 𝑖-tého znaku všechny zbývající znaky řetězce 𝑠.
            */
        }
        //NAVRAT_HODNOT -> chr ( HODNOTY )
        else if(compilerData->token.keyword == CHR)
        {

            /*
            – Vrátí jednoznakový řetězec se znakem, jehož ASCII kód je zadán parametrem 𝑖.
            Případ, kdy je 𝑖 mimo interval [0; 255], vede na běhovou chybu při práci s řetězcem
            */

        }
        //NAVRAT_HODNOT -> ord ( HODNOTY )
        else if(compilerData->token.keyword == ORD)
        {
            /*
                Vrátí ordinální hodnotu (ASCII) znaku na pozici 𝑖 v řetězci 𝑠. Je-li
                pozice mimo meze řetězce (0 až len(s) - 1), vrací None
            */
        }
    }

    else if(compilerData->token.tokenType == TOKEN_IDENTIFIER || compilerData->token.tokenType == TOKEN_INTEGER
    || compilerData->token.tokenType == TOKEN_DOUBLE || compilerData->token.tokenType == TOKEN_STRING)
    {
        solveExpr(&compilerData->token, compilerData->tablesStack, STStackSearch(compilerData->tablesStack, compilerData->varToAssign->key, NULL));
        if(compilerData->global)
        {
            generateMoveVariableToVariable(compilerData->varToAssign->key, "%lastExpresionResult", GLOBAL_VAR, GLOBAL_VAR);
        }
        else
        {
            generateMoveVariableToVariable(compilerData->varToAssign->key, "%lastExpresionResult", LOCAL_VAR, GLOBAL_VAR);
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
    if (compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        //STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
        GET_TOKEN;

        return dalsiParametr(compilerData);
    }
    else
    {
        return 0;
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
            //STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
            GET_TOKEN;

            return dalsiParametr(compilerData);
        }
        else
        {
            return SYNTAX_ERROR;
        }
    }
    else
    {
       return 0;
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
        printf("ERR\n");
        return result;
    }

    printf("%s", dynamicString.str);
    fclose(sourceCode);
}
