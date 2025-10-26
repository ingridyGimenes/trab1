#include <stdio.h>
#include <stdlib.h>
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "../tads_trabalho/geometria.h" // aqui ficam as implementações das funções geométricas

typedef struct {
    char tipo;
    void* obj;
} FormaWrap;

FORMA criaForma(char tipo, void* objeto) {
    FormaWrap* f = malloc(sizeof(FormaWrap));
    if (!f) {
        fprintf(stderr, "Erro ao alocar forma genérica.\n");
        exit(1);
    }
    f->tipo = tipo;
    f->obj = objeto;
    return (FORMA)f;
}

char tipoForma(FORMA f) {
    if (!f) return '\0';
    return ((FormaWrap*)f)->tipo;
}

void* objetoForma(FORMA f) {
    if (!f) return NULL;
    return ((FormaWrap*)f)->obj;
}

void destruirForma(FORMA f) {
    if (!f) return;
    free(f);
}

// A partir daqui, as funções geométricas chamam implementações em geometria.c

bool formasIntersectam(FORMA A, FORMA B) {
    return geometriaFormasIntersectam(A, B);
}

double areaForma(FORMA F) {
    return geometriaAreaForma(F);
}

void bordaDeBRecebeCorpDeA(FORMA A, FORMA B) {
    geometriaBordaDeBRecebeCorpDeA(A, B);
}

FORMA clonaFormaComCoresTrocadas(FORMA A, int novoId) {
    return geometriaClonaFormaComCoresTrocadas(A, novoId);
}
