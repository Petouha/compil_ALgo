#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef enum {LOCAL_VAR,PARAM_VAR} TYPE_VAR;

extern int param_number,local_number;
typedef struct sym
{
    char nom_idf[64];
    TYPE_VAR type;
    int num_var;
    struct sym *ptr;
}sym_tab;

sym_tab *nouvelle_cellule();
void ajouter(int val, char* nom, sym_tab** head);
sym_tab *recherche(char* nom, sym_tab* head);
void print_sym_tab(sym_tab *head);

void addition();
void soustraction();
void multiplication();
void division();
void num(int number);
void start_asm();
void end_asm();
void err_div();
void err_overflow();
void affectation(char* var1, char* var2,sym_tab* head);

void set_param_from_stack(char *nom, sym_tab* head);
void get_param_from_stack(char *nom,sym_tab* head);
int get_param_location(char *nom,sym_tab* head);