%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "intermediare.h"

func_tab *liste;
func_tab *current_fct;
intermediare *inter;
int param_number,local_number;
int label_number = 0;
char tmp[256];

int yylex();  
void yyerror(const char* s){
    fprintf(stderr,"yyerror :%s\n",s);
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

%token BEG END SET INCR DECR CALL;
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

S : |
   function parameters code END S call_func;

function : BEG OPEN_ACCO IDF CLOSE_ACCO{
    ajouter_func($3,0,0,&liste);
    current_fct=recherche_func($3,liste);

    add_intermediare(&inter,FUNC_OP,ARG,$3);
    param_number=0;
    } ;

parameters : OPEN_ACCO list_parameters CLOSE_ACCO {current_fct->nbr_params=param_number;};

list_parameters:
    | IDF {
        ajouter(PARAM_VAR,$1,current_fct->nom_func,liste);
        num(param_number);
        param_number++;
        printf(";%s\n",$1);
        }
    | list_parameters VIRGULE IDF{
        ajouter(PARAM_VAR,$3,current_fct->nom_func,liste);
        num(param_number);
        param_number++;
        printf(";%s\n",$3);
        };
code: instr code | ;

instr: SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO{
        if(recherche($3,current_fct->table) ==  NULL){
            ajouter(LOCAL_VAR,$3,current_fct->nom_func
            ,current_fct);
            printf(";ajouter %s",$3);
        }
        print_param($3,current_fct->table);

        affect_from_top_stack($3,liste->table);
        print_param($3,current_fct->table);

        fprintf(stderr,"affect %s = \n",$3);

        add_intermediare(&inter,SET_OP,ARG,$3);
        }
    |INCR OPEN_ACCO IDF CLOSE_ACCO
    {
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }

        increment($3,current_fct->table);
        print_param($3,current_fct->table);

        add_intermediare(&inter,INCR_OP,ARG,$3);
    };
    |DECR OPEN_ACCO IDF CLOSE_ACCO
    {
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }
        decrement($3,current_fct->table);
        add_intermediare(&inter,DECR_OP,ARG,$3);
    };

EXPR: EXPR ADD EXPR{
    if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;

            add_intermediare(&inter,ADD_OP,OP,NULL);
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
            add_intermediare(&inter,SUB_OP,OP,NULL);
        }
    }
    | EXPR MULT EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            multiplication();

            add_intermediare(&inter,MUL_OP,OP,NULL);
        }
    }
    | EXPR DIV EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            division();

            add_intermediare(&inter,DIV_OP,OP,NULL);
        }
    }
    | EXPR AND EXPR {
        if(test_expr_bool($1,$3) == ERR_T){
            $$=ERR_T;
        } else {
            $$=BOOL_T;
            multiplication();

            add_intermediare(&inter,AND_OP,OP,NULL);
        }
    }
    | OPEN_PARENT EXPR CLOSE_PARENT{$$=$2;}
    | NUM {
        $$=NUM_T;num($1);
        sprintf(tmp,"%d",$1);
        add_intermediare(&inter,NUM_OP,ARG,tmp);
        };
    | FALSE {$$=BOOL_T;num(0);}
    | TRUE {$$=BOOL_T;num(1);}
    | IDF {
        $$=NUM_T;
        if(recherche($1,current_fct->table) == NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$1);
            exit(EXIT_FAILURE);
        }
        
        add_intermediare(&inter,IDF_OP,ARG,$1);

        get_param_from_stack($1,current_fct->table);
        printf("\tpush dx\n");
        }

call_func : call call_params;

call : CALL OPEN_ACCO IDF CLOSE_ACCO{
    if(recherche_func($3,liste) == NULL){
        fprintf(stderr,"La fonction \"%s\" n'existe pas\n",$3);
        exit(EXIT_FAILURE);
    }
    param_number=0;
    current_fct=recherche_func($3,liste);
    }
    ;
call_params : OPEN_ACCO call_param CLOSE_ACCO{
    if(param_number != current_fct->nbr_params){
        fprintf(stderr,"Nombre de parametres incorrect\n");
        exit(EXIT_FAILURE);
    }
    }
    ;
call_param: 
    NUM {param_number++;}
    | call_param VIRGULE NUM{param_number++;};
%%
/*
    callprintfd dx
	const ax,nl
	callprintfs ax
*/

int main(void){
    liste=NULL;
    inter=NULL;
    /* ajouter_func("main",0,0,&liste); */
    

    start_asm();
    main_asm();
    yyparse();
    end_asm();
    printf(";Function : %s + %d params\n",liste->nom_func,liste->nbr_params);
    print_sym_tab(liste->table);
    print_intermediare(inter);
    return 0;
}