#include <stdio.h>
#include "stack.h"
#include "generator.h"
#include "compiler.h"
#include "symtable.c"
#include "dynamicString.h"
#include "fileScanner.h"

DS dynamicString;



/*char* doubleToString(double convertedNumber){

    char doubleInString[50];
    snprintf( doubleInString, 50, "%f", convertedNumber);

    return doubleInString;
}*/


int compilerDataInit(CompilerData* compilerData){

    STInit(&compilerData->localTable);
    STInit(&compilerData->globalTable);

    initStack(&compilerData->indentationStack);
    stackPush(&compilerData->indentationStack, 0);

    compilerData->token.stringValue = malloc(sizeof(DS));
    DSInit(compilerData->token.stringValue);

    compilerData->current_id = NULL;
    compilerData -> inFunction = NULL;
	compilerData -> inWhileOrIf = NULL;
	
}


    static int Prog (CompilerData *compilerData){

        getToken(&compilerData->token, &compilerData->indentationStack);

       // PROG → def id PARAMS eol indent LIST_COMMAND_FUNC eol dedent PROG
        if ((compilerData->token.tokenType == TOKEN_KEYWORD) && (compilerData->token.keyword == DEF)){

            getToken(&compilerData->token, &compilerData->indentationStack);

            if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

                compilerData->current_id = STInsert(&compilerData->localTable, compilerData->token.stringValue);
                generateFunctionStart(compilerData->current_id);

                getToken(&compilerData->token, &compilerData->indentationStack);

                if (compilerData->token.tokenType == TOKEN_LEFT_BRACKET){

                    getToken(&compilerData->token, &compilerData->indentationStack);
                        Params(&compilerData);

                    if(compilerData->token.tokenType == TOKEN_RIGHT_BRACKET){
                        getToken(&compilerData->token, &compilerData->indentationStack);

                        if(compilerData->token.tokenType == TOKEN_DOUBLE_DOT){
                            getToken(&compilerData->token, &compilerData->indentationStack);

                            if((compilerData->token.tokenType == TOKEN_EOL)){
                       
                                    getToken(&compilerData->token, &compilerData->indentationStack);

                                    if(compilerData->token.tokenType == TOKEN_INDENT){
                                        
                                        getToken(&compilerData->token, &compilerData->indentationStack);

                                        Commands(&compilerData);

                                        if (compilerData->token.tokenType == TOKEN_DEDENT){
                                            return Prog(&compilerData);
                                        }
                                    }
                                    else{
                                        return 2;
                                    }
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
                else{
                    return 2;
                }
            }
            else{
                return 2;
            }
        }

       //PROG →  eol PROG
        else if (compilerData->token.tokenType == TOKEN_EOL){
            getToken(&compilerData->token, &compilerData->indentationStack);
            return Prog(&compilerData);
        }

        //PROG →  EOF
        else if (compilerData->token.tokenType == TOKEN_EOF){
            return 0;
        }

        //PROG →  COMMANDS eol PROG
        else{
           Commands(&compilerData);
           return Prog(&compilerData);
        }
    }

    static int anotherCommand (CompilerData *compilerData){
        getToken(&compilerData->token, &compilerData->indentationStack);

    }

    static int Commands (CompilerData *compilerData){
        if(compilerData->token.tokenType == TOKEN_KEYWORD)
            if (compilerData->token.keyword == INPUTS){
                generateRead();
            }
            else if (compilerData->token.keyword == INPUTF){
                generateRead();
            }
            else if(compilerData->token.keyword == INPUTI){
                generateRead();
            }


            else if(compilerData->token.keyword == PRINT){
                generateWrite();
            }


            else if(compilerData->token.tokenType == TOKEN_IDENTIFIER){
                getToken(&compilerData->token, &compilerData->indentationStack);

                if(compilerData->token.tokenType == TOKEN_EQUALS){
                    getToken(&compilerData->token, &compilerData->indentationStack);
                    commandValue(&compilerData);

                    if(compilerData->token.tokenType == TOKEN_EOL){
                        anotherCommand(&compilerData);
                    }

                }
                else{
                    anotherCommand(&compilerData);
                }
            }
            else if(compilerData->token.keyword == IF && compilerData->token.tokenType == TOKEN_KEYWORD){
               getToken(&compilerData->token, &compilerData->indentationStack);

                

            }
            else if(compilerData->token.keyword == INPUTI){
                generateRead();
            }
            else if(compilerData->token.keyword == INPUTI){
                generateRead();
            }
            else if(compilerData->token.keyword == INPUTI){
                generateRead();
            }

    }

    static int commandValue (CompilerData *compilerData){

    }

    static int Values(CompilerData *compilerData){

    }

    static int Value(CompilerData *compilerData){

    }

    static int Params(CompilerData *compilerData){
        if (compilerData->token.tokenType == TOKEN_IDENTIFIER){

            STInsert(&compilerData->localTable, &compilerData->token.stringValue);
            getToken(&compilerData->token, &compilerData->indentationStack);

            anotherParam(&compilerData);

        }
        else{
            return 2;
        }

    }



    static int anotherParam (CompilerData *compilerData){

        if(compilerData->token.tokenType == TOKEN_COLON){
            getToken(&compilerData->token, &compilerData->indentationStack);

            if(compilerData->token.tokenType == TOKEN_IDENTIFIER){

                STInsert(&compilerData->localTable, &compilerData->token.stringValue);
                getToken(&compilerData->token, &compilerData->indentationStack);

                return anotherParam(&compilerData);
            }
            else{
                return 2;
            }
        }
        else{
           return 0;
        }
    }

    static int commandValue (){

    }

    static int listCommandFunction(){

    }



int main(int argc, char *argv[])
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

    

    Prog(&compilerData);

    free(&compilerData->token);
    fclose(sourceCode);

    generateFunctionEnd("test");

    printf("%s", dynamicString.str);

}



    /*while(token->tokenType != TOKEN_EOF)
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





