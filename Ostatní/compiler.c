#include <stdio.h>
#include "compiler.h"


DS dynamicString;




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

    getToken(&compilerData->token, &compilerData->IndentationStack);

   // PROG → def id PARAMS eol indent LIST_COMMAND_FUNC eol dedent PROG
    if ((compilerData->token.tokenType == TOKEN_KEYWORD) && (compilerData->token.keyword == DEF)){

        compilerData->inFunction = true;

        getToken(&compilerData->token, &compilerData->IndentationStack);
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
        compilerData->current_id = STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
        generateFunctionStart(compilerData->current_id->key);
    }

    getToken(&compilerData->token, &compilerData->IndentationStack);

    if (compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
        getToken(&compilerData->token, &compilerData->IndentationStack);
        return (Params(compilerData));
    }

    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
        getToken(&compilerData->token, &compilerData->IndentationStack);
    }

    if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){
        getToken(&compilerData->token, &compilerData->IndentationStack);
    }

    if((compilerData->token.tokenType == TOKEN_EOL)){
        getToken(&compilerData->token, &compilerData->IndentationStack);
    }

    if(compilerData->token.tokenType == TOKEN_INDENT){
        getToken(&compilerData->token, &compilerData->IndentationStack);
        return Commands(&compilerData);
    }

    if (compilerData->token.tokenType == TOKEN_DEDENT){
        compilerData->inFunction = false;
        return Prog(compilerData);
    }


}




static int Commands (CompilerData *compilerData){
    int finalState;

    //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE
    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == INPUTS){
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
            getToken(&compilerData->token, &compilerData->IndentationStack);
            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){

                    //TODO generateRead()
                    getToken(&compilerData->token, &compilerData->IndentationStack);
                    if(compilerData->token.tokenType == TOKEN_EOL){
                        return anotherCommand(compilerData);
                    }
                    else{
                        return 2;
                    }
            }
            else{
                return 2;
            }
        }
        else{
            return 2;
        }
    }

     //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE
   if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == INPUTF){
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
            getToken(&compilerData->token, &compilerData->IndentationStack);
            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){

                    //TODO generateRead()
                    getToken(&compilerData->token, &compilerData->IndentationStack);
                    if(compilerData->token.tokenType == TOKEN_EOL){
                        anotherCommand(compilerData);
                    }
                    else{
                        return 2;
                    }
            }
            else{
                return 2;
            }
        }
        else{
            return 2;
        }
    }

    //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE

    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == INPUTI){
        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){
            getToken(&compilerData->token, &compilerData->IndentationStack);
            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){

                    //TODO generateRead()
                    getToken(&compilerData->token, &compilerData->IndentationStack);
                    if(compilerData->token.tokenType == TOKEN_EOL){
                        anotherCommand(compilerData);
                    }
                    else{
                        return 2;
                    }
            }
            else{
                return 2;
            }
        }
        else{
            return 2;
        }
    }

     //POKUD JE TOKEN KLICOVE SLOVO FCE, GENERUJE SE KOD PRO VYKOANI FUNKCE
    if(compilerData->token.tokenType == TOKEN_KEYWORD && compilerData->token.keyword == PRINT){
            //TODO generateWrite()
        getToken(&compilerData->token, &compilerData->IndentationStack);

        if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){

            return Values(compilerData);

            if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){

                getToken(&compilerData->token, &compilerData->IndentationStack);

                if(compilerData->token.tokenType == TOKEN_EOL){
                    anotherCommand(compilerData);
                }
                else{
                    return 2;
                }
            }
            else{
                return 2;
            }
            //generateWrite(&compilerData->printedValues, )
        }
    }

    //V PRIPADE, ZE JE PRVNI ID
    else if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

        //ULOZIME ID DO SYMTABLU
        if(!compilerData->inFunction){
            compilerData->current_id = STInsert(&compilerData->globalTable, compilerData->token.stringValue->str);
        }
        else{
            compilerData->current_id = STInsert(&compilerData->globalTable, compilerData->token.stringValue->str);
        }

        getToken(&compilerData->token, &compilerData->IndentationStack);
            //PRIRAZENI
            if(compilerData->token.tokenType == TOKEN_EQUALS){
                getToken(&compilerData->token, &compilerData->IndentationStack);


                //TODO prirazeni

                //VOLANI COMMVALUE PRO VYHODNOCENI PRIRAZENE HODNOTY
                commandValue(compilerData);

                if(compilerData->token.tokenType == TOKEN_EOL){
                    anotherCommand(compilerData);
                }
            }

            //POKUD IHNED ZA ID NASLEDUJE ZAVORKA, PATRNE JDE O VOLANI FCE BEZ PRIRAZENI
            else if(compilerData->token.tokenType == TOKEN_LEFT_BRACKET){

                //VYHODNOCENI HODNOT FCE
                finalState = Values(&compilerData);
                //TODO generate volani funkce

            }

            else{
                return 2;
            }
     }

    else if(compilerData->token.keyword == IF && compilerData->token.tokenType == TOKEN_KEYWORD){
           getToken(&compilerData->token, &compilerData->IndentationStack);
    }


}
static int Values(CompilerData *compilerData){

    getToken(&compilerData->token, &compilerData->IndentationStack);

    int loadedParametrs = 0;

        while(compilerData->token.tokenType != TOKEN_RIGHT_BRACKET){

            loadedParametrs++;

            getToken(&compilerData->token, &compilerData->IndentationStack);

            //VALUES ->  VAL VALUES
            switch(compilerData->token.tokenType){

                //VAL ->  id
                case TOKEN_IDENTIFIER:

                        //pristup na polozku v symTablu
                        //pristup na typ polozky a opet stejny case jako tento, akorat bez case ID

                        switch (STSearch(&compilerData->localTable, &compilerData->token.stringValue)->type){
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
    if ( loadedParametrs != STSearch(&compilerData->globalTable, &compilerData->current_id)->numberOfParams){
        return 2;
    }



}

static int anotherCommand (CompilerData *compilerData){
    getToken(&compilerData->token, &compilerData->IndentationStack);

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
    }

    if(compilerData->token.tokenType == TOKEN_IDENTIFIER ){

            getToken(&compilerData->token, &compilerData->IndentationStack);




    }
    return 0;

}



static int Params(CompilerData *compilerData){
    if (compilerData->token.tokenType == TOKEN_IDENTIFIER){

        STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
        getToken(&compilerData->token, &compilerData->IndentationStack);

        anotherParam(compilerData);

    }
    else{
        return 2;
    }

}



static int anotherParam (CompilerData *compilerData){

    if(compilerData->token.tokenType == TOKEN_COLON){
        getToken(&compilerData->token, &compilerData->IndentationStack);

        if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

            STInsert(&compilerData->localTable, compilerData->token.stringValue->str);
            getToken(&compilerData->token, &compilerData->IndentationStack);

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



