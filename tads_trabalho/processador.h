#pragma once
#include "/workspaces/trab1/tads_gerais/fila.h"
#include "/workspaces/trab1/tads_gerais/pilha.h"
#include "/workspaces/trab1/formas/forma.h"

typedef struct processador* PROCESSADOR;

// Cria e inicializa um novo processador
PROCESSADOR criaProcessador(void);

// Processa colisões na arena e pontua
double processarArena(PROCESSADOR p, FILA arena, FILA chao);

// Estatísticas
double getPontuacaoTotal(PROCESSADOR p);
int getTotalClones(PROCESSADOR p);
int getTotalEsmagadas(PROCESSADOR p);
int getTotalDisparos(PROCESSADOR p);

// Atualiza contadores
void registrarDisparo(PROCESSADOR p);

// Libera memória
void destruirProcessador(PROCESSADOR p);
