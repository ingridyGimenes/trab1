#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "disparador.h"
#include "forma.h"

void inicializaDisparador(void) {
    srand((unsigned int)time(NULL));
}

void dispararForma(PILHA carregador, FILA arena) {
    if (!carregador || !arena) return;

    void* forma = pop(carregador);
    if (forma) {
        pushFila(arena, forma);
    } else {
        printf("[Disparador] Carregador vazio.\n");
    }
}

void dispararTudo(PILHA carregador, FILA arena) {
    if (!carregador || !arena) return;

    while (!pilhaVazia(carregador)) {
        void* forma = pop(carregador);
        if (forma) {
            pushFila(arena, forma);
        }
    }
}

void dispararAleatorio(PILHA carregador, FILA arena) {
    if (!carregador || !arena) return;

    // 1. Passar todos os elementos da pilha para um vetor temporário
    int n = tamanhoPilha(carregador);
    if (n == 0) return;

    void** formas = malloc(sizeof(void*) * n);
    for (int i = 0; i < n; i++) {
        formas[i] = pop(carregador);
    }

    // 2. Embaralhar vetor (Fisher-Yates)
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        void* temp = formas[i];
        formas[i] = formas[j];
        formas[j] = temp;
    }

    // 3. Inserir na arena na nova ordem
    for (int i = 0; i < n; i++) {
        pushFila(arena, formas[i]);
    }

    free(formas);
}

int carregadorVazio(PILHA carregador) {
    return pilhaVazia(carregador);
}
