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


    setIndentationStack(compilerData->indentationStack);

    compilerData->globalTable = STStackTop(compilerData->tablesStack);

    //compilerData->globalTable = malloc(sizeof(symTable));
    //compilerData->localTable = malloc(sizeof(symTable));

    //inicializace indentStacku, prvotni pushnuti 0

    compilerData->numberOfIfs = 0;
    compilerData->numberOfWhiles = 0;

    compilerData->current_id = "";

    compilerData->token.stringValue = malloc(sizeof(DS));
    DSInit(compilerData->token.stringValue);

    compilerData -> inFunction = NULL;
	compilerData -> inWhileOrIf = NULL;
}


static int Prog (CompilerData *compilerData){

    GET_TOKEN;

   // PROG → def id PARAMS eol indent LIST_COMMAND_FUNC eol dedent PROG
    if ((compilerData->token.tokenType == TOKEN_KEYWORD) && (compilerData->token.keyword == DEF)){

        compilerData->inFunction = true;

        GET_TOKEN;
    }

     //PROG →  eol PROG
    else if (compilerData->token.tokenType == TOKEN_EOL){
        return Prog(compilerData);
    }

    //PROG →  EOF
    else if (compilerData->token.tokenType == TOKEN_EOF)
    {
        return 0;
    }

    //PROG →  COMMANDS eol PROG
    else{
        result = Prikaz(compilerData);
        if(result != 0)
        {
            return result;
        }
        else if(compilerData->token.tokenType == TOKEN_EOF)
        {
            return 0;
        }
        else if(compilerData->token.tokenType == TOKEN_EOL){
            return Prog(compilerData);
        }
        else
        {
            result = Prikaz(compilerData);
            return Prog(compilerData);
        }
        return SYNTAX_ERROR;

    }

    //pokracovani def fce
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

        compilerData->current_id = STInsert(&compilerData->localTable, compilerData->token.stringValue->str)->key;

        STSearch(&compilerData->globalTable, compilerData->current_id)->function = true;

        generateFunctionStart(compilerData->current_id);

        GET_TOKEN;
    }
    else return 2;

    if (compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
        GET_TOKEN;
        Parametry(compilerData);
    }
    else return 2;

    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
        GET_TOKEN;
    }
    else return 2;

    if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){
        GET_TOKEN;
    }
    else return 2;

    if((compilerData->token.tokenType == TOKEN_EOL)){
        GET_TOKEN;
    }
    else return 2;

    if(compilerData->token.tokenType == TOKEN_INDENT){
        GET_TOKEN;
        //vytvoreni tabulky symbolu na zasobnik tabulek symbolu

        prikazySekv(compilerData);
    }
    else return SYNTAX_ERROR;

    //popnuti tabuky symbolu ze zasobniku

    if (compilerData->token.tokenType == TOKEN_DEDENT){
        compilerData->inFunction = false;
        return Prog(compilerData);
    }
    else return SYNTAX_ERROR;


}

static int prikazySekv(CompilerData *compilerData){
    //PRIKAZY_SEK -> PRIKAZ eol DALSI_PRIKAZ

    result = Prikaz(compilerData);
    
    if(result != 0)
    {
        return result;
    }


    if(compilerData->token.tokenType == TOKEN_EOL){
        return dalsiPrikaz(compilerData);
    }
    else
    {
        return SYNTAX_ERROR;
    }
}

static int dalsiPrikaz(CompilerData *compilerData){
    //DEDENT = konec aktualni sekvence prikazu
    //DALSI_PRIKAZ -> eps
    GET_TOKEN;

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

    if(compilerData->token.tokenType == TOKEN_ASSIGN_SIGN)
    {

        GET_TOKEN;
        fceDefNeboVest(compilerData);
        return 0;
    }
}

static int fceDefNeboVest(CompilerData *compilerData)
{
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER && (STSearch(compilerData->globalTable, compilerData->token.stringValue->str)->function))
    {
        GET_TOKEN;
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
        {

        }
    }
    else
    {
        result = navratHodnoty(compilerData);
        if(result != 0)
            return result;
    }
    return 0;

}


