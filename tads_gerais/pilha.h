#pragma once
#include <stdbool.h>

/**
 * @typedef PILHA
 * @brief Tipo opaco para representar uma pilha genérica.
 */
typedef void* PILHA;

/**
 * @brief Cria uma nova pilha vazia.
 * @return Ponteiro para a pilha criada.
 */
PILHA criaPilha(void);

/**
 * @brief Empilha um elemento no topo da pilha.
 * @param pilha Ponteiro para a pilha.
 * @param valor Ponteiro para o valor a ser empilhado.
 * @return true se a operação for bem-sucedida; false caso contrário.
 */
bool push(PILHA pilha, void* valor);

/**
 * @brief Remove o elemento do topo da pilha.
 * @param pilha Ponteiro para a pilha.
 * @return Ponteiro para o elemento removido ou NULL se a pilha estiver vazia.
 */
void* pop(PILHA pilha);

/**
 * @brief Retorna o elemento do topo da pilha sem removê-lo.
 * @param pilha Ponteiro para a pilha.
 * @return Ponteiro para o elemento do topo ou NULL se a pilha estiver vazia.
 */
void* topoPilha(PILHA pilha);

/**
 * @brief Retorna o segundo elemento da pilha sem removê-lo.
 * @param pilha Ponteiro para a pilha.
 * @return Ponteiro para o segundo elemento ou NULL se a pilha tiver menos de dois elementos.
 */
void* segundoPilha(PILHA pilha);

/**
 * @brief Verifica se a pilha está vazia.
 * @param pilha Ponteiro para a pilha.
 * @return true se a pilha estiver vazia; false caso contrário.
 */
bool pilhaVazia(PILHA pilha);

/**
 * @brief Retorna a quantidade de elementos na pilha.
 * @param pilha Ponteiro para a pilha.
 * @return Número de elementos.
 */
int tamanhoPilha(PILHA pilha);

/**
 * @brief Imprime todos os elementos da pilha usando uma função fornecida.
 * @param pilha Ponteiro para a pilha.
 * @param printFn Função de callback usada para imprimir cada elemento.
 */
void imprimePilha(PILHA pilha, void (*printFn)(void*));

/**
 * @brief Esvazia completamente a pilha, aplicando uma função de liberação a cada elemento.
 * @param pilha Ponteiro para a pilha.
 * @param freeFn Função de callback usada para liberar cada elemento.
 */
void esvaziaPilha(PILHA pilha, void (*freeFn)(void*));

/**
 * @brief Destrói a pilha e libera todos os recursos associados.
 * @param pilha Ponteiro para a pilha.
 * @param freeFn Função de callback usada para liberar cada elemento.
 */
void destruirPilha(PILHA pilha, void (*freeFn)(void*));
