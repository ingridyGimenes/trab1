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
}Fila;

typedef Fila* pont_f;

void cria_lista(){
   pont_f f = (pont_f)malloc(sizeof(Fila));
    f->comeco = NULL;
    f->fim = NULL;

    return (void*)f;
}

void add_na_fila(void* fila, void* valor){
    pont_f f = (pont_f)fila;
    pont_no temp = (pont_no)malloc(sizeof(no));

    if(f->fim==NULL){
        f->comeco = temp;
        f->fim = temp;
        return;
    }

    f->fim->prox = temp;
    f->fim = temp;
}

void* remove_da_fila(void* fila){
        pont_f f = (pont_f)fila;

        if(f->fim==NULL){
            printf("fila vazia. Nao existe elemento a ser removido!");
            return NULL;
        }

        pont_no aux = f->comeco;
        void* valor = aux->dado;
        f->comeco = f->comeco->prox;
        if(f->comeco == NULL){
            f->fim = NULL;
        }

        free(aux);
        return valor;
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