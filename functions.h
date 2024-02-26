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

extern int param_number, local_number;
typedef struct sym
{
    char nom_idf[64];
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
Fait l'addition des 2 premières valeures sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
*/
void addition();

/*
Fait la soustraction des 2 premières valeures sur la pile.
UTILISE : ax,bx
RESULTAT: en haut de la pile
*/
void soustraction();

/*
Fait la multiplication des 2 premières valeures sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
*/
void multiplication();

/*
Fait la division des 2 premières valeures sur la pile.
UTILISE : ax,bx,cx
RESULTAT: en haut de la pile
*/
void division();

/*
Met sur la pile la constante à utiliser
UTILISE: ax
*/
void num(int number);

/*
Affecte la valeur de var2 à var1.
Uniquement pour l'affectation IDF = IDF.
UTILISE: ax,bx,dx
*/
void affectation(char *var1, char *var2, sym_tab *head);

/*
Affecte la valeur qui se trouve sur le haut de la pile dans
la variable nom.
UTILISE: ax,bx,dx
*/
void affect_from_top_stack(char *nom,sym_tab* head);

/*
Incremente la valeur de la variable nom.
UTILISE: ax,bx,dx
*/
void increment(char *nom, sym_tab *head);

/*
Décremente la valeur de la variable nom.
UTILISE: ax,bx,dx
*/
void decrement(char *nom, sym_tab *head);




/*
RETOUR : la nouvelle cellule crée par malloc (la gestion d'erreur se fait dans ajouter)
*/
sym_tab *nouvelle_cellule();

/*
Ajoute en tête de liste la nouvelle cellule qui contient l'identifiant
*/
void ajouter(int val, char *nom, sym_tab **head);


/*
Recherche l'identifiant dans la table des symboles
RETOUR : la cellule si existe, NULL sinon 
*/
sym_tab *recherche(char *nom, sym_tab *head);

void print_sym_tab(sym_tab *head);



/*
    récupère depuis la pile un des paramètres (si il y en a) de l'algo.
    à partir de la base de la pile (qui contient le dernier paramètre), on ajoute
    le nombre de mot machines nécessaire pour atteindre l'adresse mémoire du paramètre.
    RESULTAT : DANS dx
    ADRESSE DE LA VAR : DANS bx
    UTILISE : ax,bx,dx
*/
void get_param_from_stack(char *nom, sym_tab *head);


void set_param_from_stack(char *nom, sym_tab *head);

/*
Recupère la position du paramètre sur la pile en utilisant la table des symboles
*/
int get_param_location(char *nom, sym_tab *head);

/*
Affiche un paramètre depuis la pile
UTILISE: bx, cx
*/
void print_param(char *nom, sym_tab *head);

/*
affiche le registre dont l'adresse est contenue dans le paramètre nom.
UTILISE: cx et le registre contenue dans nom
*/
void print_reg(char *nom);