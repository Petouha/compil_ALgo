#include"functions.h"


int label = 0;

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
}

void end_asm(){
    printf("\tcp ax,sp\n");
    printf("\tcallprintfd ax\n");
    printf("\tconst ax,nl\n");
    printf("\tcallprintfs ax\n");
    printf("\tpop ax\n");
    printf("\tend\n");
    printf(":pile\n");
    printf("@int 0\n");
}

void prepare_stack_locals(func_tab *func){
    // Variables locales
    printf(";Variables locales\n");
    for (int i = 0; i < func->nbr_locals; i++){
        printf(";ajout de la v_loc : %d\n",i);
        printf("\tconst ax,0\n");
        printf("\tpush ax\n");
    }

}


void prepare_stack_func(func_tab *func){
    printf("\tpush bp\n");
    printf("\tcp bp,sp\n");
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

void or(){
    printf(";Debut or\n");
    char *jump = create_label("jump_or",label);
    char *end = create_label("end_or",label);
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tadd ax,bx\n");
    printf("\tconst cx,%s\n",jump);
    printf("\tconst bx,0\n");
    printf("\tcmp ax,bx\n");
    printf("\tjmpc cx\n");
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");
    printf("\tconst ax,%s\n",end);
    printf("\tjmp ax\n");
    printf(":%s\n",jump);    
    printf("\tpush ax\n");
    printf(":%s\n",end);
    label++;
    free(jump);
    free(end);
    printf(";Debut or\n");
}

void not(){
    printf(";Debut not\n");

    printf("\tpop bx\n");
    printf("\tconst ax,1\n");
    printf("\tsub ax,bx\n");
    printf("\tpush ax\n");

    printf(";Fin not\n");
}

void egal(){
    printf(";Debut égalité\n");
    char *jump = create_label("jump_egal",label);
    char *end = create_label("end_egal",label);
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,%s\n",jump);
    printf("\tcmp ax,bx\n");
    printf("\tjmpc cx\n");
    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
    printf("\tconst cx,%s\n",end);
    printf("\tjmp cx\n");
    printf(":%s\n",jump);
    printf("\tconst ax,1\n");   
    printf("\tpush ax\n");
    printf(":%s\n",end);
    printf(";Fin égalité\n");
    label++;
    free(jump);
    free(end);
}

void diff(){
    printf(";Debut différence\n");
    char *jump = create_label("jump_diff",label);
    char *end = create_label("end_diff",label);
    printf("\tpop ax\n");
    printf("\tpop bx\n");
    printf("\tconst cx,%s\n",jump);
    printf("\tcmp ax,bx\n");
    printf("\tjmpc cx\n");
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");
    printf("\tconst cx,%s\n",end);
    printf("\tjmp cx\n");
    printf(":%s\n",jump);
    printf("\tconst ax,0\n");   
    printf("\tpush ax\n");
    printf(":%s\n",end);
    printf(";Fin différence\n");
    label++;
    free(jump);
    free(end);

}

void inf(){
    printf(";Debut inf\n");
    char *jump = create_label("jump_inf",label);
    char *end = create_label("end_inf",label);
    //pop les deux valeurs
    printf("\tpop bx\n");
    printf("\tpop ax\n");

    printf("\tconst cx,%s\n", jump);
    printf("\tsless ax,bx\n");
    printf("\tjmpc cx\n");

    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
    printf("\tconst dx,%s\n", end);
    printf("\tjmp dx\n");

    printf(":%s\n", jump);
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");

    printf(":%s\n", end);

    printf(";Fin inf\n");
    label++;
    free(jump);
    free(end);
}

void sup(){
    printf(";Debut sup\n");
    char *jump = create_label("jump_sup",label);
    char *end = create_label("end_sup",label);
    //pop les deux valeurs
    printf("\tpop bx\n");
    printf("\tpop ax\n");

    printf("\tconst cx,%s\n", jump);
    printf("\tsless bx,ax\n");
    printf("\tjmpc cx\n");

    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
    printf("\tconst dx,%s\n", end);
    printf("\tjmp dx\n");

    printf(":%s\n", jump);
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");

    printf(":%s\n", end);

    printf(";Fin sup\n");
    label++;
    free(jump);
    free(end);
}

void infeq(){
    printf(";Debut infeq\n");
    char *jump = create_label("jump_infeq",label);
    char *end = create_label("end_infeq",label);
    //pop les deux valeurs
    printf("\tpop bx\n");
    printf("\tpop ax\n");

    printf("\tconst cx,%s\n", jump);
    printf("\tcmp ax,bx\n");
    printf("\tjmpc cx\n");
    printf("\tsless ax,bx\n");
    printf("\tjmpc cx\n");

    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
    printf("\tconst dx,%s\n", end);
    printf("\tjmp dx\n");

    printf(":%s\n", jump);
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");

    printf(":%s\n", end);

    printf(";Fin infeq\n");
    label++;
    free(jump);
    free(end);
}

void supeq(){
    printf(";Debut supeq\n");
    char *jump = create_label("jump_supeq",label);
    char *end = create_label("end_supeq",label);
    //pop les deux valeurs
    printf("\tpop bx\n");
    printf("\tpop ax\n");

    printf("\tconst cx,%s\n", jump);
    printf("\tcmp ax,bx\n");
    printf("\tjmpc cx\n");
    printf("\tsless bx,ax\n");
    printf("\tjmpc cx\n");

    printf("\tconst ax,0\n");
    printf("\tpush ax\n");
    printf("\tconst dx,%s\n", end);
    printf("\tjmp dx\n");

    printf(":%s\n", jump);
    printf("\tconst ax,1\n");
    printf("\tpush ax\n");

    printf(":%s\n", end);

    printf(";Fin supeq\n");
    label++;
    free(jump);
    free(end);
}

void num(int number){
    printf("\tconst ax,%d\n",number);
    printf("\tpush ax\n");
}

void affect_from_top_stack(char *nom,func_tab* head){
    printf(";Affectation de %s\n",nom);
    printf("\tpop dx\n");
    printf("\tcp bx,bp\n");
    printf("\tconst ax,%d\n",get_param_location(nom,head));
    printf("\tsub bx,ax\n");
    printf("\tstorew dx,bx\n");
    printf(";Fin de l'affectation de %s\n",nom);
}

void increment(char* nom,func_tab* head){
    printf(";increment : %s\n",nom);
    get_param_from_stack(nom,head);
    printf("\tpop dx\n");
    printf("\tconst ax,1\n");
    printf("\tadd dx,ax\n");
    printf("\tstorew dx,bx\n");
    printf(";end increment\n");
}

void decrement(char* nom,func_tab* head){
    printf(";decrement : %s\n",nom);
    get_param_from_stack(nom,head);
    //print_reg("bx");
    printf("\tpop dx\n");
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
    if (func == NULL) {
        fprintf(stderr,"Function %s not found!",nom_func);
        exit(EXIT_FAILURE);
    }

    cel->nom_idf=strdup(nom);
    cel->type = val;
    cel->num_var = (val == PARAM_VAR) ? param_number : local_number;
    cel->ptr = func->table;
    func->table = cel;

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
        fprintf(stderr,";name %s--type %d--num_val %d\n",head->nom_idf,head->type,head->num_var);
        head=head->ptr;
    }
}

