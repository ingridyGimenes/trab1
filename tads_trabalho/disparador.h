#pragma once
#include <stdbool.h>
#include "../tads_gerais/pilha.h"
#include "../tads_gerais/fila.h"
#include "../formas/forma.h"

typedef struct disparador* DISPARADOR;

DISPARADOR criaDisparador(int id);
void destruirDisparador(DISPARADOR d);

void setPosicaoDisparador(DISPARADOR d, double x, double y);
void getPosicaoDisparador(DISPARADOR d, double* x, double* y);

void atachaCarregadores(DISPARADOR d, PILHA esq, PILHA dir);

void shft(DISPARADOR d, char lado /*'e'|'d'*/, int n);
void dsp(DISPARADOR d, double dx, double dy, bool visualizar, FILA arena);
void rjd(DISPARADOR d, char lado, double dx, double dy, double ix, double iy, FILA arena);

void lc(FILA chao, PILHA carregador, int n);
