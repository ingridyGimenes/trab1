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
    int tam;
}Pilha;

typedef Pilha* pont_p;

void* cria_pilha(){
    pont_p p = (pont_p)malloc(sizeof(Pilha));

    if(p==NULL){
        printf("erro ao alocar pilha");
        return NULL;
    }

    p->comeco = NULL;
    p->tam = 0;

    return (void*)p;
}

pont_p add_na_pilha(void* pilha, void* valor){
    pont_p p = (pont_p)pilha;
    pont_no temp = (pont_no)malloc(sizeof(no));

     if(temp==NULL){
        printf("erro ao alocar novo no");
        exit(1);
    }

    temp->dado = valor;
    temp->prox = p->comeco;
    p->comeco = temp;
    p->tam++;
    
    
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
    p->tam--;

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

void esvazia_pilha(void* pilha){
    pont_p p = (pont_p)pilha;
    pont_no aux = p->comeco;

    if(aux == NULL){
        printf("Pilha vazia, nao ha elemento a ser retirado.");
        return NULL;
    }

    while(aux){
        pont_no proximo = aux->prox;
        free(aux);
        aux = proximo;
    }

    p->comeco = NULL;
    p->tam = 0;
}

void* primeiro_pilha(void* pilha){
    pont_p p = (pont_p)pilha;
    if(p==NULL){
         printf("elementos insuficientes");
        return NULL;
    }

    return p->comeco->dado;
}

void* segundo_pilha(void* pilha){
    pont_p p = (pont_p)pilha;
    if(p==NULL || p->comeco->prox == NULL){
         printf("elementos insuficientes");
        return NULL;
    }

    return p->comeco->prox->dado;
}

int tamanho_pilha(void* pilha){
    pont_p p = (pont_p)pilha;
    return p->tam;
}