#include "intermediare.h"

//corps des fonctions

void add_intermediare(intermediare **head, operation op, arg_type type, char *arg){
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
