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
    /*Mettre toutes les fonctions ici*/
    
}

void main_asm(){
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

void prepare_stack(func_tab *func){
    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
    printf("\t\n");

}

int test_expr_int(int first, int second){
    if(first != NUM_T || second != NUM_T)
        return ERR_T;
    return NUM_T;
}

int test_expr_bool(int first, int second){
    if(first != BOOL_T || second != BOOL_T)
        return ERR_T;
    return BOOL_T;
}

void addition(){
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,errOverflow\n");
    printf("\tadd ax,bx\n");
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

void multiplication(){
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,errOverflow\n");
    printf("\tmul ax,bx\n");
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


void num(int number){
    printf("\tconst ax,%d\n",number);
    printf("\tpush ax\n");
}


void affectation(char* var1, char* var2,sym_tab* head){
    printf(";affectation de %s = %s\n",var1,var2);
    get_param_from_stack(var2,head);
    printf("\tcp ax,bp\n");
    printf("\tconst bx,%d\n",get_param_location(var1,head));
    printf("\tadd ax,bx\n");
    printf("\tstorew dx,ax\n");
}

void affect_from_top_stack(char *nom,sym_tab* head){
    printf("\tpop dx\n");
    //print_param(nom,head);
    printf("\tcp bx,bp\n");
    printf("\tconst ax,%d\n",get_param_location(nom,head));
    printf("\tadd bx,ax\n");
    printf("\tstorew dx,bx\n");
    //print_param(nom,head);
}

void increment(char* nom,sym_tab* head){
    printf(";increment : %s\n",nom);
    get_param_from_stack(nom,head);
    //print_reg("bx");
    printf("\tconst ax,1\n");
    printf("\tadd dx,ax\n");
    printf("\tstorew dx,bx\n");
    //affiche le registre qui contient nom; print_reg("bx");
    printf(";end increment\n");
}

void decrement(char* nom,sym_tab* head){
    printf(";decrement : %s\n",nom);
    get_param_from_stack(nom,head);
    //print_reg("bx");
    printf("\tconst ax,1\n");
    printf("\tsub dx,ax\n");
    printf("\tstorew dx,bx\n");
    //affiche le registre qui contient nom; print_reg("bx");
    printf(";end decrement\n");
}




sym_tab* nouvelle_cellule() {
    sym_tab* cel = (sym_tab*)malloc(sizeof(sym_tab));
    return cel;
}

func_tab *nouvelle_cellule_func(){
    func_tab *cel = (func_tab*)malloc(sizeof(func_tab));
    return cel;
}

void ajouter(int val, char* nom, char* nom_func, func_tab* head){
    
    sym_tab *cel = nouvelle_cellule();
    if(cel == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    func_tab *func = recherche_func(nom_func,head);
    fprintf(stderr,"func->table :%p\n",(void *)func->table);
    if (func == NULL) {
        fprintf(stderr,"Function %s not found!",nom_func);
        exit(EXIT_FAILURE);
    }

    cel->nom_idf=strdup(nom);
    cel->type = val;
    cel->num_var = (val == PARAM_VAR) ? param_number : local_number;
    cel->ptr = func->table;
    func->table = cel;
    fprintf(stderr,"func->table :%p\n",(void *)func->table);

}

void ajouter_func(char *nom, int nbr_p, int nbr_v,func_tab **head){
    func_tab *cel = nouvelle_cellule_func();
    if(cel == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    

    cel->nom_func=strdup(nom);
    cel->nbr_params = nbr_p;
    cel->nbr_locals = nbr_v;
    cel->table =  NULL;
    cel->ptr = *head;
    *head = cel;
}

sym_tab* recherche(char* nom, sym_tab* head){
    while (head != NULL){
        if (!strcmp(head->nom_idf,nom)){
            break;
        }
        head=head->ptr;
    }
    return head;
}

func_tab *recherche_func(char *nom, func_tab *head){
    while (head != NULL){
        if (!strcmp(head->nom_func,nom)){
            break;
        }
        head=head->ptr;
    }
    return head;
}

void print_sym_tab(sym_tab *head){
    printf(";------------------------\n");
    //printf("--idf---------type-------\n");
    while(head != NULL)
    {
        printf(";name %s--type %d--num_val %d\n",head->nom_idf,head->type,head->num_var);
        head=head->ptr;
    }
    
}



void get_param_from_stack(char *nom,sym_tab* head){
    printf(";get_param_from_stack:%s\n",nom);
    printf("\tcp bx,bp\n");
    printf("\tconst ax,%d\n",get_param_location(nom,head));
    printf("\tadd bx,ax\n");
    printf("\tloadw dx,bx\n");
    printf(";end get_param\n");
}

void set_param_from_stack(char *nom, sym_tab* head){
    printf(";set_param_from_stack : %s\n",nom);
    printf("\tloadw bx,sp\n");
    printf("\tconst ax,%d\n",get_param_location(nom,head));
}

int get_param_location(char *nom,sym_tab* head){
    sym_tab *node = recherche(nom,head);
    if(node == NULL){
        perror("recherche");
        exit(EXIT_FAILURE);
    }
    return node->num_var*2;
}


void print_param(char* nom,sym_tab* head){
    printf(";start print_param\n");
    printf("\tcp bx,bp\n");
    printf("\tconst cx,%d\n",get_param_location(nom,head));
    printf("\tadd bx,cx\n");
    printf("\tcallprintfd bx\n");
    printf("\tconst cx,nl\n");
    printf("\tcallprintfs cx\n");
    printf(";end print_param\n");
}

void print_reg(char *nom){
    printf(";printing %s register\n",nom);
    printf("\tcallprintfd %s\n",nom);
    printf("\tconst cx,nl\n");
    printf("\tcallprintfs cx\n");
    printf(";%s register printed\n",nom);
}