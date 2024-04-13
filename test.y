%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "intermediare.h"

cond_tab *if_table;
func_tab *liste;
func_tab *current_fct, *current_call;
intermediare *inter;
int param_number,local_number,if_label;

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

%token BEG END SET INCR DECR CALL RET IF FI ELSE;
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
   function parameters code end_function S call_final;

function : BEG OPEN_ACCO IDF CLOSE_ACCO
{
    ajouter_func($3,0,0,&liste);

    current_fct=recherche_func($3,liste);
    //fprintf(stderr,"Debyt de la fonction %s\n",current_fct->nom_func);

    add_intermediare(&inter,FUNC_OP,ARG,$3,current_fct);

    param_number=0;
    local_number=0;
    };

end_function : RET OPEN_ACCO EXPR CLOSE_ACCO END
{
    add_intermediare(&inter,RET_OP,OP,NULL,current_fct);
    //fprintf(stderr,"Fin de la fonction %s\n",current_fct->nom_func);
    };
    
parameters : OPEN_ACCO list_parameters CLOSE_ACCO 
{
    current_fct->nbr_params=param_number;add_intermediare(&inter,NUL_OP,OP,NULL,current_fct);};

list_parameters:
    | IDF 
    {
        //fprintf(stderr,"%s\n",$1);
        ajouter(PARAM_VAR,$1,current_fct->nom_func,liste);
        param_number++;
        add_intermediare(&inter,TEST_OP,OP,NULL,current_fct);
        }
    | list_parameters VIRGULE IDF
    {
        //fprintf(stderr,"%s\n",$3);
        ajouter(PARAM_VAR,$3,current_fct->nom_func,liste);
        param_number++;
        add_intermediare(&inter,TEST_OP,OP,NULL,current_fct);
        };
code: instr code | ;

instr: SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO
    {
        if(recherche($3,current_fct->table) ==  NULL){
            ajouter(LOCAL_VAR,$3,current_fct->nom_func
            ,current_fct);
            local_number++;
            current_fct->nbr_locals++;
        }
        //print_param($3,current_fct->table);

        //affect_from_top_stack($3,liste->table);
        //print_param($3,current_fct->table);

        //fprintf(stderr,"affect %s = \n",$3);

        add_intermediare(&inter,SET_OP,ARG,$3,current_fct);
        }
    |INCR OPEN_ACCO IDF CLOSE_ACCO
    {
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }

        //increment($3,current_fct->table);
        //print_param($3,current_fct->table);

        add_intermediare(&inter,INCR_OP,ARG,$3,current_fct);
    };
    |DECR OPEN_ACCO IDF CLOSE_ACCO
    {
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }
        //decrement($3,current_fct->table);
        add_intermediare(&inter,DECR_OP,ARG,$3,current_fct);
    };
    | IF OPEN_ACCO COND {
        snprintf(tmp,256,"%d",if_label);
        add_cond(&if_table,if_label);
        if_label++;
        add_intermediare(&inter,IF_OP,ARG,tmp,current_fct);
    } CLOSE_ACCO code FI {
        snprintf(tmp,256,"%d",pop_cond(&if_table));
        add_intermediare(&inter,FI_OP,ARG,tmp,current_fct);
    }
    




