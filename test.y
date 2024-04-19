%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "intermediare.h"

func_tab *liste;
func_tab *current_fct, *current_call;
intermediare *inter;
int param_number,local_number;
int if_label,while_label,do_label;
cond_tab *if_table, *while_table, *do_table;


char tmp[256], tmp2[256];

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

%token BEG END SET INCR DECR CALL RET IF FI ELSE DOWHILE OD DOFORI;
%token OPEN_ACCO CLOSE_ACCO VIRGULE OPEN_PARENT CLOSE_PARENT;
%token ADD SUB MULT DIV;
%token DIF AND EGAL OR NOT TRUE FALSE INF INFEQ SUP SUPEQ;

%left EGAL DIF INF INFEQ SUP SUPEQ
%left OR
%left AND
%left NOT
%left ADD SUB
%left MULT DIV

%start S

%%

S : |
   function parameters code END S call_final;

function : BEG OPEN_ACCO IDF CLOSE_ACCO
{
    ajouter_func($3,0,0,&liste);
    current_fct=recherche_func($3,liste);
    add_intermediare(&inter,FUNC_OP,ARG,$3,current_fct);

    param_number=0;
    local_number=0;
};


    
parameters : OPEN_ACCO list_parameters CLOSE_ACCO {
    current_fct->nbr_params=param_number;
    add_intermediare(&inter,NUL_OP,OP,NULL,current_fct);
    };

list_parameters:
    | IDF 
    {
        ajouter(PARAM_VAR,$1,current_fct->nom_func,liste);
        param_number++;
        add_intermediare(&inter,TEST_OP,OP,NULL,current_fct);
        }
    | list_parameters VIRGULE IDF
    {
        ajouter(PARAM_VAR,$3,current_fct->nom_func,liste);
        param_number++;
        add_intermediare(&inter,TEST_OP,OP,NULL,current_fct);
        }
;

code: instr code | ;

instr:
// Affectation d'une variable 
SET OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO{
        if(recherche($3,current_fct->table) ==  NULL){
            ajouter(LOCAL_VAR,$3,current_fct->nom_func
            ,current_fct);
            local_number++;
            current_fct->nbr_locals++;
        }
        add_intermediare(&inter,SET_OP,ARG,$3,current_fct);
    };
// Incrémnetation d'une variable
    |INCR OPEN_ACCO IDF CLOSE_ACCO{
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }
        add_intermediare(&inter,INCR_OP,ARG,$3,current_fct);
    };
// Décrémentation d'une variable
    |DECR OPEN_ACCO IDF CLOSE_ACCO{
        if(recherche($3,current_fct->table) ==  NULL){
            fprintf(stderr,"La variable \"%s\" n'existe pas\n",$3);
            exit(EXIT_FAILURE);
        }
        add_intermediare(&inter,DECR_OP,ARG,$3,current_fct);
    };
// Condition if
    | IF OPEN_ACCO EXPR{
        snprintf(tmp,256,"%d",if_label);
        add_cond(&if_table,if_label);
        if_label++;
        add_intermediare(&inter,IF_OP,ARG,tmp,current_fct);
    }
    CLOSE_ACCO code ELSE_F;
// Boucle While
    | DOWHILE {
        snprintf(tmp,256,"%d",while_label);
        add_cond(&while_table,while_label);
        while_label++;
        add_intermediare(&inter,DOWHILE_1_OP,ARG,tmp,current_fct);
    }OPEN_ACCO EXPR CLOSE_ACCO {
        snprintf(tmp,256,"%d",while_label);
        add_cond(&while_table,while_label);
        while_label++;
        add_intermediare(&inter,DOWHILE_2_OP,ARG,tmp,current_fct);
    } code OD {
        //récupérer le label pour la fin du while
        snprintf(tmp2,256,"%d",pop_cond(&while_table));
        //récupérer le label pour revenir au while
        snprintf(tmp,256,"%d",pop_cond(&while_table));
        add_intermediare(&inter,OD_1_OP,ARG,tmp,current_fct);
        //pour ajouter le label de la fin
        add_intermediare(&inter,OD_2_OP,ARG,tmp2,current_fct);
    }
;
// Retour de fonction
    | RET OPEN_ACCO EXPR CLOSE_ACCO {
        add_intermediare(&inter,RET_OP,OP,NULL,current_fct);
    };
// Boucle for
    | DOFORI OPEN_ACCO IDF CLOSE_ACCO OPEN_ACCO EXPR CLOSE_ACCO {
        if(recherche($3,current_fct->table) ==  NULL){
            ajouter(LOCAL_VAR,$3,current_fct->nom_func
            ,current_fct);
            local_number++;
            current_fct->nbr_locals++;
        }
        // Pour faire le set de la variable de début
        add_intermediare(&inter,DOFORI_1_OP,ARG,$3,current_fct);

        // Création du label pour revenir au for
        snprintf(tmp,256,"%d",do_label);
        add_cond(&do_table,do_label);
        do_label++;
        add_intermediare(&inter,DOFORI_2_OP,ARG,tmp,current_fct);

    } 
    OPEN_ACCO EXPR CLOSE_ACCO {
        // Création du jump pour sortir du for
        snprintf(tmp,256,"%d",do_label);
        add_cond(&do_table,do_label);
        do_label++;
        add_intermediare(&inter,DOFORI_3_OP,ARG,tmp,current_fct);
    } code OD {
        // récupérer le label de sortie du for
        snprintf(tmp2,256,"%d",pop_cond(&do_table));
        // récupérer le label pour revenir au for
        snprintf(tmp,256,"%d",pop_cond(&do_table));
        
        add_intermediare(&inter,OD_FORI_1_OP,ARG,tmp,current_fct);
        add_intermediare(&inter,OD_FORI_2_OP,ARG,tmp2,current_fct);
    }