static int Prikaz (CompilerData *compilerData)
{
    bool global;

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        symTableItem *temp = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &global);
        if(temp == NULL)
        {
            strcpy(compilerData->varToAssign, compilerData->token.stringValue->str);
            STInsert(compilerData->tablesStack->top->symTablePtr, compilerData->varToAssign);

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
                generateVariableDef(compilerData->varToAssign, GLOBAL_VAR);
            }
            else
            {
                generateVariableDef(compilerData->varToAssign, LOCAL_VAR);
            }

            GET_TOKEN;
            if(compilerData->token.tokenType == TOKEN_EOL)
            {
                return SYNTAX_ERROR;
            }

            volaniNeboPrirazeni(compilerData);
        }
        else
        {
            strcpy(compilerData->varToAssign, temp->key);
            compilerData->global = global;

            GET_TOKEN;
            volaniNeboPrirazeni(compilerData);
        }
        return 0;
    }

    else if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == RETURN)
    {
        compilerData->inFunction = true;
        compilerData->current_id = "test";
        if(!compilerData->inFunction)
        {
            return SYNTAX_ERROR;
        }
        else
        {
            GET_TOKEN;
            if(compilerData->token.tokenType != TOKEN_EOL)
            {
                solveExpr(&compilerData->token, compilerData->tablesStack, NULL);
                generateFunctionReturn(compilerData->current_id, false);
                return 0;
            }
            generateFunctionReturn(compilerData->current_id, true);
            return 0;

        }
    }
    else if(compilerData->token.keyword == IF && compilerData->token.tokenType == TOKEN_KEYWORD)
    {
        int ifNumber = compilerData->numberOfIfs;
        int startIndent = compilerData->indentationStack->arr[compilerData->indentationStack->top];
        compilerData->numberOfIfs++;

        GET_TOKEN;

        result = solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

        if(result != 0)
        {
            return result;
        }

        generateIfStart(ifNumber, compilerData->current_id);

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){
            GET_TOKEN;
        }
        else return SYNTAX_ERROR;

        if(compilerData->token.tokenType == TOKEN_EOL){
            GET_TOKEN;
        }
        else return SYNTAX_ERROR;

        if(compilerData->token.tokenType == TOKEN_INDENT){
            //generate true exp sekvence
            GET_TOKEN;

        }
        else return SYNTAX_ERROR;
        result = prikazySekv(compilerData);


        if(result != 0)
        {
            return result;
        }
        if(compilerData->indentationStack->arr[compilerData->indentationStack->top] != startIndent)
        {
            GET_TOKEN;
            result = prikazySekv(compilerData);


            if(result != 0)
            {
                return result;
            }

        }


        if(compilerData->token.tokenType == TOKEN_DEDENT){

            GET_TOKEN;
        }
        else return SYNTAX_ERROR;
        



        if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == ELSE){
            
            GET_TOKEN;
        }
        else
        {
            return SYNTAX_ERROR;
        }

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){

            GET_TOKEN;
        }
        else return SYNTAX_ERROR;

        if(compilerData->token.tokenType == TOKEN_EOL){

            GET_TOKEN;
        }
        else return SYNTAX_ERROR;

        generateElseStart(ifNumber, compilerData->current_id);

        if(compilerData->token.tokenType == TOKEN_INDENT){

            GET_TOKEN;
        }
        else return SYNTAX_ERROR;

        result = prikazySekv(compilerData);

        if(result != 0)
        {
            return result;
        }

        if(compilerData->token.tokenType == TOKEN_DEDENT){

            GET_TOKEN;
        }
        else return SYNTAX_ERROR;
        printf("konec %d %d %s\n", ifNumber, compilerData->token.tokenType, compilerData->token.stringValue->str);

        generateElseEnd(ifNumber, compilerData->current_id);
        return 0;
        //generate label end
    }
    else if(compilerData->token.keyword == WHILE && compilerData->token.tokenType == TOKEN_KEYWORD){

        int whileNumber = compilerData->numberOfWhiles;
        compilerData->numberOfWhiles++;
        int startIndent = compilerData->indentationStack->arr[compilerData->indentationStack->top];


        GET_TOKEN;

        generateWhileLabel(whileNumber, compilerData->current_id);
        solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

        generateWhileStart(whileNumber, compilerData->current_id);

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){

            GET_TOKEN;
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){

            GET_TOKEN;
        }
            else return 2;

        if(compilerData->token.tokenType == TOKEN_INDENT){

            GET_TOKEN;
        }
        else return SYNTAX_ERROR;


        result = prikazySekv(compilerData);
        printf("jsem tudas dasd asd ays %d\n", whileNumber);

        if(result != 0)
        {
            return result;
        }

        if(compilerData->indentationStack->arr[compilerData->indentationStack->top] != startIndent)
        {
            result = prikazySekv(compilerData);

            if(result != 0)
            {
                return result;
            }
        }

        if(compilerData->token.tokenType == TOKEN_DEDENT){
            GET_TOKEN;
        }
        else return SYNTAX_ERROR;

        generateWhileEnd(whileNumber, compilerData->current_id);
        return 0;

    }
    //PRIKAZ -> pass
    else if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == PASS){
        GET_TOKEN;
    }

    else
    {
        result = navratHodnoty(compilerData);
        if(result != 0)
        {
            return result;
        }

        return 0;
    }
}

