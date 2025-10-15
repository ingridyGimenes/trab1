#include <stdio.h>
#include <stdlib.h>
#include "disparador.h"
#include "carregador.h"

struct disparador {
    Carregador* esquerdo;
    Carregador* direito;
};

// ---------------------------------------------------------
// Criação e destruição
// ---------------------------------------------------------
Disparador* criaDisparador(Carregador* esquerdo, Carregador* direito) {
    Disparador* d = (Disparador*)malloc(sizeof(Disparador));
    if (!d) {
        printf("Erro ao criar disparador!\n");
        exit(1);
    }

    d->esquerdo = esquerdo;
    d->direito = direito;
    return d;
}

void destroiDisparador(Disparador* d) {
    if (!d) return;
    free(d);
}

// ---------------------------------------------------------
// Disparo de formas
// ---------------------------------------------------------
void* disparaEsquerda(Disparador* d) {
    if (!d || carregadorVazio(d->esquerdo)) {
        printf("Carregador esquerdo vazio!\n");
        return NULL;
    }

    void* forma = retiraFormaCarregador(d->esquerdo);
    printf(" Disparador (esquerda): forma retirada do carregador %d!\n", getIdCarregador(d->esquerdo));
    return forma;
}

void* disparaDireita(Disparador* d) {
    if (!d || carregadorVazio(d->direito)) {
        printf(" Carregador direito vazio!\n");
        return NULL;
    }

    void* forma = retiraFormaCarregador(d->direito);
    printf(" Disparador (direita): forma retirada do carregador %d!\n", getIdCarregador(d->direito));
    return forma;
}

// ---------------------------------------------------------
// Devolução de formas
// ---------------------------------------------------------
void devolveEsquerda(Disparador* d, void* forma) {
    if (!d || !forma) return;
    addFormaCarregador(d->esquerdo, forma);
    printf(" Forma devolvida ao carregador esquerdo (%d)\n", getIdCarregador(d->esquerdo));
}

void devolveDireita(Disparador* d, void* forma) {
    if (!d || !forma) return;
    addFormaCarregador(d->direito, forma);
    printf("Forma devolvida ao carregador direito (%d)\n", getIdCarregador(d->direito));
}

// ---------------------------------------------------------
// Impressão do estado atual
// ---------------------------------------------------------
void imprimeEstadoDisparador(Disparador* d) {
    if (!d) return;
    printf("\n=== ESTADO DO DISPARADOR ===\n");
    imprimeCarregador(d->esquerdo);
    imprimeCarregador(d->direito);
    printf("============================\n\n");
}
