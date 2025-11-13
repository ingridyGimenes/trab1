
#include <string.h>
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

    /* callback para marcar asterisco no SVG do .qry */
    PROC_MARK_CB mark_cb;
    void *mark_ctx;
};

/* setter do callback (chamado pelo qry_io.c) */
void processador_set_mark_cb(PROCESSADOR p, PROC_MARK_CB cb, void *ctx) {
    if (!p) return;
    p->mark_cb = cb;
    p->mark_ctx = ctx;
}

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
    p->totalClones    = 0;
    p->totalEsmagadas = 0;
    p->totalDisparos  = 0;
    p->mark_cb        = NULL;  /* inicializa callback vazio */
    p->mark_ctx       = NULL;
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

    /* dica do PDF: manter “maior código” para clonar — aqui usamos uma base simples */
    int proximoIdClone = 10000; // base para IDs de clones

    /* pega a primeira forma (I) */
    FORMA f1 = removeDaFila(arena);

    while (!filaVazia(arena)) {
        /* pega a segunda forma (J) do par */
        FORMA f2 = removeDaFila(arena);

        if (geometriaFormasIntersectam(f1, f2)) {
            double a1 = geometriaAreaForma(f1);
            double a2 = geometriaAreaForma(f2);

            if (a1 < a2) {
                /* f1 é esmagada; f2 sobrevive e volta ao chão */
                areaEsmagadaRodada += a1;
                p->totalEsmagadas++;

                /* >>> NOVO: marca a posição da forma esmagada no SVG do .qry */
                if (p->mark_cb) {
                    double xi, yi;
                    geometriaAncoraDaForma(f1, &xi, &yi);  /* âncora do círculo/retângulo/texto; ponto médio se linha */
                    p->mark_cb(xi, yi, p->mark_ctx);
                }

                insereNaFila(chao, f2);   /* sobrevivente volta ao chão */
                destruirFormaDeep(f1);    /* destruímos a forma esmagada */

                /* próximo par começa da cabeça da arena */
                if (!filaVazia(arena))
                    f1 = removeDaFila(arena);
                else
                    f1 = NULL;

                continue; /* já lidamos com esse par */
            }
            else if (a1 > a2) {
                /* f2 NÃO é destruída: borda de f2 recebe corp de f1; ambos voltam;
                   cria-se um CLONE de f1 (cores trocadas) e entra após I e J. */

                geometriaBordaDeBRecebeCorpDeA(f1, f2);

                /* ambos voltam ao chão (na mesma ordem relativa) */
                insereNaFila(chao, f1);
                insereNaFila(chao, f2);

                /* cria clone de f1 com cores invertidas */
                FORMA clone = geometriaClonaFormaComCoresTrocadas(f1, proximoIdClone++);
                if (clone) {
                    insereNaFila(chao, clone);
                    p->totalClones++;
                }

                /* próximo par: novo f1 da arena, se houver */
                if (!filaVazia(arena))
                    f1 = removeDaFila(arena);
                else
                    f1 = NULL;

                continue;
            }
            else {
                /* mesma área: ninguém é destruído; ambos voltam ao chão */
                insereNaFila(chao, f1);
                insereNaFila(chao, f2);

                /* próximo f1 */
                if (!filaVazia(arena))
                    f1 = removeDaFila(arena);
                else
                    f1 = NULL;

                continue;
            }
        } else {
            /* sem colisão: ambos voltam ao chão, ordem relativa mantida */
            insereNaFila(chao, f1);
            insereNaFila(chao, f2);

            /* próximo f1 */
            if (!filaVazia(arena))
                f1 = removeDaFila(arena);
            else
                f1 = NULL;

            continue;
        }
    }

    /* se sobrou um f1 desemparelhado, devolve ao chão */
    if (f1) insereNaFila(chao, f1);

    p->pontuacaoTotal += areaEsmagadaRodada;
    return areaEsmagadaRodada;
}

double getPontuacaoTotal(PROCESSADOR p) { return p ? p->pontuacaoTotal : 0.0; }
int getTotalClones(PROCESSADOR p)       { return p ? p->totalClones     : 0;   }
int getTotalEsmagadas(PROCESSADOR p)    { return p ? p->totalEsmagadas  : 0;   }
int getTotalDisparos(PROCESSADOR p)     { return p ? p->totalDisparos   : 0;   }
