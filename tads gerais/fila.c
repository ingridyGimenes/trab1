#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

typedef struct NO{

    void *dado;
    struct NO *prox;
}no;
 
typedef no* pont_no;

typedef struct{
    pont_no comeco;
    pont_no fim;
    int tam;
}Fila;

typedef Fila* pont_f;

void cria_fila(){
   pont_f f = (pont_f)malloc(sizeof(Fila));
   if(f==NULL){
    printf("erro ao alocar memoria da fila");
    return NULL;
   }
    f->comeco = NULL;
    f->fim = NULL;
    f->tam = 0;
    return (void*)f;
}

void add_na_fila(void* fila, void* valor){
    pont_f f = (pont_f)fila;
    pont_no temp = (pont_no)malloc(sizeof(no));

    if(temp==NULL){
        print("erro ao alocar novo no");
        exit(1);
    }

    if(f->fim==NULL){
        f->comeco = temp;
        f->fim = temp;
        return;
    }

    f->fim->prox = temp;
    f->fim = temp;
    f->tam++;
}

void* remove_da_fila(void* fila){
        pont_f f = (pont_f)fila;

        if(f->fim==NULL){
            printf("fila vazia. Nao existe elemento a ser removido!");
            return NULL;
        }

        pont_no aux = f->comeco;
        void* valor = aux->dado;
        f->comeco = aux->prox;

        if(f->comeco == NULL){
            f->fim = NULL;
        }

        f->tam--;
        free(aux);
         
        return (void*)valor;
}

void imprime_fila(void* fila){
    pont_f f = (pont_f)fila;
    pont_no aux = f->fim;

    printf("fila: \n");

    while(aux){
        printf("%p ",  aux->dado);
        aux= aux->prox;
    }

    printf("\n");
}

void* esvazia_fila(void* fila){
    pont_f f = (pont_f)fila;

        if(f->fim==NULL){
            printf("fila vazia. Nao existe elemento a ser removido!");
            return NULL;
        }

        pont_no aux = f->comeco;

        while(aux){
        pont_no proximo = aux->prox;
        free(aux);
        aux = proximo;
        
    }

    f->comeco = NULL;
    f->fim == NULL;

        free(aux);
       
}

void* primeiro_fila(void* fila){
    pont_f f = (pont_f)fila;
    if(f==NULL){
         printf("elementos insuficientes");
        return NULL;
    }

    return f->comeco->dado;
}

void* primeiro_fila(void* fila){
    pont_f f = (pont_f)fila;
    if(f==NULL || f->comeco->prox==NULL){
        printf("elementos insuficientes");
        return NULL;
    }

    return f->comeco->prox->dado;
}

int tamanho_fila(void* fila){
    pont_f f = (pont_f)fila;
    return f->tam;
}