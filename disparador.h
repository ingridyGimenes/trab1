#ifndef DISPARADOR_H
#define DISPARADOR_H

#include "carregador.h"

/**
 * Estrutura que representa o disparador.
 * Ele possui dois carregadores (esquerdo e direito)
 * e controla as ações de disparo e devolução de formas.
 */
typedef struct disparador Disparador;

/**
 * Cria um novo disparador com dois carregadores.
 */
Disparador* criaDisparador(Carregador* esquerdo, Carregador* direito);

/**
 * Dispara uma forma do carregador esquerdo (se houver).
 * Retorna o ponteiro da forma disparada (void*).
 */
void* disparaEsquerda(Disparador* d);

/**
 * Dispara uma forma do carregador direito (se houver).
 * Retorna o ponteiro da forma disparada (void*).
 */
void* disparaDireita(Disparador* d);

/**
 * Devolve uma forma para o carregador esquerdo.
 */
void devolveEsquerda(Disparador* d, void* forma);

/**
 * Devolve uma forma para o carregador direito.
 */
void devolveDireita(Disparador* d, void* forma);

/**
 * Imprime o estado atual dos carregadores.
 */
void imprimeEstadoDisparador(Disparador* d);

/**
 * Libera completamente a memória do disparador.
 * (Não libera os carregadores, pois eles pertencem ao processador.)
 */
void destroiDisparador(Disparador* d);

#endif
