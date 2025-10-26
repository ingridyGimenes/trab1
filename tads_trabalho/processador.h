#pragma once

#include "../tads_gerais/fila.h"   // para o tipo FILA
#include "../tads_gerais/pilha.h"  // (se não usar aqui, pode remover)
#include "../formas/forma.h"       // para o tipo FORMA (apenas referências)

/**
 * @typedef PROCESSADOR
 * Tipo opaco que representa o processador (estatísticas + regras de colisão).
 */
typedef struct processador* PROCESSADOR;

/** Callback para marcar visualmente (ex.: asterisco no SVG) o ponto de esmagamento. */
typedef void (*PROC_MARK_CB)(double x, double y, void *ctx);

/**
 * Liga/desliga o callback de marcação. O módulo .qry injeta o contexto do SVG via ctx.
 */
void processador_set_mark_cb(PROCESSADOR p, PROC_MARK_CB cb, void *ctx);

/** Cria/destrói o processador. */
PROCESSADOR criaProcessador(void);
void destruirProcessador(PROCESSADOR p);

/** Estatísticas de disparo. */
void registrarDisparo(PROCESSADOR p, int qtd);

/**
 * Processa a arena (pares I,J), aplicando as regras do PDF:
 * - Se colide e área(I) < área(J): I é esmagado (pontua e é destruído; J volta ao chão).
 * - Se colide e área(I) > área(J): borda(J) recebe corp(I); ambos voltam; clona(I) com cores trocadas.
 * - Áreas iguais ou sem colisão: ambos voltam ao chão.
 * Retorna a área esmagada nesta rodada.
 */
double processarArena(PROCESSADOR p, FILA arena, FILA chao);

/** Getters de estatísticas acumuladas. */
double getPontuacaoTotal(PROCESSADOR p);
int     getTotalClones(PROCESSADOR p);
int     getTotalEsmagadas(PROCESSADOR p);
int     getTotalDisparos(PROCESSADOR p);
