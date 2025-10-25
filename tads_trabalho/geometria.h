#pragma once
#include <stdbool.h>
#include "../formas/forma.h"

// Verifica se duas formas se intersectam
bool geometriaFormasIntersectam(FORMA A, FORMA B);

// Calcula a área de uma forma genérica
double geometriaAreaForma(FORMA F);

// Clona uma forma com cores invertidas (quando houver borda/preenchimento)
FORMA geometriaClonaFormaComCoresTrocadas(FORMA F, int novoId);

// Faz a borda (ou cor) de B receber a cor de preenchimento de A
void geometriaBordaDeBRecebeCorpDeA(FORMA A, FORMA B);
