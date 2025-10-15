#ifndef PROCESSADOR_H
#define PROCESSADOR_H

#include "fila.h"
#include "carregador.h"
#include "disparador.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

/**
 * Estrutura do processador principal.
 * Ele controla o chão, os carregadores e o disparador.
 */
typedef struct processador Processador;

/**
 * Cria o processador com uma fila (chão) e um disparador com dois carregadores.
 */
Processador* criaProcessador();

/**
 * Cria e adiciona uma forma nova no chão.
 * Tipo: 1=círculo, 2=retângulo, 3=linha, 4=texto
 */
void criaForma(Processador* p, int tipo);

/**
 * Carrega n formas do chão para o carregador indicado.
 */
void carregaFormas(Processador* p, int idCarregador, int n);

/**
 * Dispara uma forma de um dos lados (1=esquerdo, 2=direito).
 */
void disparaForma(Processador* p, int lado);

/**
 * Mostra o estado atual (fila + carregadores).
 */
void imprimeEstado(Processador* p);

/**
 * Libera toda a memória do processador.
 */
void destroiProcessador(Processador* p);

#endif
