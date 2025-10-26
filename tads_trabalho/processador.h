#pragma once
#include "../tads_gerais/fila.h"
#include "../tads_gerais/pilha.h"
#include "../formas/forma.h"

/**
 * @typedef PROCESSADOR
 * @brief Tipo opaco para representar o processador responsável por registrar disparos,
 *        processar colisões e acumular estatísticas de jogo.
 */
typedef struct processador* PROCESSADOR;

/**
 * @brief Cria uma nova instância de processador.
 * @return Ponteiro para o processador criado.
 */
PROCESSADOR criaProcessador(void);

/**
 * @brief Libera todos os recursos associados ao processador.
 * @param p Ponteiro para o processador.
 */
void destruirProcessador(PROCESSADOR p);

/**
 * @brief Registra a ocorrência de disparos para fins estatísticos.
 * @param p Ponteiro para o processador.
 * @param qtd Quantidade de disparos efetuados.
 */
void registrarDisparo(PROCESSADOR p, int qtd);

/**
 * @brief Processa colisões entre as formas presentes na arena e atualiza pontuação e estatísticas.
 *
 * @param p Ponteiro para o processador.
 * @param arena Fila com as formas que foram disparadas.
 * @param chao  Fila onde as formas sobreviventes ou geradas serão colocadas após o processamento.
 * @return Área total esmagada nesta rodada.
 *
 * @note Formas esmagadas contribuem para a pontuação total.
 * @note Clones e mudanças de cor são aplicados conforme regras de colisão.
 */
double processarArena(PROCESSADOR p, FILA arena, FILA chao);

/**
 * @brief Retorna a pontuação total acumulada.
 * @param p Ponteiro para o processador.
 * @return Pontuação total.
 */
double getPontuacaoTotal(PROCESSADOR p);

/**
 * @brief Retorna o total de formas clonadas.
 * @param p Ponteiro para o processador.
 * @return Número de clones.
 */
int getTotalClones(PROCESSADOR p);

/**
 * @brief Retorna o total de formas esmagadas.
 * @param p Ponteiro para o processador.
 * @return Número de formas esmagadas.
 */
int getTotalEsmagadas(PROCESSADOR p);

/**
 * @brief Retorna o total de disparos registrados.
 * @param p Ponteiro para o processador.
 * @return Número total de disparos.
 */
int getTotalDisparos(PROCESSADOR p);
