#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    char nom_idf[64];
    int valeur;
    sym_tab* ptr;
}sym_tab;

sym_tab *nouvelle_cellule();
void ajouter(int val, char* nom, sym_tab** head);
int recherche(char* nom, sym_tab** head);

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