static int Hodnota(CompilerData *compilerData){

    int loadedParametrs = 0;

        while(compilerData->token.tokenType != TOKEN_RIGHT_BRACKET){

            loadedParametrs++;

            GET_TOKEN;

            //Hodnoty ->  VAL Hodnoty
            switch(compilerData->token.tokenType){

                //VAL ->  id
                case TOKEN_IDENTIFIER:

                        //pristup na polozku v symTablu
                        //pristup na typ polozky a opet stejny case jako tento, akorat bez case ID

                        switch (STSearch(&compilerData->localTable, compilerData->token.stringValue->str)->type){
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
                    return 2;

            }
        }


    //kontrola poctu parametru
    if ( loadedParametrs != STSearch(&compilerData->globalTable, compilerData->current_id)->numberOfParams){
        return 2;
    }
}

static int dalsiHodnota (CompilerData *compilerData){
    int finalState = 0;

    if(compilerData->token.tokenType == TOKEN_COLON){
        Hodnota(compilerData);
         return dalsiHodnota(compilerData);
    }
    else return finalState;
}

static int Hodnoty(CompilerData *compilerData){

    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
        return 0;
    }
    else{
        Hodnota(compilerData);

        GET_TOKEN;

        dalsiHodnota(compilerData);
    }

}


