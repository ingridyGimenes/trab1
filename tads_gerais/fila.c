
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

typedef struct NO {
    void* dado;
    struct NO* prox;
} No;

typedef struct {
    No* comeco;
    No* fim;
    int tam;
} FilaInterna;

FILA criaFila(void) {
    FilaInterna* f = malloc(sizeof(FilaInterna));
    if (!f) {
        fprintf(stderr, "Erro ao alocar memória para fila.\n");
        exit(1);
    }
    f->comeco = NULL;
    f->fim = NULL;
    f->tam = 0;
    return (FILA)f;
}

bool insereNaFila(FILA fila, void* valor) {
    if (!fila) return false;

    FilaInterna* f = (FilaInterna*)fila;
    No* novo = malloc(sizeof(No));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar nó da fila.\n");
        return false;
    }

    novo->dado = valor;
    novo->prox = NULL;

    if (f->fim == NULL) {
        f->comeco = novo;
        f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }

    f->tam++;
    return true;
}

void* removeDaFila(FILA fila) {
    if (!fila) return NULL;
    FilaInterna* f = (FilaInterna*)fila;

    if (f->comeco == NULL) {
        return NULL; // fila vazia
    }

    No* aux = f->comeco;
    void* valor = aux->dado;

    f->comeco = aux->prox;
    if (f->comeco == NULL) {
        f->fim = NULL;
    }

    free(aux);
    f->tam--;
    return valor;
}

void* primeiroFila(FILA fila) {
    if (!fila) return NULL;
    FilaInterna* f = (FilaInterna*)fila;
    return (f->comeco ? f->comeco->dado : NULL);
}

void* segundoFila(FILA fila) {
    if (!fila) return NULL;
    FilaInterna* f = (FilaInterna*)fila;
    return (f->comeco && f->comeco->prox ? f->comeco->prox->dado : NULL);
}

bool filaVazia(FILA fila) {
    if (!fila) return true;
    FilaInterna* f = (FilaInterna*)fila;
    return (f->tam == 0);
}

int tamanhoFila(FILA fila) {
    if (!fila) return 0;
    FilaInterna* f = (FilaInterna*)fila;
    return f->tam;
}

void imprimeFila(FILA fila, void (*printFn)(void*)) {
    if (!fila) return;
    FilaInterna* f = (FilaInterna*)fila;
    No* aux = f->comeco;

    printf("Fila (tamanho: %d): ", f->tam);
    while (aux) {
        if (printFn) printFn(aux->dado);
        else printf("%p ", aux->dado);
        aux = aux->prox;
    }
    printf("\n");
}

void esvaziaFila(FILA fila, void (*freeFn)(void*)) {
    if (!fila) return;
    FilaInterna* f = (FilaInterna*)fila;
    No* atual = f->comeco;

    while (atual) {
        No* proximo = atual->prox;
        if (freeFn) freeFn(atual->dado);
        free(atual);
        atual = proximo;
    }

    f->comeco = NULL;
    f->fim = NULL;
    f->tam = 0;
}

void destruirFila(FILA fila, void (*freeFn)(void*)) {
    if (!fila) return;
    esvaziaFila(fila, freeFn);
    free(fila);
}
