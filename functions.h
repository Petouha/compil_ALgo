#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct sym
{
    char nom_idf[64];
    int valeur;
    struct sym *ptr;
}sym_tab;

sym_tab *nouvelle_cellule();
void ajouter(int val, char* nom, sym_tab** head);
int recherche(char* nom, sym_tab* head);
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
void affectation(int i, int j);