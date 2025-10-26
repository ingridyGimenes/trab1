#pragma once
#include <stdbool.h>

/**
 * @typedef FILA
 * @brief Tipo opaco para representar uma fila genérica.
 */
typedef void* FILA;

/**
 * @brief Cria uma nova fila vazia.
 * @return Ponteiro para a fila criada.
 */
FILA criaFila(void);

/**
 * @brief Insere um elemento no final da fila.
 * @param fila Ponteiro para a fila.
 * @param valor Ponteiro para o valor a ser inserido.
 * @return true se a inserção foi bem-sucedida; false caso contrário.
 */
bool insereNaFila(FILA fila, void* valor);

/**
 * @brief Remove o elemento do início da fila.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para o elemento removido ou NULL se a fila estiver vazia.
 */
void* removeDaFila(FILA fila);

/**
 * @brief Retorna o elemento no início da fila sem removê-lo.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para o primeiro elemento ou NULL se a fila estiver vazia.
 */
void* primeiroFila(FILA fila);

/**
 * @brief Retorna o segundo elemento da fila sem removê-lo.
 * @param fila Ponteiro para a fila.
 * @return Ponteiro para o segundo elemento ou NULL se a fila tiver menos de dois elementos.
 */
void* segundoFila(FILA fila);

/**
 * @brief Verifica se a fila está vazia.
 * @param fila Ponteiro para a fila.
 * @return true se a fila estiver vazia; false caso contrário.
 */
bool filaVazia(FILA fila);

/**
 * @brief Retorna a quantidade de elementos na fila.
 * @param fila Ponteiro para a fila.
 * @return Número de elementos.
 */
int tamanhoFila(FILA fila);

/**
 * @brief Imprime todos os elementos da fila usando uma função de impressão fornecida.
 * @param fila Ponteiro para a fila.
 * @param printFn Função de callback usada para imprimir cada elemento.
 */
void imprimeFila(FILA fila, void (*printFn)(void*));

/**
 * @brief Esvazia completamente a fila, aplicando uma função de liberação a cada elemento.
 * @param fila Ponteiro para a fila.
 * @param freeFn Função de callback para liberar a memória de cada elemento.
 */
void esvaziaFila(FILA fila, void (*freeFn)(void*));

/**
 * @brief Destrói a fila e libera todos os recursos associados.
 * @param fila Ponteiro para a fila.
 * @param freeFn Função de callback para liberar a memória de cada elemento.
 */
void destruirFila(FILA fila, void (*freeFn)(void*));
