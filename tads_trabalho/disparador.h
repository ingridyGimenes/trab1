#pragma once
#include "../tads_gerais/pilha.h"
#include "../tads_gerais/fila.h"

// Move formas do carregador (pilha) para a arena (fila)
void dispararForma(PILHA carregador, FILA arena, int quantidade);

// Move todas as formas da pilha para a fila
void dispararTudo(PILHA carregador, FILA arena);
