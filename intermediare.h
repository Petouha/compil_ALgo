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
    ADD_OP, //0
    SUB_OP, //1
    MUL_OP, //2
    DIV_OP, //3
    AND_OP, //4
    OR_OP, //5
    NOT_OP, //6
    SET_OP, //7
    INCR_OP, //8
    DECR_OP, //9
    IDF_OP, //10
    NUM_OP, //11
    NUL_OP, //12
    FUNC_OP, //13
    CALL_OP, //14
    CALLEND_OP, //15
    RET_OP, //16
    TEST_OP, //17
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

// prototypes des fonctions
/*
ajoute en fin de liste un élément de la liste chainée
argumetns:
    - head: pointeur vers le pointeur de la tête de la liste
    - op: l'opération à ajouter
    - arg1: l'argument de l'opération

*/
void generate_intermediare(intermediare* head);
void add_intermediare(intermediare **head, operation op,arg_type type, char *arg,func_tab* func);
void print_intermediare(intermediare *head);
void choose_op(intermediare *node);
void free_intermediare(intermediare *head);


#endif