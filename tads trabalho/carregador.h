#ifndef CARREGADOR_H
#define CARREGADOR_H
#include <stdbool.h>
#include "pilha.h"


typedef struct carregador Carregador;

/**
 * Cria um novo carregador vazio.
 * Retorna: ponteiro para o carregador criado, ou NULL se falha.
 */
Carregador* cria_carregador(int id);

/**
 * Retorna o identificador do carregador.
 */
int get_id_carregador(Carregador* c);

/**
 * Adiciona uma forma (qualquer tipo) no carregador.
 * Retorna: true se sucesso, false caso contrário.
 */
bool adiciona_forma_carregador(Carregador* c, void* forma);

/**
 * Remove e retorna a forma do topo do carregador.
 * Retorna: ponteiro para a forma ou NULL se vazio.
 */
void* remove_forma_carregador(Carregador* c);

/**
 * Retorna a forma do topo sem removê-la.
 */
void* topo_carregador(Carregador* c);

/**
 * Verifica se o carregador está vazio.
 */
bool carregador_vazio(Carregador* c);

/**
 * Retorna o número de formas armazenadas.
 */
int tamanho_carregador(Carregador* c);

/**
 * Libera toda a memória do carregador (não libera as formas).
 */
void libera_carregador(Carregador* c);

#endif
