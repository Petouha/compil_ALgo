%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

func_tab *liste;
func_tab *current_fct;
int param_number,local_number;
int label_number = 0;

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

S : starting_point parameters code {};

starting_point : BEG OPEN_ACCO IDF CLOSE_ACCO {ajouter_func($3,0,0,&liste);current_fct=recherche_func($3,liste);} ;

parameters : OPEN_ACCO list_parameters CLOSE_ACCO {current_fct->nbr_params=param_number;};

list_parameters:
    | IDF {
        ajouter(PARAM_VAR,$1,current_fct->nom_func,liste);
        num(param_number);
        param_number++;
        printf(";%s\n",$1);}
    | list_parameters VIRGULE IDF{
        ajouter(PARAM_VAR,$3,current_fct->nom_func,liste);
        num(param_number);
        param_number++;
        printf(";%s\n",$3);};

/* list_parameters: 
    | VIRGULE IDF list_parameters{ajouter(PARAM_VAR,$2,liste);num(param_number);param_number++;printf(";%s\n",$2);}; */

code: //rien
    | SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO code{
        // if(recherche($3,liste) ==  NULL){
        //     ajouter(LOCAL_VAR,$3,"main",liste);
        //     printf(";ajouter %s",$3);
        // }
        print_param($3,current_fct->table);

        affect_from_top_stack($3,liste->table);
        print_param($3,current_fct->table);

    }
    |INCR OPEN_ACCO IDF CLOSE_ACCO code
    {
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }

        increment($3,current_fct->table);
        print_param($3,current_fct->table);
    };
    |DECR OPEN_ACCO IDF CLOSE_ACCO code
    {
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }
        decrement($3,current_fct->table);
    };

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
    | EXPR AND EXPR {
        if(test_expr_bool($1,$3) == ERR_T){
            $$=ERR_T;
        } else {
            $$=BOOL_T;
            multiplication();
        }
    }
    | OPEN_PARENT EXPR CLOSE_PARENT{$$=$2;}
    | NUM {$$=NUM_T;num($1);};
    | FALSE {$$=BOOL_T;num(0);}
    | TRUE {$$=BOOL_T;num(1);}
    | IDF {
        get_param_from_stack($1,current_fct->table);
        printf("\tpush dx\n");
        }
%%
/*
    callprintfd dx
	const ax,nl
	callprintfs ax
*/

int main(void){
    liste=NULL;
    /* ajouter_func("main",0,0,&liste); */
    

    start_asm();
    main_asm();
    yyparse();
    end_asm();
    printf(";Function : %s + %d params\n",liste->nom_func,liste->nbr_params);
    print_sym_tab(liste->table);
    return 0;
}