// génère du code pour une fonction qui parcours la strcuture func_tab en profondeur
void traverse_func_tab(func_tab *head) {
    if (head == NULL) {
        return;
    }
    fprintf(stderr, "--------Function: %s---------\n", head->nom_func);
    fprintf(stderr, "Number of parameters: %d\n", head->nbr_params);
    fprintf(stderr, "Number of locals: %d\n", head->nbr_locals);
    fprintf(stderr, "Symbol Table:\n");
    print_sym_tab(head->table);
    traverse_func_tab(head->ptr);
}


void get_param_from_stack(char *nom,func_tab *head){
    printf(";Récupérer le paramètre:%s\n",nom);
    printf("\tcp bx,bp\n");
    printf("\tconst ax,%d\n",get_param_location(nom,head));
    printf("\tsub bx,ax\n");
    printf("\tloadw dx,bx\n");
    printf("\tpush dx\n");
    printf(";Fin de la récupération de:%s\n",nom);
}

int get_param_location(char *nom,func_tab *head){
    sym_tab *node = recherche(nom,head->table);
    if(node == NULL){
        perror("recherche");
        exit(EXIT_FAILURE);
    }
    if(node->type == PARAM_VAR)
        return 2 + 2*head->nbr_locals + 2*head->nbr_params - node->num_var*2;
    return 2 + 2*head->nbr_locals - node->num_var*2;
}


void print_param(char* nom,func_tab* head){
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

void return_from_func(func_tab *head){
    // Mettre la valeur de retour
    printf(";Mettre la valeur de retour\n");
    printf("\tcp bx,bp\n");
    printf("\tpop ax\n");
    int n = 4 + head->nbr_locals * 2 + head->nbr_params * 2;
    printf("\tconst cx,%d\n",n);
    printf("\tsub bx,cx\n");
    printf("\tstorew ax,bx\n");

    // Restaurer la base de la pile
    printf(";Restaurer la base de la pile et mettre sp à bp\n");
    printf("\tcp sp,bp\n");
    printf("\tpop bp\n");
    printf("\tret\n");
}


void free_sym(sym_tab *sym) {
    if (sym == NULL)
        return;
    free(sym->nom_idf);
    free(sym);
}


void free_func(func_tab *func) {
    if (func == NULL)
        return;
    free(func->nom_func);
    free_sym(func->table); // libérer la table de symboles
    free_func(func->ptr); // libérer le prochain élément dans la liste
    free(func);
}
char* create_label(char* label_name, int number){
    char* buffer = (char*)malloc(128 * sizeof(char));
    if(buffer == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if(snprintf(buffer,128,"%s:%d",label_name,number) == -1){
        perror("snprintf");
        exit(EXIT_FAILURE);
    }
    return buffer;
}