static int navratHodnoty (CompilerData *compilerData)
{

    if(compilerData->token.tokenType == TOKEN_KEYWORD)
    {
        if(compilerData->token.keyword == INPUTS)
        {
            //CALL INPUTS
            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
                    GET_TOKEN;
            }
            else return 2;

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
                GET_TOKEN;
            }
            else return 2;

            if(compilerData->token.tokenType == TOKEN_EOL)
            {
                if(compilerData->global)
                {
                    generateRead(compilerData->varToAssign, GLOBAL_VAR, STRING);
                }
                else
                {
                    generateRead(compilerData->varToAssign, LOCAL_VAR, STRING);
                }

                dalsiPrikaz(compilerData);
            }
            else return 2;
        }

        //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE
        if(compilerData->token.keyword == INPUTF)
        {
            //CALL INPUTF

            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
            {
                GET_TOKEN;
            }
            else return 2;

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET)
            {
                GET_TOKEN;
            }
            else return 2;

            if(compilerData->token.tokenType == TOKEN_EOL)
            {
                if(compilerData->global)
                {
                    generateRead(compilerData->varToAssign, GLOBAL_VAR, DOUBLE);
                }
                else
                {
                    generateRead(compilerData->varToAssign, LOCAL_VAR, DOUBLE);
                }
                dalsiPrikaz(compilerData);
            }
            else return 2;
        }

        //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE

        if(compilerData->token.keyword == INPUTI)
        {

            //CALL INPUTI

            GET_TOKEN;

            if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
                GET_TOKEN;
            }
            else return 2;

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
                GET_TOKEN;
            }
            else return 2;

            if(compilerData->token.tokenType == TOKEN_EOL){
                if(compilerData->global)
                {
                    generateRead(compilerData->varToAssign, GLOBAL_VAR, INT);
                }
                else
                {
                    generateRead(compilerData->varToAssign, LOCAL_VAR, INT);
                }
                STSearch(compilerData->tablesStack->top->symTablePtr, compilerData->varToAssign)->type = INT;
                dalsiPrikaz(compilerData);
            }
            else return 2;
        }

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
                    else if(compilerData->token.tokenType == TOKEN_INTEGER || compilerData->token.tokenType == TOKEN_DOUBLE
                            || compilerData->token.tokenType == TOKEN_STRING)
                            {
                                generateWriteValue(&compilerData->token);
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

        else if(compilerData->token.keyword == LEN)
        {
            /*
            len(s) – Vrátí délku (počet znaků) řetězce zadaného jediným parametrem 𝑠. Např.
            len('x\nz') vrací 3.
            */


        }
        else if(compilerData->token.keyword == SUBSTR)
        {
            /*
            substr(s, i, n) – Vrátí podřetězec zadaného řetězce 𝑠. Druhým parametrem 𝑖 je dán
            začátek požadovaného podřetězce (počítáno od nuly) a třetí parametr 𝑛 určuje délku
            podřetězce. Je-li index 𝑖 mimo meze 0 až len(s) nebo 𝑛 < 0, vrací funkce None.
            Jeli 𝑛 > len(s)−𝑖, jsou jako řetězec vráceny od 𝑖-tého znaku všechny zbývající znaky řetězce 𝑠.

            */

        }
        else if(compilerData->token.keyword == CHR)
        {

            /*
            – Vrátí jednoznakový řetězec se znakem, jehož ASCII kód je zadán parametrem 𝑖.
            Případ, kdy je 𝑖 mimo interval [0; 255], vede na běhovou chybu při práci s řetězcem
            */

        }
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
        solveExpr(&compilerData->token, compilerData->tablesStack, STStackSearch(compilerData->tablesStack, compilerData->varToAssign, NULL));
        if(compilerData->global)
        {
            generateMoveVariableToVariable(compilerData->varToAssign, "%lastExpresionResult", GLOBAL_VAR, GLOBAL_VAR);
        }
        else
        {
            generateMoveVariableToVariable(compilerData->varToAssign, "%lastExpresionResult", LOCAL_VAR, GLOBAL_VAR);
        }

    }
    else
    {
        printf("err %d\n", compilerData->token.stringValue->str);
        return SYNTAX_ERROR;
    }

    return 0;

}



static int Parametry(CompilerData *compilerData){
    if (compilerData->token.tokenType == TOKEN_IDENTIFIER){

        //STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
        GET_TOKEN;

        dalsiParametr(compilerData);

    }
    else{
        return 0;
    }

}



static int dalsiParametr (CompilerData *compilerData){

    if(compilerData->token.tokenType == TOKEN_COLON){
        GET_TOKEN;

        if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

            //STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
            GET_TOKEN;

            return dalsiParametr(compilerData);
        }
        else{
            return 2;
        }
    }
    else{
       return 0;
    }
}





int main(int argc, char *argv[])
{
    DS dynamicString;

    DSInit(&dynamicString);
    setDynamicString(&dynamicString);

    CompilerData *compilerData = (CompilerData *) malloc(sizeof(CompilerData));

    compilerData->indentationStack = (Stack *) malloc(sizeof(Stack));

    compilerData->tablesStack = (STStack *) malloc(sizeof(STStack));

    compilerDataInit(compilerData);


    compilerData->token.stringValue = malloc(sizeof(DS));
    DSInit(compilerData->token.stringValue);


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

    result = Prog(compilerData);

    if(result != 0)
    {
        printf("ERR\n");
        return result;
    }

    printf("%s", dynamicString.str);
    fclose(sourceCode);
}