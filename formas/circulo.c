#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"

#define PI 3.14159

typedef struct{
    int id;
    double x, y;
    double raio;
    char *cor_b, *cor_p;
}circulo;


CIRCLE criaCirculoInterno(int id, double x, double y, double r, const char* corb, const char* corp) {
    circulo* c = (circulo*)malloc(sizeof(circulo));
    if (c == NULL) {
        fprintf(stderr, "Erro na criação do círculo!\n");
        exit(1);
    }

    c->id = id;
    c->x = x;
    c->y = y;
    c->raio = r;

    c->cor_b = strdup(corb);
    c->cor_p = strdup(corp);

    return (CIRCLE)c;
}


int getIdCirculo(CIRCLE c) {
    return ((circulo*)c)->id;
}

double getXCirculo(CIRCLE c) {
    return ((circulo*)c)->x;
}

double getYCirculo(CIRCLE c) {
    return ((circulo*)c)->y;
}

double getRaioCirculo(CIRCLE c) {
    return ((circulo*)c)->raio;
}

const char* getCorBordaCirculo(CIRCLE c) {
    return ((circulo*)c)->cor_b;
}

const char* getCorPreenchimentoCirculo(CIRCLE c) {
    return ((circulo*)c)->cor_p;
}

void setIdCirculo(CIRCLE c, int id){
    circulo* cir = (circulo*)c;
    cir->id = id;
}

void setYCirculo(CIRCLE c, double y) {
    circulo* cir = (circulo*)c;
    cir->y = y;
}

void setXCirculo(CIRCLE c, double x) {
    circulo* cir = (circulo*)c;
    cir->x = x;
}

void setCorBordaCirculo(CIRCLE c, const char* novaCor) {
    circulo* cir = (circulo*)c;
    free(cir->cor_b);
    cir->cor_b = strdup(novaCor);
}

void setCorPreenchimentoCirculo(CIRCLE c, const char* novaCor) {
    circulo* cir = (circulo*)c;
    free(cir->cor_p);
    cir->cor_p = strdup(novaCor);
}

void setRCirculo(CIRCLE c, double r){

    circulo* cir = (circulo*)c;
    cir->raio = r;
}


double areaCirculo(CIRCLE c) {
    circulo* cir = (circulo*)c;
    return PI * cir->raio * cir->raio;
}


CIRCLE clonaCirculo(CIRCLE c, int novoId) {
    circulo* orig = (circulo*)c;
    circulo* novo = (circulo*)malloc(sizeof(circulo));

    novo->id = novoId;
    novo->x = orig->x;
    novo->y = orig->y;
    novo->raio = orig->raio;
    novo->cor_b = strdup(orig->cor_b);
    novo->cor_p = strdup(orig->cor_p);

    return (CIRCLE)novo;
}

void inverteCores(CIRCLE c){
    circulo* cir = (circulo*)c;
    circulo* aux = (circulo*)malloc(sizeof(circulo));
    if(aux== NULL){
        printf("erro ao trocar cores");
        exit(1);
    }
    aux->cor_b = cir->cor_b;
    cir->cor_b = cir->cor_p;
    cir->cor_p =  aux->cor_b;
    free(aux);
}


void destroiCirculo(CIRCLE c) {
    circulo* cir = (circulo*)c;
    if (!cir) return;
    free(cir->cor_b);
    free(cir->cor_p);
    free(cir);
}

