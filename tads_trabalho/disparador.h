#ifndef DISPARADOR_H
#define DISPARADOR_H

#include <stdbool.h>
#include "tads_trabalho/carregador.h"


typedef struct disparador Disparador;

/**
 * Cria um novo disparador com um identificador.
 */
Disparador* cria_disparador(int id);

/**
 * Posiciona o disparador na coordenada (x, y).
 */
void posiciona_disparador(Disparador* d, double x, double y);

/**
 * Conecta dois carregadores ao disparador:
 *  - cesq → carregador esquerdo
 *  - cdir → carregador direito
 */
void conecta_carregadores(Disparador* d, Carregador* cesq, Carregador* cdir);

/**
 * Pressiona o botão de shift (e = esquerdo, d = direito).
 * 
 * - Se já houver uma forma em posição de disparo, 
 *   ela é devolvida para o topo do outro carregador.
 * - Retira o topo do carregador correspondente e o coloca 
 *   na posição de disparo.
 * 
 * Retorna: ponteiro para a forma que ficou em posição de disparo.
 */
void* shift_disparador(Disparador* d, char lado);

/**
 * Dispara a forma atualmente em posição de disparo.
 * Move a forma para (x + dx, y + dy).
 * 
 * Retorna a forma disparada (para colocá-la na arena).
 */
void* disparar(Disparador* d, double dx, double dy, char modo);

/**
 * Retorna a coordenada X do disparador.
 */
double get_x_disparador(Disparador* d);

/**
 * Retorna a coordenada Y do disparador.
 */
double get_y_disparador(Disparador* d);

/**
 * Retorna o ID do disparador.
 */
int get_id_disparador(Disparador* d);

/**
 * Libera toda a memória associada ao disparador.
 * (Não libera os carregadores nem as formas.)
 */
void libera_disparador(Disparador* d);

#endif
