#pragma once
#include "pilha.h"
#include "fila.h"

// Cria um novo disparador
void inicializaDisparador(void);

// Dispara uma única forma do carregador para a arena
void dispararForma(PILHA carregador, FILA arena);

// Dispara todas as formas do carregador para a arena
void dispararTudo(PILHA carregador, FILA arena);

// Dispara de forma aleatória (se aplicável)
void dispararAleatorio(PILHA carregador, FILA arena);

// Retorna se o carregador ainda possui formas
int carregadorVazio(PILHA carregador);
