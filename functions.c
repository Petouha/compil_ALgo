#include"functions.h"

void err_div(){
    printf(":msgErrDiv0\n");
    printf("@string \"Erreur division par 0\\n\"\n");
    printf(":errDiv0\n");
    printf("\tconst ax,msgErrDiv0\n");
    printf("\tcallprintfs ax\n");
    printf("\tend\n");
}

void err_overflow(){
    printf(":msgErrOverflow\n");
    printf("@string \"Erreur dépassement de capacité\\n\"\n");
    printf(":errOverflow\n");
    printf("\tconst ax,msgErrOverflow\n");
    printf("\tcallprintfs ax\n");
    printf("\tend\n");
}

void start_asm(){
printf("\tconst ax,debut\n");
printf("\tjmp ax\n");
printf(":nl\n");
printf("@string \"\\n\"\n");
err_div();
err_overflow();
printf(":debut\n");
printf("\tconst bp,pile\n");
printf("\tconst sp,pile\n");
printf("\tconst ax,2\n");
printf("\tsub sp,ax\n");
printf(";Début du code effectué par yyparse()\n");
}

void end_asm(){
printf(";Fin du code effectué par yyparse()\n");
printf("\tcp ax,sp\n");
printf("\tcallprintfd ax\n");
printf("\tconst ax,nl\n");
printf("\tcallprintfs ax\n");
printf("\tpop ax\n");
printf("\tend\n");
printf(":pile\n");
printf("@int 0\n");
}


void addition(){
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,errOverflow\n");
    printf("\tadd ax,bx\n");
    printf("\tjmpe cx\n");
    printf("\tpush ax\n");
}


void division(){
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    printf("\tconst cx,errDiv0\n");
    printf("\tdiv ax,bx\n");
    printf("\tjmpe cx\n");
    printf("\tpush ax\n");
}

void multiplication(){
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,errOverflow\n");
    printf("\tmul ax,bx\n");
    printf("\tjmpe cx\n");
    printf("\tpush ax\n");
}

void soustraction()
{
    printf("\tpop bx\n");
    printf("\tpop ax\n");
    printf("\tsub ax,bx\n");
    printf("\tpush ax\n");
}

void num(int number){
    printf("\tconst ax,%d\n",number);
    printf("\tpush ax\n");
}

void affectation(int i, int j){
    printf("\tconst ax,%d\n",i);
    printf("\tpush ax\n");
    printf("\tconst ax,%d\n",j);
    printf("\tpush ax\n");
}

void ajouter(int val, char* nom, sym_tab** head){
    
    sym_tab *cel = nouvelle_cellule();
    if(cel == NULL)
        exit(EXIT_FAILURE);
    strcpy(cel->nom_idf,nom);
    cel->type = val;
    cel->ptr = *head;
    *head = cel;

}
sym_tab* nouvelle_cellule() {
    sym_tab* cel = (sym_tab*)malloc(sizeof(sym_tab));
    return cel;
}

void print_sym_tab(sym_tab *head){
    printf("------------------------\n");
    printf("--idf---------type-------\n");
    while(head != NULL)
    {
        printf("  %s          %d\n",head->nom_idf,head->type);
        head=head->ptr;
    }
    
}