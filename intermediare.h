// un fichier .h qui contient la structure de la liste chainée et les prototypes 
// des fonctions qui permettent de manipuler cette liste du code intermédiaire
// l'insertion se fait en fin de liste (FIFO)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// enumération des types des opérations

typedef enum operation{
    ADD_OP,
    SUB_OP,
    MUL_OP,
    DIV_OP,
    AND_OP,
    OR_OP,
    NOT_OP,
    SET_OP,
    INCR_OP,
    DECR_OP,
    IDF_OP,
    NUM_OP,
    NUL_OP,
    FUNC_OP,
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
void add_intermediare(intermediare **head, operation op,arg_type type, char *arg);
void print_intermediare(intermediare *head);





