#pragma once
#include "../tads_gerais/pilha.h"
#include "../tads_gerais/fila.h"

/**
 * @typedef DISPARADOR
 * @brief Tipo opaco para representar um disparador responsável por lançar formas da pilha (carregador) para a arena (fila).
 */
typedef void* DISPARADOR;

/**
 * @brief Move uma quantidade específica de formas do carregador (pilha) para a arena (fila).
 *
 * @param carregador Pilha de onde as formas serão removidas.
 * @param arena      Fila para onde as formas serão enviadas.
 * @param quantidade Número de formas a serem disparadas.
 *
 * @note As formas são removidas do topo da pilha e inseridas no final da fila,
 *       preservando a ordem de disparo.
 */
void dispararForma(PILHA carregador, FILA arena, int quantidade);

/**
 * @brief Move todas as formas do carregador (pilha) para a arena (fila).
 *
 * @param carregador Pilha de onde todas as formas serão removidas.
 * @param arena      Fila para onde as formas serão enviadas.
 *
 * @note A pilha ficará vazia ao final da operação.
 */
void dispararTudo(PILHA carregador, FILA arena);
