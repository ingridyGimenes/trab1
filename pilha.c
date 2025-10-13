#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

typedef struct NO{

    void *dado;
    struct NO *prox;
}no;
 
typedef no* pont_no;

typedef struct{
    pont_no comeco;
}Pilha;

typedef Pilha* pont_p;

void cria_pilha(){
    pont_p p = (pont_p*)malloc(sizeof(Pilha));
    p->comeco = NULL;
    return (void*)p;
}

void add_na_pilha(void* pilha, void* valor){
    pont_p p = (pont_p)pilha;
    pont_no temp = (pont_no)malloc(sizeof(no));

     if(temp==NULL){
        print("erro ao alocar novo no");
        exit(1);
    }

    temp->dado = valor;
    temp->prox = p->comeco;
    p->comeco = temp;
    
    
}

void* retira_da_pilha(void* pilha){
    pont_p p = (pont_p)pilha;
    pont_no aux = p->comeco;
    if(aux == NULL){
        printf("pilha vazia. nao ha nada a ser removido!");
        return NULL;
    }
    void* valor = aux->dado;
    p->comeco = aux->prox;
    free(aux);
    return (void*)valor;
}

void imprime_pilha(void* pilha){
    pont_p p = (pont_p)pilha;
    pont_no aux = p->comeco;

    printf("pilha: \n");

    while(aux){
        printf("%p ", aux->dado);
        aux = aux->prox;
    }
    
    printf("\n");
}