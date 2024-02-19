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
    TYPE_SYNTH synth;
}

%type<synth> EXPR

%token<entier> NUM
%token<idf> IDF

%token BEG END SET INCR DECR;
%token OPEN_ACCO CLOSE_ACCO VIRGULE OPEN_PARENT CLOSE_PARENT;
%token ADD SUB MULT DIV;
%token DIF AND EGAL OR NOT TRUE FALSE;

%left EGAL DIF
%left OR
%left AND
%left NOT
%left ADD SUB
%left MULT DIV

%start S

%%

S : parameters code;

// starting_point : BEG OPEN_ACCO IDF CLOSE_ACCO ;

parameters : OPEN_ACCO list_parameters CLOSE_ACCO {};

list_parameters:
    | IDF {
        ajouter(PARAM_VAR,$1,&liste);
        num(param_number);
        param_number++;
        printf(";%s\n",$1);}
    | list_parameters VIRGULE IDF{
        ajouter(PARAM_VAR,$3,&liste);
        num(param_number);
        param_number++;
        printf(";%s\n",$3);};

/* list_parameters: 
    | VIRGULE IDF list_parameters{ajouter(PARAM_VAR,$2,&liste);num(param_number);param_number++;printf(";%s\n",$2);}; */

code: //rien
    | SET_INSTRUCTION code
    |INCR OPEN_ACCO IDF CLOSE_ACCO code
    {
        increment($3,liste);
    };
    |DECR OPEN_ACCO IDF CLOSE_ACCO code
    {
        decrement($3,liste);
    };



SET_INSTRUCTION:
    /* SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO IDF CLOSE_ACCO code {
        print_param($3,liste);
        affectation($3,$6,liste);
        // printf("\tpop cx\n");
        // printf("\tpush dx\n");
        // printf("\tpush cx\n");

    } */
    | SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO code{
        print_param($3,liste);
        affect_from_top_stack($3,liste);
        //print_param($3,liste);

    }
    ;


EXPR: EXPR ADD EXPR{
    if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            addition();
        }
    }
    | EXPR SUB EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            soustraction();
        }
    }
    | EXPR MULT EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            multiplication();
        }
    }
    | EXPR DIV EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            division();
        }
    }
    | OPEN_PARENT EXPR CLOSE_PARENT{$$=$2;}
    | NUM {$$=NUM_T;num($1);};
    | FALSE {$$=BOOL_T;num(0);}
    | TRUE {$$=BOOL_T;num(1);}
    | IDF {$$=NUM_T;get_param_from_stack($1,liste);printf("\tpush dx\n");}
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