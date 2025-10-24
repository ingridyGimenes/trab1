#ifndef PROCESSADOR_H
#define PROCESSADOR_H
#include "/workspaces/trab1/tads_gerais/fila.h"
#include "/workspaces/trab1/formas/circulo.h"
#include "/workspaces/trab1/formas/retangulo.h"
#include "/workspaces/trab1/formas/linha.h"
#include "/workspaces/trab1/formas/texto.h"

/// Estrutura opaca do Processador
typedef struct processador Processador;

/**
 * Cria um novo processador.
 */
Processador* cria_processador(void);

/**
 * Processa as formas da arena, aplicando as regras do enunciado.
 * Parâmetros:
 *   arena → fila com as formas disparadas
 *   chao  → fila com as formas “no chão”
 * 
 * Retorna: área total esmagada nesta rodada.
 */
double processa_formas(Processador* p, Fila* arena, Fila* chao);

/**
 * Retorna a pontuação total acumulada (soma de todas as rodadas).
 */
double get_pontuacao_total(Processador* p);

/**
 * Retorna o número total de formas clonadas até agora.
 */
int get_total_clones(Processador* p);

/**
 * Retorna o número total de formas esmagadas até agora.
 */
int get_total_esmagadas(Processador* p);

/**
 * Libera toda a memória associada ao processador.
 */
void libera_processador(Processador* p);

#endif
