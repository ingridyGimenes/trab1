#include <stdio.h>
#include <stdlib.h>

#include "processador.h"
#include "geometria.h"

#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

// Para destruir a forma concreta corretamente
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"

struct processador {
    double pontuacaoTotal;
    int totalClones;
    int totalEsmagadas;
    int totalDisparos;
};

// ---------- helpers internos ----------

// destrói a forma concreta e o wrapper
static void destruirFormaDeep(FORMA f) {
    if (!f) return;
    char t = tipoForma(f);
    void* o = objetoForma(f);

    switch (t) {
        case 'c': destruirCirculo((CIRCULO)o); break;
        case 'r': destruirRetangulo((RETANGULO)o); break;
        case 'l': destruirLinha((LINHA)o); break;
        case 't': destruirTexto((TEXTO)o); break;
        default: break;
    }
    destruirForma(f);
}

// ---------- API ----------

PROCESSADOR criaProcessador(void) {
    PROCESSADOR p = (PROCESSADOR)malloc(sizeof(*p));
    if (!p) {
        fprintf(stderr, "[processador] erro ao alocar.\n");
        exit(1);
    }
    p->pontuacaoTotal = 0.0;
    p->totalClones = 0;
    p->totalEsmagadas = 0;
    p->totalDisparos = 0;
    return p;
}

void destruirProcessador(PROCESSADOR p) {
    if (!p) return;
    free(p);
}

void registrarDisparo(PROCESSADOR p, int qtd) {
    if (!p) return;
    if (qtd < 0) qtd = 0;
    p->totalDisparos += qtd;
}

double processarArena(PROCESSADOR p, FILA arena, FILA chao) {
    if (!p || !arena || !chao) return 0.0;
    if (filaVazia(arena)) return 0.0;

    double areaEsmagadaRodada = 0.0;
    int proximoIdClone = 10000; // base para IDs de clones

    // pega a primeira forma
    FORMA f1 = removeDaFila(arena);

    while (!filaVazia(arena)) {
        FORMA f2 = removeDaFila(arena);

        if (geometriaFormasIntersectam(f1, f2)) {
            double a1 = geometriaAreaForma(f1);
            double a2 = geometriaAreaForma(f2);

            if (a1 < a2) {
                // f1 esmagada, f2 sobrevive
                areaEsmagadaRodada += a1;
                p->totalEsmagadas++;

                insereNaFila(chao, f2);   // sobrevivente volta ao chão
                destruirFormaDeep(f1);    // destrói wrapper + objeto concreto
            }
            else if (a1 > a2) {
                // f2 esmagada, f1 sobrevive; borda de f2 recebe corp de f1
                areaEsmagadaRodada += a2;
                p->totalEsmagadas++;

                geometriaBordaDeBRecebeCorpDeA(f1, f2);

                // ambos vão ao chão
                insereNaFila(chao, f1);
                insereNaFila(chao, f2);

                // cria clone de f1 com cores invertidas
                FORMA clone = geometriaClonaFormaComCoresTrocadas(f1, proximoIdClone++);
                if (clone) {
                    insereNaFila(chao, clone);
                    p->totalClones++;
                }

                // prepara o próximo par: novo f1
                if (!filaVazia(arena))
                    f1 = removeDaFila(arena);
                else
                    f1 = NULL;

                continue; // já tratou o emparelhamento
            }
            else {
                // mesma área → ninguém esmagado
                insereNaFila(chao, f1);
                insereNaFila(chao, f2);
            }
        } else {
            // sem colisão → ambos voltam para o chão
            insereNaFila(chao, f1);
            insereNaFila(chao, f2);
        }

        // pega próximo f1
        if (!filaVazia(arena))
            f1 = removeDaFila(arena);
        else
            f1 = NULL;
    }

    // se ficou alguém desemparelhado como f1, devolve ao chão
    if (f1) insereNaFila(chao, f1);

    p->pontuacaoTotal += areaEsmagadaRodada;
    return areaEsmagadaRodada;
}

double getPontuacaoTotal(PROCESSADOR p) { return p ? p->pontuacaoTotal : 0.0; }
int getTotalClones(PROCESSADOR p)       { return p ? p->totalClones     : 0;   }
int getTotalEsmagadas(PROCESSADOR p)    { return p ? p->totalEsmagadas  : 0;   }
int getTotalDisparos(PROCESSADOR p)     { return p ? p->totalDisparos   : 0;   }
