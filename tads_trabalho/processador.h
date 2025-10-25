#pragma once
#include "../tads_gerais/fila.h"
#include "../tads_gerais/pilha.h"
#include "../formas/forma.h"

typedef struct processador* PROCESSADOR;

// cria / destrói
PROCESSADOR criaProcessador(void);
void destruirProcessador(PROCESSADOR p);

// registra disparos (ex.: pd, dsp, shft)
void registrarDisparo(PROCESSADOR p, int qtd);

// processa colisões da arena e joga sobreviventes/gerados no chão
// retorna a área total esmagada nesta rodada
double processarArena(PROCESSADOR p, FILA arena, FILA chao);

// métricas
double getPontuacaoTotal(PROCESSADOR p);
int getTotalClones(PROCESSADOR p);
int getTotalEsmagadas(PROCESSADOR p);
int getTotalDisparos(PROCESSADOR p);
