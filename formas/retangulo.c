
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

     if (!corB) corB = "#000000";
    if (!corP) corP = "none";
    r->cor_b = malloc(strlen(corB) + 1);
    r->cor_p = malloc(strlen(corP) + 1);
    if (!r->cor_b || !r->cor_p) { fprintf(stderr, "Erro: malloc cores ret\n"); exit(1); }
    strcpy(r->cor_b, corB);
    strcpy(r->cor_p, corP);
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
    if (!corB) corB = "#000000";
    size_t need = strlen(corB) + 1;
    if (!ret->cor_b || strlen(ret->cor_b) + 1 < need) {
        char* tmp = realloc(ret->cor_b, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor_b\n"); exit(1); }
        ret->cor_b = tmp;
    }
    strcpy(ret->cor_b, corB);
}

void setCorPreenchimentoRetangulo(RETANGULO r, const char* corP) {
    Retangulo* ret = (Retangulo*)r;
    if (!corP) corP = "none";
    size_t need = strlen(corP) + 1;
    if (!ret->cor_p || strlen(ret->cor_p) + 1 < need) {
        char* tmp = realloc(ret->cor_p, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor_p\n"); exit(1); }
        ret->cor_p = tmp;
    }
    strcpy(ret->cor_p, corP);
}

double calculaAreaRetangulo(RETANGULO r) {
    Retangulo* ret = (Retangulo*)r;
    return ret->largura * ret->altura;
}

RETANGULO clonaRetangulo(RETANGULO r, int novoId) {
    Retangulo* orig = (Retangulo*)r;
    return criaRetangulo(novoId, orig->x, orig->y, orig->largura, orig->altura,
                         orig->cor_b, orig->cor_p);
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
