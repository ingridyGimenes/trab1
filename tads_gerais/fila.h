#pragma once
#include <stdbool.h>

typedef void* FILA;

FILA criaFila(void);
bool insereNaFila(FILA fila, void* valor);
void* removeDaFila(FILA fila);
void* primeiroFila(FILA fila);
void* segundoFila(FILA fila);
bool filaVazia(FILA fila);
int tamanhoFila(FILA fila);
void imprimeFila(FILA fila, void (*printFn)(void*));

void esvaziaFila(FILA fila, void (*freeFn)(void*));
void destruirFila(FILA fila, void (*freeFn)(void*));
