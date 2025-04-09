// un fichier .h qui contient la structure de la liste chainée et les prototypes 
// des fonctions qui permettent de manipuler cette liste du code intermédiaire
// l'insertion se fait en fin de liste (FIFO)
#ifndef INTERMEDIAIRE_H
#define INTERMEDIAIRE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
// enumération des types des opérations

typedef enum operation{
    // opérations arithmétiques
    ADD_OP, //0
    SUB_OP, //1
    MUL_OP, //2
    DIV_OP, //3
    // opérations logiques
    AND_OP, //4
    OR_OP, //5
    NOT_OP, //6
    EGAL_OP, //21
    DIF_OP, //22
    INF_OP, //23
    SUP_OP, //24
    INFEQ_OP, //25
    SUPEQ_OP, //26
    SET_OP, //7
    INCR_OP, //8
    DECR_OP, //9
    IDF_OP, //10
    NUM_OP, //11
    NUL_OP, //12
    FUNC_OP, //13
    CALL_OP, //14
    CALL_PARAM_END_OP, //15
    RET_OP, //16
    TEST_OP, //17
    ARG_OP, //18
    CALL_EXP_OP, //19
    CALL_EXP_PARAM_END_OP, //20
    IF_OP, //22
    FI_OP, //23
    ELSE_OP, //24
    ELSE_2_OP, //25
    DOWHILE_1_OP, //26
    DOWHILE_2_OP, //27
    OD_1_OP, //28
    OD_2_OP, //29
    DOFORI_1_OP, //30
    DOFORI_2_OP, //31
    DOFORI_3_OP, //32
    OD_FORI_1_OP, //33
    OD_FORI_2_OP, //34
} operation;

// enum pour différencier entre les deux types d'arguments
typedef enum arg_type{
    OP,
    ARG,
} arg_type;

// union pour indiquer qu'un argument peut être soit une operation soit une chaine de caractères


// structure de la liste chainée
typedef struct intermediare{
    operation op;
    arg_type type;
    char *arg;
    func_tab *function;
    struct intermediare *next;
} intermediare;


// structure pour gérer les appels de if
typedef struct cond_tab{
    int cond_id;
    struct cond_tab *next;
} cond_tab;

// ------prototypes des fonctions--------
/*
ajoute en fin de liste un élément de la liste chainée
argumetns:
    - head: pointeur vers le pointeur de la tête de la liste
    - op: l'opération à ajouter
    - type: le type de l'argument (OP ou ARG)
    - arg: l'argument de l'opération (vide si type == OP)
    - func: la fonction actuelle

*/
// fonction qui ajoute un élément à la liste des cond en fin de queue
void add_cond(cond_tab **head, int id);
// fonction qui supprime un élément de la liste des cond en fin de queue
int pop_cond(cond_tab **head);
// fonction qui libère la mémoire allouée pour la liste des cond
void free_cond(cond_tab **head);

void add_intermediare(intermediare **head, operation op,arg_type type, char *arg,func_tab* func);

/*
Parcours la liste et génère du code assembleur
*/
void generate_intermediare(intermediare* head);

/*
Affiche le code intermédiaire
*/
void print_intermediare(intermediare *head);

/*
Choisit l'opération à effectuer pendant le parcours
*/
void choose_op(intermediare *node);
/*
Libère la mémoire allouée pour la liste chainée
*/
void free_intermediare(intermediare *head);


#endif
