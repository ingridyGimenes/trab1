#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct {
    int id;
    double x;
    double y;
    double largura;
    double altura;
    char* cor_b;
    char* cor_p;
} Retangulo;

RETANGULO criaRetangulo(int id, double x, double y, double largura, double altura, const char* corB, const char* corP) {
    Retangulo* r = malloc(sizeof(Retangulo));
    if (!r) {
        fprintf(stderr, "Erro ao alocar memória para retângulo.\n");
        exit(1);
    }

    r->id = id;
    r->x = x;
    r->y = y;
    r->largura = largura;
    r->altura = altura;
    r->cor_b = strdup(corB);
    r->cor_p = strdup(corP);

    return (RETANGULO)r;
}

int getIdRetangulo(RETANGULO r) { return ((Retangulo*)r)->id; }
double getXRetangulo(RETANGULO r) { return ((Retangulo*)r)->x; }
double getYRetangulo(RETANGULO r) { return ((Retangulo*)r)->y; }
double getLarguraRetangulo(RETANGULO r) { return ((Retangulo*)r)->largura; }
double getAlturaRetangulo(RETANGULO r) { return ((Retangulo*)r)->altura; }
const char* getCorBordaRetangulo(RETANGULO r) { return ((Retangulo*)r)->cor_b; }
const char* getCorPreenchimentoRetangulo(RETANGULO r) { return ((Retangulo*)r)->cor_p; }

void setIdRetangulo(RETANGULO r, int id) { ((Retangulo*)r)->id = id; }
void setXRetangulo(RETANGULO r, double x) { ((Retangulo*)r)->x = x; }
void setYRetangulo(RETANGULO r, double y) { ((Retangulo*)r)->y = y; }
void setLarguraRetangulo(RETANGULO r, double w) { ((Retangulo*)r)->largura = w; }
void setAlturaRetangulo(RETANGULO r, double h) { ((Retangulo*)r)->altura = h; }

void setCorBordaRetangulo(RETANGULO r, const char* corB) {
    Retangulo* ret = (Retangulo*)r;
    free(ret->cor_b);
    ret->cor_b = strdup(corB);
}

void setCorPreenchimentoRetangulo(RETANGULO r, const char* corP) {
    Retangulo* ret = (Retangulo*)r;
    free(ret->cor_p);
    ret->cor_p = strdup(corP);
}

double calculaAreaRetangulo(RETANGULO r) {
    Retangulo* ret = (Retangulo*)r;
    return ret->largura * ret->altura;
}

RETANGULO clonaRetangulo(RETANGULO r, int novoId) {
    Retangulo* orig = (Retangulo*)r;
    Retangulo* novo = malloc(sizeof(Retangulo));
    if (!novo) {
        fprintf(stderr, "Erro ao clonar retângulo.\n");
        exit(1);
    }

    novo->id = novoId;
    novo->x = orig->x;
    novo->y = orig->y;
    novo->largura = orig->largura;
    novo->altura = orig->altura;
    novo->cor_b = strdup(orig->cor_b);
    novo->cor_p = strdup(orig->cor_p);

    return (RETANGULO)novo;
}

void inverterCoresRetangulo(RETANGULO r) {
    Retangulo* ret = (Retangulo*)r;
    char* temp = ret->cor_b;
    ret->cor_b = ret->cor_p;
    ret->cor_p = temp;
}

void destruirRetangulo(RETANGULO r) {
    Retangulo* ret = (Retangulo*)r;
    if (!ret) return;
    free(ret->cor_b);
    free(ret->cor_p);
    free(ret);
}
