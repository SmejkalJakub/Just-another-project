#include <stdio.h>
#include "compiler.h"


/*char* doubleToString(double convertedNumber){

    char doubleInString[50];
    snprintf( doubleInString, 50, "%f", convertedNumber);

    return doubleInString;
}*/


int compilerDataInit(CompilerData* compilerData){

    STInit(&compilerData->localTable);
    STInit(&compilerData->globalTable);

    //compilerData->globalTable = malloc(sizeof(symTable));
    //compilerData->localTable = malloc(sizeof(symTable));

    //inicializace indentStacku, prvotni pushnuti 0
    initStack(compilerData->IndentationStack);
    stackPush(compilerData->IndentationStack, 0);

    compilerData->printedValues = malloc(sizeof(DS));
    DSInit(compilerData->printedValues);


    compilerData->token.stringValue = malloc(sizeof(DS));
    DSInit(compilerData->token.stringValue);

    compilerData->current_id = NULL;
    compilerData -> inFunction = NULL;
	compilerData -> inWhileOrIf = NULL;

}


static int Prog (CompilerData *compilerData){

    getToken(&compilerData->token, *(&compilerData->IndentationStack));

   // PROG → def id PARAMS eol indent LIST_COMMAND_FUNC eol dedent PROG
    if ((compilerData->token.tokenType == TOKEN_KEYWORD) && (compilerData->token.keyword == DEF)){

        compilerData->inFunction = true;

        getToken(&compilerData->token, *(&compilerData->IndentationStack));
    }

     //PROG →  eol PROG
    else if (compilerData->token.tokenType == TOKEN_EOL){
        return Prog(compilerData);
    }

    //PROG →  EOF
    else if (compilerData->token.tokenType == TOKEN_EOF){
        return 0;
    }

    //PROG →  COMMANDS eol PROG
    else{
        Commands(compilerData);
        return Prog(compilerData);
    }

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER){
        compilerData->current_id = STInsert(&compilerData->localTable, compilerData->token.stringValue->str)->key;
        STSearch(&compilerData->globalTable, compilerData->current_id)->function = true;
        generateFunctionStart(compilerData->current_id);
        getToken(&compilerData->token, *(&compilerData->IndentationStack));
    }
    else return 2;

    if (compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));
        Params(compilerData);
    }
    else return 2;

    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));
    }
    else return 2;

    if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));
    }
    else return 2;

    if((compilerData->token.tokenType == TOKEN_EOL)){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));
    }
    else return 2;

    if(compilerData->token.tokenType == TOKEN_INDENT){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));
        Commands(compilerData);
    }
    else return 2;

    if (compilerData->token.tokenType == TOKEN_DEDENT){
        compilerData->inFunction = false;
        return Prog(compilerData);
    }
    else return 2;


}




static int Commands (CompilerData *compilerData){


    //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE
    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == INPUTS){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
                getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
            getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){
            anotherCommand(compilerData);
        }
        else return 2;


    }

     //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE
   if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == INPUTF){
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
            getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
            //TODO generateRead()
            getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){
            anotherCommand(compilerData);
        }
        else return 2;
    }

    //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE

    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == INPUTI){
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
            getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
            //TODO generateRead()
            getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){
            anotherCommand(compilerData);
        }
        else return 2;
    }

     //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKONANI FUNKCE
    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == PRINT){
         //TODO generateWrite()
        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
           // puvodni - dle me nesmysl Values(compilerData);
            solveExpr(&compilerData->token);
            //print DS ulozeny v compilerData
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
            getToken(&compilerData->token, *(&compilerData->IndentationStack));
        }
        else return 2;

        if(compilerData->token.tokenType == TOKEN_EOL){
                    anotherCommand(compilerData);
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


        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        callOrAssign(compilerData);


     }

    else if(compilerData->token.keyword == IF && compilerData->token.tokenType == TOKEN_KEYWORD){
           getToken(&compilerData->token, *(&compilerData->IndentationStack));
    }


}

static int defFunctionOrIncluded(CompilerData *compilerData){



    //v pripade, ze je prvni token identifikator fce
    if (compilerData->token.tokenType == TOKEN_IDENTIFIER &&
        ( (STSearch(&compilerData->localTable, compilerData->token.stringValue->str)->function) ||
        (STSearch(&compilerData->globalTable, compilerData->token.stringValue->str)->function) ) )
    {

        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        Values(compilerData);
    }

    //jinak prirazujeme hodnotu vyrazu
    else{
        commandValue(compilerData);
    }


}

