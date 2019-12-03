#include "compiler.h"


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

    //compilerData->globalTable = malloc(sizeof(symTable));
    //compilerData->localTable = malloc(sizeof(symTable));

    //inicializace indentStacku, prvotni pushnuti 0
    initStack(compilerData->IndentationStack);
    stackPush(compilerData->IndentationStack, 0);

    compilerData->token.stringValue = malloc(sizeof(DS));
    DSInit(compilerData->token.stringValue);

    compilerData->current_id = NULL;
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
        Prikaz(compilerData);
        if(compilerData->token.tokenType == TOKEN_EOL ){
            return Prog(compilerData);
        }
        return 2;

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

        prikazyFunkce(compilerData);
    }
    else return 2;

    //popnuti tabuky symbolu ze zasobniku

    if (compilerData->token.tokenType == TOKEN_DEDENT){
        compilerData->inFunction = false;
        return Prog(compilerData);
    }
    else return 2;


}

static int prikazyFunkce(CompilerData * compilerData){
    //PRIKAZY_FUNKCE -> eps
    if(compilerData->token.tokenType == TOKEN_DEDENT){
        return 0;
    }
    //PRIKAZY_FUNKCE -> PRIKAZY_SEKV
    else{
        return prikazySekv(compilerData);
    }
}

static int prikazySekv(CompilerData *compilerData){
    //PRIKAZY_SEK -> PRIKAZ eol DALSI_PRIKAZ

    Prikaz(compilerData);
    if(compilerData->token.tokenType == TOKEN_EOL){
        return dalsiPrikaz(compilerData);
    }
    else return 2;
}

static int dalsiPrikaz(CompilerData *compilerData){
    //DEDENT = konec aktualni sekvence prikazu
    //DALSI_PRIKAZ -> eps
    if (compilerData->token.tokenType == TOKEN_DEDENT)
    {

        return 0;
    }
    else if(compilerData->token.tokenType == TOKEN_EOL || compilerData->token.tokenType == TOKEN_EOF)
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
    }
}

static int fceDefNeboVest(CompilerData *compilerData)
{
    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {
        GET_TOKEN;
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET)
        {

        }
    }
    else
    {
        navratHodnoty(compilerData);
    }

}


