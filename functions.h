#ifndef FUNCTIONS_H 
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef enum
{
    LOCAL_VAR,
    PARAM_VAR
} TYPE_VAR;

typedef enum
{
    NUM_T,
    BOOL_T,
    ERR_T
} TYPE_SYNTH;

extern int param_number, local_number,label;

typedef struct func_sym
{
    char* nom_func;
    int nbr_params;
    int nbr_locals;
    struct sym *table;
    struct func_sym *ptr;
} func_tab;

typedef struct sym
{
    char *nom_idf;
    TYPE_VAR type;
    int num_var;
    struct sym *ptr;
} sym_tab;



/*
Fonction en assembleur qui gère l'erreur de division par 0.
*/
void err_div();

/*
Fonction en assembleur qui gère l'erreur de dépassement
de capacité.
*/
void err_overflow();

/*
Ecrit en assembleur tout ce dont le programme a besoin
en entête.
*/
void start_asm();

/*
Permet de lancer le programme en exécutant la fonction donné.
*/
void main_asm();

/*
Affiche le haut de la pile et termine le programme.
*/
void end_asm();

/*
Teste si les deux paramètres sont de types NUM_T.
RETOUR: NUM_T si oui, ERR_T sinon.
*/
int test_expr_int(int first, int second);

/*
Teste si les deux paramètres sont de types BOOL_T.
RETOUR: BOOL_T si oui, ERR_T sinon.
*/
int test_expr_bool(int first, int second);

/*
Fait l'addition des 2 premières valeurs sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
*/
void addition();

/*
Fait la soustraction des 2 premières valeurs sur la pile.
UTILISE : ax,bx
RESULTAT: en haut de la pile
*/
void soustraction();

/*
Fait la multiplication des 2 premières valeurs sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
*/
void multiplication();

/*
Fait la division des 2 premières valeurs sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
*/
void division();

/*
Fait le or logique des 2 premières valeurs sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void or();


/*
Fait le not logique de la 1ère valeur sur la pile.
UTILISE : ax,bx
RESULTAT: en haut de la pile
*/
void not();

/*
Fait l'égalité entre les deux premières valeurs sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void egal();

/*
Fait la différence entre les deux premières valeurs sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void diff();

/*
Fait la comparaison si la première valeur est supérieure à la deuxième valeur sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void sup();

/*
Fait la comparaison si la première valeur est inférieure à la deuxième valeur sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void inf();

/*
Fait la comparaison si la première valeur est supérieure ou égale à la deuxième valeur sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void supeq();

/*
Fait la comparaison si la première valeur est inférieure ou égale à la deuxième valeur sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
Incrémente label.
*/
void infeq();

/*
Met sur la pile la constante à utiliser
UTILISE: ax
*/
void num(int number);

/*
Affecte la valeur qui se trouve sur le haut de la pile dans
la variable nom.
UTILISE: ax,bx,dx
*/
void affect_from_top_stack(char *nom,func_tab* head);

/*
Incremente la valeur de la variable nom.
UTILISE: ax,bx,dx
*/
void increment(char *nom, func_tab *head);

/*
Décremente la valeur de la variable nom.
UTILISE: ax,bx,dx
*/
void decrement(char *nom, func_tab *head);




/*
RETOUR : la nouvelle cellule de sym_tab crée par malloc (la gestion d'erreur se fait dans ajouter)
*/
sym_tab *nouvelle_cellule();

/*
RETOUR : la nouvelle cellule de func_tab crée par malloc (la gestion d'erreur se fait dans ajouter)
*/
func_tab *nouvelle_cellule_func();

/*
Ajoute en tête de liste la nouvelle cellule qui contient l'identifiant
*/
void ajouter(int val, char *nom, char* nom_func,func_tab* head);

/*
Ajoute en tête de liste la nouvelle cellule qui contient la fonction
*/
void ajouter_func(char *nom, int nbr_p, int nbr_v,func_tab **head);

/*
Recherche l'identifiant dans la table des symboles
RETOUR : la cellule si existe, NULL sinon 
*/
sym_tab *recherche(char *nom, sym_tab *head);

/*
Recherche la fonction dans la table des symboles
RETOUR : la cellule si existe, NULL sinon 
*/
func_tab *recherche_func(char *nom, func_tab *head);


/*
Affiche la table des symboles
*/
void print_sym_tab(sym_tab *head);



/*
    récupère depuis la pile un des paramètres (si il y en a) de l'algo.
    à partir de la base de la pile (qui contient le dernier paramètre), on ajoute
    le nombre de mot machines nécessaire pour atteindre l'adresse mémoire du paramètre.
    RESULTAT : DANS dx
    ADRESSE DE LA VAR : DANS bx
    UTILISE : ax,bx,dx
*/
void get_param_from_stack(char *nom, func_tab *head);


/*
Recupère la position du paramètre sur la pile en utilisant la table des symboles
*/
int get_param_location(char *nom, func_tab *head);

/*
Affiche un paramètre depuis la pile
UTILISE: bx, cx
*/
void print_param(char *nom, func_tab *head);

/*
affiche le registre dont l'adresse est contenue dans le paramètre nom.
UTILISE: cx et le registre contenue dans nom
*/
void print_reg(char *nom);

/*
Prépare la 1ère partie de la pile d'execution avant un appel de fonction:
- Empile 0 pour la valeur de retour
- Empile les variables locales de la fonction
*/
void prepare_stack_locals(func_tab *func);

/*
Prépare la 3ème partie de la pile d'execution après un appel de fonction:
- Empile le retour de la fonction (fait automatiquement par asipro)
- Empile l'ancienne base de la pile
- Monter la base de la pile
*/
void prepare_stack_func(func_tab *func);

/*
Execute la fin de l'appel de fonction:
- Mettre la valeur de retour dans son emplacement
- Mettre sp au même niveau que bp
- Restaurer la base de la pile
- Retourner à l'appelant
*/
void return_from_func(func_tab *func);
// Fonction pour libérer la structure sym_tab
void free_sym(sym_tab *head);
// Fonction pour libérer la structure func_tab
void free_func(func_tab *head);
// création du label des conditions
char* create_label(char* label_name, int number);
#endif