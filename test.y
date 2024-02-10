%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

sym_tab *liste;
int param_number,local_number;

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

%token BEG END OPEN_ACCO CLOSE_ACCO VIRGULE SET INCR;

%start S

%%

S : parameters code;

// starting_point : BEG OPEN_ACCO IDF CLOSE_ACCO ;

parameters : OPEN_ACCO list_parameters_start CLOSE_ACCO;

list_parameters_start:
    | IDF list_parameters {ajouter(PARAM_VAR,$1,&liste);};

list_parameters: 
    | VIRGULE IDF list_parameters{ajouter(PARAM_VAR,$2,&liste);};

code: //rien 
    | SET OPEN_ACCO IDF CLOSE_ACCO IDF {};

%%


int main(void){
    
    printf("debut\n");
    liste=NULL;

    yyparse();
    print_sym_tab(liste);
    return 0;
}