#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

typedef struct NO {
    void* dado;
    struct NO* prox;
} No;

typedef struct {
    No* topo;
    int tam;
} PilhaInterna;

PILHA criaPilha(void) {
    PilhaInterna* p = malloc(sizeof(PilhaInterna));
    if (!p) {
        fprintf(stderr, "Erro ao alocar memória para pilha.\n");
        exit(1);
    }
    p->topo = NULL;
    p->tam = 0;
    return (PILHA)p;
}

bool push(PILHA pilha, void* valor) {
    if (!pilha) return false;
    PilhaInterna* p = (PilhaInterna*)pilha;

    No* novo = malloc(sizeof(No));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar nó da pilha.\n");
        return false;
    }

    novo->dado = valor;
    novo->prox = p->topo;
    p->topo = novo;
    p->tam++;
    return true;
}

void* pop(PILHA pilha) {
    if (!pilha) return NULL;
    PilhaInterna* p = (PilhaInterna*)pilha;

    if (p->topo == NULL) return NULL;

    No* aux = p->topo;
    void* valor = aux->dado;

    p->topo = aux->prox;
    free(aux);
    p->tam--;

    return valor;
}

void* topoPilha(PILHA pilha) {
    if (!pilha) return NULL;
    PilhaInterna* p = (PilhaInterna*)pilha;
    return (p->topo ? p->topo->dado : NULL);
}

void* segundoPilha(PILHA pilha) {
    if (!pilha) return NULL;
    PilhaInterna* p = (PilhaInterna*)pilha;
    return (p->topo && p->topo->prox ? p->topo->prox->dado : NULL);
}

bool pilhaVazia(PILHA pilha) {
    if (!pilha) return true;
    PilhaInterna* p = (PilhaInterna*)pilha;
    return (p->tam == 0);
}

int tamanhoPilha(PILHA pilha) {
    if (!pilha) return 0;
    PilhaInterna* p = (PilhaInterna*)pilha;
    return p->tam;
}

void imprimePilha(PILHA pilha, void (*printFn)(void*)) {
    if (!pilha) return;
    PilhaInterna* p = (PilhaInterna*)pilha;
    No* aux = p->topo;

    printf("Pilha (tamanho: %d): ", p->tam);
    while (aux) {
        if (printFn) printFn(aux->dado);
        else printf("%p ", aux->dado);
        aux = aux->prox;
    }
    printf("\n");
}

void esvaziaPilha(PILHA pilha, void (*freeFn)(void*)) {
    if (!pilha) return;
    PilhaInterna* p = (PilhaInterna*)pilha;
    No* atual = p->topo;

    while (atual) {
        No* proximo = atual->prox;
        if (freeFn) freeFn(atual->dado);
        free(atual);
        atual = proximo;
    }

    p->topo = NULL;
    p->tam = 0;
}

void destruirPilha(PILHA pilha, void (*freeFn)(void*)) {
    if (!pilha) return;
    esvaziaPilha(pilha, freeFn);
    free(pilha);
}
