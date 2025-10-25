#pragma once
#include <stdbool.h>

typedef void* PILHA;

PILHA criaPilha(void);

bool push(PILHA pilha, void* valor);
void* pop(PILHA pilha);
void* topoPilha(PILHA pilha);
void* segundoPilha(PILHA pilha);
bool pilhaVazia(PILHA pilha);
int tamanhoPilha(PILHA pilha);
void imprimePilha(PILHA pilha, void (*printFn)(void*));

void esvaziaPilha(PILHA pilha, void (*freeFn)(void*));
void destruirPilha(PILHA pilha, void (*freeFn)(void*));
