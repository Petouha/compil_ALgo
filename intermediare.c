#include "intermediare.h"
#include "functions.h"

//corps des fonctions

void add_intermediare(intermediare **head, operation op, arg_type type, char *arg, func_tab* func){
    intermediare *new = (intermediare *)malloc(sizeof(intermediare));
    fprintf(stderr,"je vais ajouter %d\n",op);
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
            fprintf(stderr,"OR_OP\n");
            break;
        case NOT_OP:
            fprintf(stderr,"NOT_OP\n");
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
            break;
        case CALLEND_OP:
            printf(";callend\n");
            prepare_stack_params(node->function);
            prepare_stack_locals(node->function);
            printf("\tconst ax,%s\n",node->function->nom_func);
            printf("\tcall ax\n");
            printf(";end callend\n");
            printf(";Dépiler le nombre de variables locales + paramètres\n");
            for (int i = 0; i < node->function->nbr_locals + node->function->nbr_params; i++){
            printf("\tpop ax\n");
        }
            end_asm();
            break;
        case RET_OP:
            return_from_func(node->function,node->arg);
            break;
        default:
            break;
    }
}