static int callOrAssign(CompilerData *compilerData){

    if(compilerData->token.tokenType == TOKEN_EQUALS){

         getToken(&compilerData->token, *(&compilerData->IndentationStack));

         defFunctionOrIncluded(*(&compilerData));


    }
    else if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){

        //TODO functionCall

        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        Values(*(&compilerData));

        if (compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
            return 0;
        }
        else return 2;
    }
    else return 2;
}
static int Values(CompilerData *compilerData){

    getToken(&compilerData->token, *(&compilerData->IndentationStack));

    int loadedParametrs = 0;

        while(compilerData->token.tokenType != TOKEN_RIGHT_BRACKET){

            loadedParametrs++;

            getToken(&compilerData->token, *(&compilerData->IndentationStack));

            //VALUES ->  VAL VALUES
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

                //VALUES ->  eps
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

static int anotherCommand (CompilerData *compilerData){
    getToken(&compilerData->token, *(&compilerData->IndentationStack));

}

static int commandValue (CompilerData *compilerData){
    if(compilerData->token.tokenType == TOKEN_KEYWORD){
        if (compilerData->token.keyword == INPUTS){
            //generateRead();
        }
        else if (compilerData->token.keyword == INPUTF){
            //generateRead();
        }
        else if(compilerData->token.keyword == INPUTI){
            //generateRead();
        }
        else if(compilerData->token.keyword == PRINT){
            //generateWrite();
        }

        else if(compilerData->token.keyword == LEN){
            /*
            len(s) – Vrátí délku (počet znaků) řetězce zadaného jediným parametrem 𝑠. Např.
            len('x\nz') vrací 3.
            */
        }
        else if(compilerData->token.keyword == SUBSTR){
            /*
            substr(s, i, n) – Vrátí podřetězec zadaného řetězce 𝑠. Druhým parametrem 𝑖 je dán
            začátek požadovaného podřetězce (počítáno od nuly) a třetí parametr 𝑛 určuje délku
            podřetězce. Je-li index 𝑖 mimo meze 0 až len(s) nebo 𝑛 < 0, vrací funkce None.
            Jeli 𝑛 > len(s)−𝑖, jsou jako řetězec vráceny od 𝑖-tého znaku všechny zbývající znaky řetězce 𝑠.

            */

        }
        else if(compilerData->token.keyword == CHR){

            /*
            – Vrátí jednoznakový řetězec se znakem, jehož ASCII kód je zadán parametrem 𝑖.
            Případ, kdy je 𝑖 mimo interval [0; 255], vede na běhovou chybu při práci s řetězcem
            */

        }
        else if(compilerData->token.keyword == ORD){
            /*
                Vrátí ordinální hodnotu (ASCII) znaku na pozici 𝑖 v řetězci 𝑠. Je-li
                pozice mimo meze řetězce (0 až len(s) - 1), vrací None
            */
        }


    }

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER ){

            getToken(&compilerData->token, *(&compilerData->IndentationStack));




    }
    return 0;

}



static int Params(CompilerData *compilerData){
    if (compilerData->token.tokenType == TOKEN_IDENTIFIER){

        STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        anotherParam(compilerData);

    }
    else{
        return 2;
    }

}



static int anotherParam (CompilerData *compilerData){

    if(compilerData->token.tokenType == TOKEN_COLON){
        getToken(&compilerData->token, *(&compilerData->IndentationStack));

        if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

            STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
            getToken(&compilerData->token, *(&compilerData->IndentationStack));

            return anotherParam(compilerData);
        }
        else{
            return 2;
        }
    }
    else{
       return 0;
    }
}



static int listCommandFunction(){

}

/*int main(int argc, char *argv[])
{
    DS dynamicString;

    DSInit(&dynamicString);
    setDynamicString(&dynamicString);

    CompilerData *compilerData;
     compilerDataInit(&compilerData);


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

    free(&compilerData->token);
    fclose(sourceCode);

    generateFunctionEnd("test");

    printf("%s", dynamicString.str);

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



