#include <stdio.h>
#include <stdlib.h>
#include "processador.h"
#include "geometria.h"
#include "disparador.h"
#include "/workspaces/trab1/tads_gerais/fila.h"
#include "/workspaces/trab1/tads_gerais/pilha.h"
#include "/workspaces/trab1/formas/forma.h"

struct processador {
    double pontuacaoTotal;
    int totalClones;
    int totalEsmagadas;
    int totalDisparos;
};

PROCESSADOR criaProcessador(void) {
    PROCESSADOR p = malloc(sizeof(*p));
    if (!p) {
        fprintf(stderr, "Erro ao criar processador.\n");
        exit(1);
    }
    p->pontuacaoTotal = 0;
    p->totalClones = 0;
    p->totalEsmagadas = 0;
    p->totalDisparos = 0;
    return p;
}

void registrarDisparo(PROCESSADOR p) {
    if (p) p->totalDisparos++;
}

double processarArena(PROCESSADOR p, FILA arena, FILA chao) {
    if (!p || !arena) return 0.0;

    double areaEsmagadaRodada = 0.0;
    int proximoIdClone = 10000; // base para IDs novos

    // Retira a primeira forma da arena
    FORMA f1 = popFila(arena);

    while (!filaVazia(arena)) {
        FORMA f2 = popFila(arena);

        if (geometriaFormasIntersectam(f1, f2)) {
            double a1 = geometriaAreaForma(f1);
            double a2 = geometriaAreaForma(f2);

            if (a1 < a2) {
                // f1 é esmagada
                areaEsmagadaRodada += a1;
                p->totalEsmagadas++;
                pushFila(chao, f2);
                destruirForma(f1);
            }
            else if (a1 > a2) {
                // f2 é esmagada
                areaEsmagadaRodada += a2;
                p->totalEsmagadas++;

                geometriaBordaDeBRecebeCorpDeA(f1, f2);
                pushFila(chao, f1);
                pushFila(chao, f2);

                // cria clone de f1 com cores trocadas
                FORMA clone = geometriaClonaFormaComCoresTrocadas(f1, proximoIdClone++);
                pushFila(chao, clone);
                p->totalClones++;
            }
            else {
                // mesma área → nenhuma é esmagada
                pushFila(chao, f1);
                pushFila(chao, f2);
            }
        } else {
            // sem colisão → voltam pro chão
            pushFila(chao, f1);
            pushFila(chao, f2);
        }

        // próxima iteração
        if (!filaVazia(arena))
            f1 = popFila(arena);
        else
            f1 = NULL;
    }

    if (f1) pushFila(chao, f1);

    p->pontuacaoTotal += areaEsmagadaRodada;
    return areaEsmagadaRodada;
}

double getPontuacaoTotal(PROCESSADOR p) {
    return p ? p->pontuacaoTotal : 0.0;
}

int getTotalClones(PROCESSADOR p) {
    return p ? p->totalClones : 0;
}

int getTotalEsmagadas(PROCESSADOR p) {
    return p ? p->totalEsmagadas : 0;
}

int getTotalDisparos(PROCESSADOR p) {
    return p ? p->totalDisparos : 0;
}

void destruirProcessador(PROCESSADOR p) {
    if (!p) return;
    free(p);
}
