%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

sym_tab* liste;

int yylex();  
void yyerror(const char* s){
    fprintf(stderr,"%s\n",s);
}

%}

%define parse.error verbose

%union{
    int entier;
    char *idf;
}

%token<entier> NUM
%token<idf> IDF

%token BEG OPEN_ACCO CLOSE_ACCO VIRGULE SET;

%start S

%%

S : parameters code;

// starting_point : BEG OPEN_ACCO IDF CLOSE_ACCO ;

parameters : OPEN_ACCO list_parameters_start CLOSE_ACCO;

list_parameters_start:
    | IDF list_parameters ;

list_parameters: 
    | VIRGULE IDF list_parameters ;

code: //rien 
    | SET OPEN_ACCO IDF CLOSE_ACCO IDF {printf("")}

%%


int main(void){
    


    yyparse();
    // for (int j = 0; j < 10 && vars[j] != NULL; j++) {
    //     printf("%s\n", vars[j]);
    //     free(vars[j]);  // Free the allocated memory for each variable
    // }
    return 0;
}