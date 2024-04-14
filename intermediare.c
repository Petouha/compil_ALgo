#include "intermediare.h"
#include "functions.h"


//corps des fonctions

void add_cond(cond_tab **head, int id){
    cond_tab *new = (cond_tab *)malloc(sizeof(cond_tab));
    if(new == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->cond_id = id;
    new->next = NULL;
    
    if(*head == NULL){
        *head = new;
    }else{
        cond_tab *current = *head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new;
    }
}

int pop_cond(cond_tab **head){
    cond_tab *current = *head;
    if(current == NULL){
        return -1;
    }
    cond_tab *prev = NULL;
    while(current->next != NULL){
        prev = current;
        current = current->next;
    }
    int id = current->cond_id;
    if(prev != NULL){
        prev->next = NULL;
    }else{
        *head = NULL;
    }
    free(current);
    return id;
}

void free_cond(cond_tab **head){
    cond_tab *current = *head;
    cond_tab *tmp = current;
    while(tmp != NULL){
        tmp = current->next;
        free(tmp);
        current = tmp;
    }
    *head = NULL;
}

void add_intermediare(intermediare **head, operation op, arg_type type, char *arg, func_tab* func){
    intermediare *new = (intermediare *)malloc(sizeof(intermediare));
    //fprintf(stderr,"je vais ajouter %d\n",op);
    if(new == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new->type = type;
    if(type == OP){
        new->arg = NULL;
    }else{
        new->arg = strdup(arg);
    }
    
    new->op = op;
    new->function = func;
    new->next = NULL;
    if(*head == NULL){
        *head = new;
    }else{
        intermediare *current = *head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new;
    }
}


void generate_intermediare(intermediare *head){
    start_asm();
    while(head != NULL){
        choose_op(head);
        head = head->next;
    }
}

void free_intermediare(intermediare *head){
    intermediare *current = head;
    while(current != NULL){
        intermediare *tmp = current;
        current = current->next;
        if(tmp->type == ARG){
            free(tmp->arg);
        }
        free(tmp);
    }
    head = NULL;
}


void print_intermediare(intermediare *head){
    fprintf(stderr,"------------\n");
    while(head != NULL){
        if(head->type == OP){
            fprintf(stderr,"Opération: %d -- no arg\n",head->op);
        }
        if(head->type == ARG){
            fprintf(stderr,"Opération: %d -- arg1 : %s\n",head->op,head->arg);
        }
        head=head->next;
    }
    fprintf(stderr,"------------\n");
}


void choose_op(intermediare *node){
    switch(node->op){
        case ADD_OP:
            addition();
            break;
        case SUB_OP:
            soustraction();
            break;
        case MUL_OP:
            multiplication();
            break;
        case DIV_OP:
            division();
            break;
        case AND_OP:
            multiplication();
            break;
        case OR_OP:
            or();
            break;
        case NOT_OP:
            not();
            break;
        case SET_OP:
            affect_from_top_stack(node->arg,node->function);
            break;
        case INCR_OP:
            increment(node->arg,node->function);
            break;
        case DECR_OP:
            decrement(node->arg,node->function);
            break;
        case IDF_OP:
            get_param_from_stack(node->arg,node->function);
            break;
        case NUM_OP:
            num(atoi(node->arg));
            break;
        case NUL_OP:
            break;
        case FUNC_OP:
            printf(":%s\n",node->function->nom_func);
            prepare_stack_func(node->function);
            break;
        case CALL_OP:
            main_asm();
            // Valeur de retour
            printf(";Valeur de retour\n");
            printf("\tconst ax,0\n");
            printf("\tpush ax\n");
            printf(";Paramètres\n");
            break;
        case CALL_EXP_OP:
            printf(";préparation de l'appel de %s\n",node->function->nom_func);
            printf(";Valeur de retour\n");
            printf("\tconst ax,0\n");
            printf("\tpush ax\n");
            printf(";Paramètres\n"); 
            break;

        case CALL_EXP_PARAM_END_OP:
            printf(";Fin des paramètres\n");
            prepare_stack_locals(node->function);
            printf(";Appel de la fonction %s\n",node->function->nom_func);
            printf("\tconst ax,%s\n",node->function->nom_func);
            printf("\tcall ax\n");
            printf(";Dépiler le nombre de variables locales + paramètres\n");
            for (int i = 0; i < node->function->nbr_locals + node->function->nbr_params; i++)
                printf("\tpop ax\n");
            printf(";Fin de la dépilement\n");
            break;
        case CALL_PARAM_END_OP:
            prepare_stack_locals(node->function);
            printf(";Appel de la fonction %s\n",node->function->nom_func);
            printf("\tconst ax,%s\n",node->function->nom_func);
            printf("\tcall ax\n");
            printf(";Dépiler le nombre de variables locales + paramètres\n");
            for (int i = 0; i < node->function->nbr_locals + node->function->nbr_params; i++)
                printf("\tpop ax\n");
            printf(";Fin de la dépilement\n");
            end_asm();
            break;
        case RET_OP:
            return_from_func(node->function);
            break;
        case ARG_OP:
            break;
        case EGAL_OP:
            egal();
            break;
        case DIF_OP:
            diff();
            break;
        case IF_OP:
            printf(";If statement\n");
            char *if_jump=create_label("if_jump",atoi(node->arg));

            printf("\tpop ax\n");
            printf("\tconst bx,0\n");
            printf("\tconst cx,%s\n",if_jump);
            printf("\tcmp ax,bx\n");
            printf("\tjmpz cx\n");

            break;
        case FI_OP:
            printf(":%s\n",create_label("if_jump",atoi(node->arg)));
            break;

        case ELSE_OP:
            printf(";If jumping after Else\n");
            printf("\tconst ax,%s\n",create_label("if_jump",atoi(node->arg)));
            printf("\tjmp ax\n");
            break;
        case ELSE_2_OP:
            printf(";Else statement\n");
            char *else_jump=create_label("if_jump",atoi(node->arg));
            printf(":%s\n",else_jump);
            break;
        case DOWHILE_1_OP:
            printf(";Do while statement\n");
            char *do_while=create_label("do_while",atoi(node->arg));
            printf(":%s\n",do_while);
            break;
        case DOWHILE_2_OP:
            printf(";Testing condition\n");
            printf("\tpop ax\n");
            printf("\tconst bx,0\n");
            printf("\tconst cx,%s\n",create_label("do_while",atoi(node->arg)));
            printf("\tcmp ax,bx\n");
            printf("\tjmpz cx\n");
            break;
        case OD_1_OP:
            printf(";Jumping to the while\n");
            printf("\tconst ax,%s\n",create_label("do_while",atoi(node->arg)));
            printf("\tjmp ax\n");
            break;
        case OD_2_OP:
            printf(";End of the do while\n");
            printf(":%s\n",create_label("do_while",atoi(node->arg)));
            break;
        default:
        break;
    }
}