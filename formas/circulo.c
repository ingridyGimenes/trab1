#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "circulo.h"

#define PI 3.14159

typedef struct {
    int id;
    double x, y;
    double raio;
    char *cor_b, *cor_p;
} Circulo;

CIRCULO criaCirculo(int id, double x, double y, double r, const char* corB, const char* corP) {
    Circulo* c = malloc(sizeof(Circulo));
    if (!c) {
        fprintf(stderr, "Erro: falha ao alocar memória para círculo.\n");
        exit(1);
    }

    c->id = id;
    c->x = x;
    c->y = y;
    c->raio = r;
    c->cor_b = strdup(corB);
    c->cor_p = strdup(corP);
    return (CIRCULO)c;
}

int getIdCirculo(CIRCULO c) { return ((Circulo*)c)->id; }
double getXCirculo(CIRCULO c) { return ((Circulo*)c)->x; }
double getYCirculo(CIRCULO c) { return ((Circulo*)c)->y; }
double getRaioCirculo(CIRCULO c) { return ((Circulo*)c)->raio; }
const char* getCorBordaCirculo(CIRCULO c) { return ((Circulo*)c)->cor_b; }
const char* getCorPreenchimentoCirculo(CIRCULO c) { return ((Circulo*)c)->cor_p; }

void setIdCirculo(CIRCULO c, int id) { ((Circulo*)c)->id = id; }
void setXCirculo(CIRCULO c, double x) { ((Circulo*)c)->x = x; }
void setYCirculo(CIRCULO c, double y) { ((Circulo*)c)->y = y; }
void setRaioCirculo(CIRCULO c, double r) { ((Circulo*)c)->raio = r; }

void setCorBordaCirculo(CIRCULO c, const char* novaCor) {
    Circulo* cir = (Circulo*)c;
    free(cir->cor_b);
    cir->cor_b = strdup(novaCor);
}

void setCorPreenchimentoCirculo(CIRCULO c, const char* novaCor) {
    Circulo* cir = (Circulo*)c;
    free(cir->cor_p);
    cir->cor_p = strdup(novaCor);
}

double calculaAreaCirculo(CIRCULO c) {
    Circulo* cir = (Circulo*)c;
    return PI * cir->raio * cir->raio;
}

CIRCULO clonaCirculo(CIRCULO c, int novoId) {
    Circulo* orig = (Circulo*)c;
    Circulo* novo = malloc(sizeof(Circulo));
    if (!novo) {
        fprintf(stderr, "Erro ao clonar círculo.\n");
        exit(1);
    }

    novo->id = novoId;
    novo->x = orig->x;
    novo->y = orig->y;
    novo->raio = orig->raio;
    novo->cor_b = strdup(orig->cor_b);
    novo->cor_p = strdup(orig->cor_p);
    return (CIRCULO)novo;
}

void inverterCoresCirculo(CIRCULO c) {
    Circulo* cir = (Circulo*)c;
    char* temp = cir->cor_b;
    cir->cor_b = cir->cor_p;
    cir->cor_p = temp;
}

void destruirCirculo(CIRCULO c) {
    Circulo* cir = (Circulo*)c;
    if (!cir) return;
    free(cir->cor_b);
    free(cir->cor_p);
    free(cir);
}