static int Prikaz (CompilerData *compilerData){

    int tableIndex = 0;

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER)
    {

        symTableItem *temp = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &tableIndex);
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
                printf("err\n");
                return SYNTAX_ERROR;
            }
            volaniNeboPrirazeni(compilerData);
        }
        else
        {
            strcpy(compilerData->varToAssign, temp->key);            
            compilerData->global = !tableIndex;

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
    else
    {
        navratHodnoty(compilerData);
        
        return 0;
    }
    

    //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE

     //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKONANI FUNKCE
    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == PRINT){
         //TODO generateWrite()

        GET_TOKEN;

        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
           // puvodni - dle me nesmysl Hodnoty(compilerData);
            //solveExpr(&compilerData->token);
            //print DS ulozeny v compilerData


            //VOLAL BYCH Hodnoty A KAZDOU TISKL. ZA POSLEDNI VYTISK KONCE RADKU//

            while(compilerData->token.tokenType != TOKEN_RIGHT_BRACKET){

                GET_TOKEN;

                //generate PRINT VAL


            }

            //generate PRINT \n
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
            GET_TOKEN;
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){
                    dalsiPrikaz(compilerData);
        }
        else return 2;

    }

    //V PRIPADE, ZE JE PRVNI ID
    else if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

        compilerData->current_id = compilerData->token.stringValue->str;

        //Najdeme ID v symtablu, pokud ne, vlozime jej
        if( ( !compilerData->inFunction ) && ( !STSearch(&compilerData->localTable, compilerData->current_id) ) ){

            compilerData->current_id = STInsert(&compilerData->localTable, compilerData->current_id)->key;

        }
        else if(!STSearch(&compilerData->globalTable, compilerData->current_id)){
                compilerData->current_id = STInsert(&compilerData->globalTable, compilerData->current_id)->key;
        }


        GET_TOKEN;

        volaniNeboPrirazeni(compilerData);


     }

    else if(compilerData->token.keyword == IF && compilerData->token.tokenType == TOKEN_KEYWORD){
           GET_TOKEN;

           //generate IF

           //solveExpr(&compilerData->token);
           //generate podmineny jump
           GET_TOKEN;

           if(compilerData->token.tokenType == TOKEN_COLON){
             GET_TOKEN;

           }
           else return 2;

           if(compilerData->token.tokenType == TOKEN_EOL){
                GET_TOKEN;
           }
           else return 2;

           if(compilerData->token.tokenType == TOKEN_INDENT){
               //generate true exp sekvence
                GET_TOKEN;

           }
           else return 2;

           //vytvoreni tabulky symbolu a pushnuti na zasobnik tabulek symbolu

           Prikaz(compilerData);

           if(compilerData->token.tokenType == TOKEN_DEDENT){

                GET_TOKEN;
           }
           else return 2;

           //generate jump end

            if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == ELSE){

                GET_TOKEN;
           }
           else return 2;

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
           else return 2;

           //vytvoreni tabulky symbolu a pushnuti na zasobnik symbolu. v else se tvori nova tabulka symbolu :-) aspon myslim podle Discordu

           Prikaz(compilerData);

           //popnuti tabuky symbolu ze zasobniku

            if(compilerData->token.tokenType == TOKEN_DEDENT){

                GET_TOKEN;
           }
           else return 2;

           //generate label end
    }

    else if(compilerData->token.keyword == WHILE && compilerData->token.tokenType == TOKEN_KEYWORD){
        GET_TOKEN;



        //generate while start

        //generate WHILE JUMP LABEL

        solveExpr(&compilerData->token, compilerData->tablesStack, NULL);

        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){

            GET_TOKEN;
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){

            GET_TOKEN;
        }
         else return 2;

        //generate podm jump end while


        if(compilerData->token.tokenType == TOKEN_INDENT){

            GET_TOKEN;
        }
       else return 2;

        //vytvoreni tabulky symbolu a pushnuti na zasobnik symbolu. v else se tvori nova tabulka symbolu :-) aspon myslim podle Discordu

       Prikaz(compilerData);

       //generate jump while start

       //popnuti tabuky symbolu ze zasobniku
       if(compilerData->token.tokenType == TOKEN_DEDENT){
            GET_TOKEN;
        }
        else return 2;



        //generate while end
    }

    //PRIKAZ -> pass
    else if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == PASS){
        GET_TOKEN;
    }

    else if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == RETURN){
        GET_TOKEN;

        //solveExpr(&compilerData->token);

        //generate return vysledku expressionu
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
                        int index = 0;
                        temp = STStackSearch(compilerData->tablesStack, compilerData->token.stringValue->str, &index);
                        if(temp == NULL)
                        {
                            return SEM_ERROR_DEF;
                        }
                        else
                        {
                            if(index == 0)
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

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER || compilerData->token.tokenType == TOKEN_INTEGER 
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
        printf("err\n");
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

    compilerData->IndentationStack = (Stack *) malloc(sizeof(Stack));

    compilerData->tablesStack = (STStack *) malloc(sizeof(STStack));

    compilerDataInit(compilerData);

    setIndentationStack(compilerData->IndentationStack);


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


    Prog(compilerData);


    printf("%s", dynamicString.str);
    fclose(sourceCode);



}


  /*  while(token->tokenType != TOKEN_EOF)
    {
        if(getToken(token, compilerData->indentationStack) == ERROR_LEX)
        {
            printf("LEX ERROR");
            free(token);
            fclose(sourceCode);

            return -1;
        }
        if(token->tokenType == TOKEN_KEYWORD && token->keyword == PRINT)
        {
            getToken(token, false, compilerData->indentationStack);
            if(token->tokenType == TOKEN_LEFT_BRACKET)
            {
                while ( getToken(token, false, compilerData->indentationStack ) ){//prochazim termy a ukladam je do tisknuteho retezce dokud nenarazim na pravou zavorku
                    if ( token->tokenType == TOKEN_RIGHT_BRACKET ){
                        break;
                    }

                    //do dynamicStringu ukladam hodnoty jednotlivych termu a generuji vysledny tisknutelny retezec
                    //jak je uluzena hodnota tokenu po prvotni analyze? vse ve stringu?
                    else if ( token->tokenType == TOKEN_STRING){
                        dynamicString.str = DSAddStr(dynamicString.str, token->stringValue);
                    }
                    else if( token->tokenType == TOKEN_DOUBLE){
                        dynamicString.str = DSAddStr(dynamicString.str, doubleToString( token->doubleValue));
                    }
                     else if( token->tokenType == TOKEN_INTEGER)
                    {
                        dynamicString.str = DSAddStr(dynamicString.str, token->integerValue);
                    }
                    else if( token->tokenType == TOKEN_IDENTIFIER){
                        //patri sem hodnota z tabluky symbolu
                    }
                    else{
                        printf("SYNTAX ERROR");
                        return -6;
                    }

                    getToken(token, false, compilerData->indentationStack );
                     if ( token->tokenType == TOKEN_RIGHT_BRACKET ){
                        break;
                    }
                    else if ( token->tokenType == TOKEN_COLON){
                        ;
                    }
                    else{
                        printf("SYNTAX ERROR");
                        return -6;
                    }
                }

                if (token->tokenType == TOKEN_RIGHT_BRACKET){
                        generateWriteValue("s", token->stringValue->str);
                }
                else{
                    printf("SYNTAX ERROR");
                    return -6;
                }


            }
            else
            {
                printf("SYNTAX ERROR");
                return -6;
            }

        }
        if(token->tokenType == TOKEN_KEYWORD && token->keyword == DEF)
        {
            getToken(token, false, compilerData->indentationStack);
            if(token->tokenType == TOKEN_IDENTIFIER)
            {
                generateFunctionStart(token->stringValue->str);
            }
            else
            {
                printf("SYNTAX ERROR");
                return -6;
            }

        }
        if(token->tokenType == TOKEN_KEYWORD && token->keyword == RETURN)
        {
            getToken(token, false, compilerData->indentationStack);
            generateFunctionReturn("test", token);
        }
    }*/



