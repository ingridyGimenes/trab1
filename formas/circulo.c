
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

     if (!corB) corB = "#000000";
    if (!corP) corP = "none";
    c->cor_b = malloc(strlen(corB) + 1);
    c->cor_p = malloc(strlen(corP) + 1);
    if (!c->cor_b || !c->cor_p) { fprintf(stderr, "Erro: malloc cores ret\n"); exit(1); }
    strcpy(c->cor_b, corB);
    strcpy(c->cor_p, corP);
   
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

void setCorBordaCirculo(CIRCULO c, const char* corB) {
    Circulo* cir = (Circulo*)c;
     if (!corB) corB = "#000000";
    size_t need = strlen(corB) + 1;
    if (!cir->cor_b || strlen(cir->cor_b) + 1 < need) {
        char* tmp = realloc(cir->cor_b, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor_b\n"); exit(1); }
        cir->cor_b = tmp;
    }
}

void setCorPreenchimentoCirculo(CIRCULO c, const char* corP) {
    Circulo* cir = (Circulo*)c;
      if (!corP) corP = "#000000";
    size_t need = strlen(corP) + 1;
    if (!cir->cor_b || strlen(cir->cor_b) + 1 < need) {
        char* tmp = realloc(cir->cor_b, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor_b\n"); exit(1); }
        cir->cor_b = tmp;
    }
}

double calculaAreaCirculo(CIRCULO c) {
    Circulo* cir = (Circulo*)c;
    return PI * cir->raio * cir->raio;
}

CIRCULO clonaCirculo(CIRCULO c, int novoId) {
    Circulo* orig = (Circulo*)c;
  return criaCirculo(novoId, orig->x, orig->y, orig->raio,
                         orig->cor_b, orig->cor_p);
}

void inverterCoresCirculo(CIRCULO c) {
    Circulo* cir = (Circulo*)c;
    char* temp = cir->cor_b;
    cir->cor_b = cir->cor_p;
    cir->cor_p = temp;
}

void destruirCirculo(CIRCULO* c) {
    if (!c || !*c) return;
    Circulo* cir = (Circulo*)(*c);
    free(cir->cor_b);
    free(cir->cor_p);
    free(cir);
    *c = NULL;
}
