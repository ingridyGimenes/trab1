#pragma once
#include <stdbool.h>

typedef void* FORMA;

// Criação e destruição de forma genérica
FORMA criaForma(char tipo, void* objeto);
char tipoForma(FORMA f);
void* objetoForma(FORMA f);
void destruirForma(FORMA f);

// Operações geométricas genéricas
bool formasIntersectam(FORMA A, FORMA B);
double areaForma(FORMA F);

// Operações de cor e clonagem
void bordaDeBRecebeCorpDeA(FORMA A, FORMA B);
FORMA clonaFormaComCoresTrocadas(FORMA A, int novoId);
