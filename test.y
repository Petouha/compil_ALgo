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

parameters : OPEN_ACCO list_parameters CLOSE_ACCO {printf("\tpush bp\n");};

list_parameters:
    | IDF list_parameters {ajouter(PARAM_VAR,$1,&liste);num(param_number); param_number++;printf(";%s\n",$1);};
    | VIRGULE list_parameters 

/* list_parameters: 
    | VIRGULE IDF list_parameters{ajouter(PARAM_VAR,$2,&liste);num(param_number);param_number++;printf(";%s\n",$2);}; */

code: //rien 
    {
    }
    | SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO IDF CLOSE_ACCO {
        affectation($3,$6,liste);
        printf("\tpop cx\n");
        printf("\tpush dx\n");
    };

%%
/*
    callprintfd dx
	const ax,nl
	callprintfs ax
*/

int main(void){
    liste=NULL;

    start_asm();
    yyparse();
    end_asm();
    print_sym_tab(liste);
    return 0;
}