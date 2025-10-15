#include <stdio.h>
#include <stdlib.h>
#include "processador.h"
#include "fila.h"
#include "carregador.h"
#include "disparador.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

struct processador {
    Fila* chao;
    Carregador* c1;
    Carregador* c2;
    Disparador* disp;
};

// ---------------------------------------------------
// Criação e destruição
// ---------------------------------------------------

Processador* criaProcessador() {
    Processador* p = (Processador*)malloc(sizeof(Processador));
    if (!p) {
        printf("Erro ao criar processador!\n");
        exit(1);
    }

    p->chao = cria_fila();
    p->c1 = criaCarregador(1);
    p->c2 = criaCarregador(2);
    p->disp = criaDisparador(p->c1, p->c2);

    printf("✅ Processador criado com sucesso!\n");
    return p;
}

void destroiProcessador(Processador* p) {
    if (!p) return;
    destroi_fila(p->chao);
    destroiCarregador(p->c1);
    destroiCarregador(p->c2);
    destroiDisparador(p->disp);
    free(p);
    printf("🗑️ Processador destruído.\n");
}

// ---------------------------------------------------
// Criação de formas (adiciona no chão)
// ---------------------------------------------------

void criaForma(Processador* p, int tipo) {
    if (!p) return;
    void* novaForma = NULL;

    switch (tipo) {
        case 1:
            novaForma = criaCirculoInterno(1, 10, 10, 5, "blue", "red");
            printf("➕ Círculo criado e adicionado ao chão!\n");
            break;
        case 2:
            novaForma = cria_retangulo(2, 0, 0, 30, 15, "black", "yellow");
            printf("➕ Retângulo criado e adicionado ao chão!\n");
            break;
        case 3:
            novaForma = cria_linha(3, 0, 10, 0, 10, "purple");
            printf("➕ Linha criada e adicionada ao chão!\n");
            break;
        case 4:
            novaForma = cria_texto(4, 5, 5, "black", "white", "n", "Ola!");
            printf("➕ Texto criado e adicionado ao chão!\n");
            break;
        default:
            printf("Tipo de forma inválido!\n");
            return;
    }

    insere_na_fila(p->chao, novaForma);
}

// ---------------------------------------------------
// Carregamento de formas
// ---------------------------------------------------

void carregaFormas(Processador* p, int idCarregador, int n) {
    if (!p) return;

    Carregador* c = (idCarregador == 1) ? p->c1 : p->c2;

    for (int i = 0; i < n; i++) {
        void* forma = remove_da_fila(p->chao);
        if (!forma) {
            printf("⚠️  O chão está vazio!\n");
            break;
        }
        addFormaCarregador(c, forma);
    }

    printf("📦 %d formas carregadas no carregador %d.\n", n, idCarregador);
}

// ---------------------------------------------------
// Disparo
// ---------------------------------------------------

void disparaForma(Processador* p, int lado) {
    if (!p) return;

    void* forma = NULL;
    if (lado == 1)
        forma = disparaEsquerda(p->disp);
    else if (lado == 2)
        forma = disparaDireita(p->disp);
    else {
        printf("Lado inválido (1=esquerda, 2=direita)\n");
        return;
    }

    if (forma)
        printf("💥 Forma disparada com sucesso!\n");
}

// ---------------------------------------------------
// Impressão
// ---------------------------------------------------

void imprimeEstado(Processador* p) {
    printf("\n========== ESTADO ATUAL ==========\n");
    printf("Formas no chão: \n");
    imprime_fila(p->chao);
    imprimeEstadoDisparador(p->disp);
    printf("==================================\n\n");
}