EXPR:
 EXPR ADD EXPR{
    if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;

            add_intermediare(&inter,ADD_OP,OP,NULL,current_fct);
            //addition();
        }
    }
    | EXPR SUB EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            //soustraction();
            add_intermediare(&inter,SUB_OP,OP,NULL,current_fct);
        }
    }
    | EXPR MULT EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            //multiplication();

            add_intermediare(&inter,MUL_OP,OP,NULL,current_fct);
        }
    }
    | EXPR DIV EXPR {
        if(test_expr_int($1,$3) == ERR_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        } else {
            $$=NUM_T;
            add_intermediare(&inter,DIV_OP,OP,NULL,current_fct);
        }
    }
    | EXPR AND EXPR {
        if(test_expr_bool($1,$3) == ERR_T){
            $$=ERR_T;
        } else {
            $$=BOOL_T;
            add_intermediare(&inter,AND_OP,OP,NULL,current_fct);
        }
    }
    | EXPR OR EXPR{
        if(test_expr_bool($1,$3) == ERR_T){
            $$=ERR_T;
        } else {
            $$=BOOL_T;
            add_intermediare(&inter,OR_OP,OP,NULL,current_fct);
        }
    }
    | OPEN_PARENT EXPR CLOSE_PARENT{$$=$2;}
    | NUM {
        $$=NUM_T;
        //num($1);
        sprintf(tmp,"%d",$1);
        add_intermediare(&inter,NUM_OP,ARG,tmp,current_fct);
        };
    | FALSE {
        $$=BOOL_T;
        add_intermediare(&inter,NUM_OP,ARG,"0",current_fct);
        }
    | TRUE {
        $$=BOOL_T;
        add_intermediare(&inter,NUM_OP,ARG,"1",current_fct);
        }
    | IDF {
        $$=NUM_T;
        if(recherche($1,current_fct->table) == NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$1);
            exit(EXIT_FAILURE);
        }
        
        add_intermediare(&inter,IDF_OP,ARG,$1,current_fct);
        }
    | call_expr
     {
        $$=NUM_T;
        };
COND : EXPR EGAL EXPR {
    if($1 != $3){
        fprintf(stderr,"Type non compatible\n");
        exit(EXIT_FAILURE);
    }
    add_intermediare(&inter,EGAL_OP,OP,NULL,current_fct);
}
call_expr : call_e call_expr_params {};    

call_e : CALL OPEN_ACCO IDF CLOSE_ACCO
{
    if(recherche_func($3,liste) == NULL){
        fprintf(stderr,"La fonction \"%s\" n'existe pas\n",$3);
        exit(EXIT_FAILURE);
    }
    param_number=0;
    current_call=recherche_func($3,liste);

    add_intermediare(&inter,CALL_EXP_OP,OP,NULL,current_call);
    }
    ;
    
call_expr_params : OPEN_ACCO call_expr_param CLOSE_ACCO
{
    if(param_number != current_call->nbr_params){
        fprintf(stderr,"Nombre de parametres incorrect\n");
        exit(EXIT_FAILURE);}
    add_intermediare(&inter,CALL_EXP_PARAM_END_OP,OP,NULL,current_call);
    // Dépiler le nombre de variables locales + paramètres
    
};

call_expr_param: 
    EXPR 
    {
        param_number++;
        }
    | call_expr_param VIRGULE EXPR
    {
        param_number++;
        };


call_final : call_f call_params;

call_f : CALL OPEN_ACCO IDF CLOSE_ACCO
{
    if(recherche_func($3,liste) == NULL){
        fprintf(stderr,"La fonction \"%s\" n'existe pas\n",$3);
        exit(EXIT_FAILURE);
    }
    param_number=0;
    current_call=recherche_func($3,liste);

    add_intermediare(&inter,CALL_OP,OP,NULL,current_call);
    }
    ;
call_params : OPEN_ACCO call_param CLOSE_ACCO
{
    if(param_number != current_call->nbr_params){
        fprintf(stderr,"Nombre de parametres incorrect\n");
        exit(EXIT_FAILURE);}
    add_intermediare(&inter,CALL_PARAM_END_OP,OP,NULL,current_call);
    // Dépiler le nombre de variables locales + paramètres
    }
    ;
call_param: 
    EXPR 
    {
        param_number++;
        }
    | call_param VIRGULE EXPR
    {
        param_number++;
        };
%%

int main(void){
    liste=NULL;
    inter=NULL;
    yyparse();
    print_intermediare(inter);
    generate_intermediare(inter);
    print_sym_tab(liste->table);
    free_intermediare(inter);
    return 0;
}