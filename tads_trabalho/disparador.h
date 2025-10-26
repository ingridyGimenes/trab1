#pragma once
#include <stdbool.h>
#include "../tads_gerais/pilha.h"
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

/**
 * @typedef DISPARADOR
 * @brief Tipo opaco para representar um disparador com posição e dois carregadores (esquerda/direita).
 */
typedef struct disparador* DISPARADOR;

/**
 * @brief Cria um disparador com identificador informado.
 * @param id Identificador do disparador.
 * @return Ponteiro para o disparador criado.
 */
DISPARADOR criaDisparador(int id);

/**
 * @brief Libera os recursos associados ao disparador.
 * @param d Ponteiro para o disparador.
 */
void destruirDisparador(DISPARADOR d);

/**
 * @brief Define a posição do disparador no plano.
 * @param d Ponteiro para o disparador.
 * @param x Coordenada x.
 * @param y Coordenada y.
 */
void setPosicaoDisparador(DISPARADOR d, double x, double y);

/**
 * @brief Obtém a posição do disparador no plano.
 * @param d Ponteiro para o disparador.
 * @param x Ponteiro de saída para a coordenada x.
 * @param y Ponteiro de saída para a coordenada y.
 */
void getPosicaoDisparador(DISPARADOR d, double* x, double* y);

/**
 * @brief Acopla dois carregadores ao disparador.
 * @param d   Ponteiro para o disparador.
 * @param esq Pilha do carregador da esquerda.
 * @param dir Pilha do carregador da direita.
 */
void atachaCarregadores(DISPARADOR d, PILHA esq, PILHA dir);

/**
 * @brief Rotaciona a carga para a posição de disparo a partir de um lado.
 * @param d    Ponteiro para o disparador.
 * @param lado Lado a operar: 'e' (esquerda) ou 'd' (direita).
 * @param n    Número de rotações (pressões de botão).
 *
 * @note A cada rotação, o topo do carregador escolhido vai para a posição de disparo.
 *       Se já houver forma na posição de disparo, ela é deslocada para o topo do lado oposto.
 */
void shft(DISPARADOR d, char lado /*'e'|'d'*/, int n);

/**
 * @brief Dispara a forma atualmente na posição de disparo aplicando deslocamento relativo.
 * @param d           Ponteiro para o disparador.
 * @param dx          Deslocamento em x relativo à posição do disparador.
 * @param dy          Deslocamento em y relativo à posição do disparador.
 * @param visualizar  Indica se devem ser registradas/indicadas visualmente informações do disparo.
 * @param arena       Fila onde a forma disparada é inserida.
 *
 * @note Se não houver forma na posição de disparo, não há efeito.
 */
void dsp(DISPARADOR d, double dx, double dy, bool visualizar, FILA arena);

/**
 * @brief Executa uma rajada de disparos a partir de um lado, com incrementos de deslocamento.
 * @param d     Ponteiro para o disparador.
 * @param lado  Lado a operar: 'e' (esquerda) ou 'd' (direita).
 * @param dx    Deslocamento inicial em x relativo à posição do disparador.
 * @param dy    Deslocamento inicial em y relativo à posição do disparador.
 * @param ix    Incremento em x aplicado a cada disparo subsequente.
 * @param iy    Incremento em y aplicado a cada disparo subsequente.
 * @param arena Fila onde cada forma disparada é inserida.
 *
 * @note A operação repete a sequência (shft 1x no lado informado, dsp com deslocamento corrente)
 *       até esvaziar o carregador correspondente.
 */
void rjd(DISPARADOR d, char lado, double dx, double dy, double ix, double iy, FILA arena);

/**
 * @brief Carrega n formas do chão para um carregador.
 * @param chao        Fila com as formas disponíveis.
 * @param carregador  Pilha do carregador que receberá as formas.
 * @param n           Quantidade de formas a carregar (na ordem em que estão no chão).
 */
void lc(FILA chao, PILHA carregador, int n);