;
// Condition else 
ELSE_F : ELSE {
    // récupérer le label pour le if qui saute juste avant le else
    snprintf(tmp2,256,"%d",pop_cond(&if_table));
    // ajouter le label de la fin du code du if
    snprintf(tmp,256,"%d",if_label);
    add_cond(&if_table,if_label);
    if_label++;
    add_intermediare(&inter,ELSE_OP,ARG,tmp,current_fct);

    add_intermediare(&inter,ELSE_2_OP,ARG,tmp2,current_fct);
    }   code FI {
        snprintf(tmp,256,"%d",pop_cond(&if_table));
        add_intermediare(&inter,FI_OP,ARG,tmp,current_fct);
    }
    |FI {
        snprintf(tmp,256,"%d",pop_cond(&if_table));
        add_intermediare(&inter,FI_OP,ARG,tmp,current_fct);
    
    }
;
// Les expressions
EXPR:
    EXPR ADD EXPR {
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
    
    | EXPR OR EXPR {
        if(test_expr_bool($1,$3) == ERR_T){
            $$=ERR_T;
        } else {
            $$=BOOL_T;
            add_intermediare(&inter,OR_OP,OP,NULL,current_fct);
        }
    }

    | EXPR EGAL EXPR {
    if($1 != $3){
        fprintf(stderr,"Type non compatible\n");
        exit(EXIT_FAILURE);
    }
    $$=BOOL_T;
    add_intermediare(&inter,EGAL_OP,OP,NULL,current_fct);
    }

    | EXPR DIF EXPR {
        if($1 != $3){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        add_intermediare(&inter,DIF_OP,OP,NULL,current_fct);
    }

    | EXPR INF EXPR {
        if($1 != NUM_T || $3 != NUM_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        add_intermediare(&inter,INF_OP,OP,NULL,current_fct);
    }

    | EXPR INFEQ EXPR {
        if($1 != NUM_T || $3 != NUM_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        add_intermediare(&inter,INFEQ_OP,OP,NULL,current_fct);
    }

    | EXPR SUP EXPR {
        if($1 != NUM_T || $3 != NUM_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        add_intermediare(&inter,SUP_OP,OP,NULL,current_fct);
    }

    | EXPR SUPEQ EXPR {
        if($1 != NUM_T || $3 != NUM_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        add_intermediare(&inter,SUPEQ_OP,OP,NULL,current_fct);
    }

    | NOT OPEN_PARENT EXPR CLOSE_PARENT {
        if($3 != BOOL_T){
            fprintf(stderr,"Type non compatible\n");
            exit(EXIT_FAILURE);
        }
        $$=BOOL_T;
        add_intermediare(&inter,NOT_OP,OP,NULL,current_fct);
    }

    | OPEN_PARENT EXPR CLOSE_PARENT{
        $$=$2;
    }

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

    | call_expr{
        $$=NUM_T;
    }
;
// Appel de fonction dans le code
call_expr : call_e call_expr_params;   

call_e : CALL OPEN_ACCO IDF CLOSE_ACCO{
    param_number=0;
    if(recherche_func($3,liste) == NULL){
        fprintf(stderr,"La fonction \"%s\" n'existe pas\n",$3);
        exit(EXIT_FAILURE);
    }
    current_call=recherche_func($3,liste);

    add_intermediare(&inter,CALL_EXP_OP,OP,NULL,current_call);
};
    
call_expr_params : OPEN_ACCO call_expr_param CLOSE_ACCO{
    // if(param_number != current_call->nbr_params){
    //     fprintf(stderr,"Nombre de parametres incorrect pour %s:%d\n",current_call->nom_func,param_number);
    //     exit(EXIT_FAILURE);}
    add_intermediare(&inter,CALL_EXP_PARAM_END_OP,OP,NULL,current_call);
};

call_expr_param: EXPR {
        param_number++;
    }

    | call_expr_param VIRGULE EXPR{
        param_number++;
    }
;


call_final : call_f call_params;

call_f : CALL OPEN_ACCO IDF CLOSE_ACCO{
    if(recherche_func($3,liste) == NULL){

        fprintf(stderr,"La fonction \"%s\" n'existe pas\n",$3);
        exit(EXIT_FAILURE);
    }
    param_number=0;
    current_call=recherche_func($3,liste);

    add_intermediare(&inter,CALL_OP,OP,NULL,current_call);
};
call_params : OPEN_ACCO call_param CLOSE_ACCO{

    if(param_number != current_call->nbr_params){
        fprintf(stderr,"Nombre de parametres incorrect\n");
        exit(EXIT_FAILURE);}
    add_intermediare(&inter,CALL_PARAM_END_OP,OP,NULL,current_call);
    // Dépiler le nombre de variables locales + paramètres
};

call_param: EXPR {
        param_number++;
    }
    | call_param VIRGULE EXPR{
        param_number++;
    }
;
%%

int main(void){
    liste=NULL;
    inter=NULL;
    yyparse();
    traverse_func_tab(liste);
    generate_intermediare(inter);
    free_intermediare(inter);
    return 0;
}