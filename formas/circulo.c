#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "circulo.h"

/*
  TAD CÍRCULO (opaco)
  - struct interna só neste .c
  - mantém suas funções originais (getXCirculo, setCorBordaCirculo, ...)
  - adiciona ALIASES usados pelo geometria.c (get_x_circ, set_corb_circ, ...)
  - remove vazamentos em inverteCores (swap de ponteiros)
*/

#define PI 3.14159265358979323846

typedef struct {
    int id;
    double x, y;
    double raio;
    char *cor_b, *cor_p;
} circulo;

// --- helpers ---------------------------------------------------------------
static char* dupstr(const char* s){ if(!s) return NULL; size_t n=strlen(s)+1; char* d=(char*)malloc(n); if(d) memcpy(d,s,n); return d; }
static inline circulo* as(CIRCLE c){ return (circulo*)c; }

// --- criação / destruição --------------------------------------------------

CIRCLE cria_circulo(int id, double x, double y, double r, const char* corb, const char* corp) {
    circulo* c = (circulo*) malloc(sizeof(circulo));
    if (!c) { fprintf(stderr, "Erro na criação do círculo!\n"); return NULL; }
    c->id = id; c->x = x; c->y = y; c->raio = r;
    c->cor_b = dupstr(corb); c->cor_p = dupstr(corp);
    if ((corb && !c->cor_b) || (corp && !c->cor_p)) { free(c->cor_b); free(c->cor_p); free(c); return NULL; }
    return (CIRCLE)c;
}

void destruir_circulo(CIRCLE c) {
    circulo* cir = as(c);
    if (!cir) return;
    free(cir->cor_b);
    free(cir->cor_p);
    free(cir);
}

// --- getters originais -----------------------------------------------------
int    getIdCirculo(CIRCLE c)              { return as(c)->id; }
double getXCirculo(CIRCLE c)               { return as(c)->x; }
double getYCirculo(CIRCLE c)               { return as(c)->y; }
double getRaioCirculo(CIRCLE c)            { return as(c)->raio; }
const char* getCorBordaCirculo(CIRCLE c)   { return as(c)->cor_b; }
const char* getCorPreenchimentoCirculo(CIRCLE c){ return as(c)->cor_p; }

double areaCirculo(CIRCLE c)               { double r=as(c)->raio; return PI*r*r; }

// --- setters originais -----------------------------------------------------
void setIdCirculo(CIRCLE c, int id)                 { as(c)->id = id; }
void setXCirculo(CIRCLE c, double x)                { as(c)->x  = x; }
void setYCirculo(CIRCLE c, double y)                { as(c)->y  = y; }
void setRCirculo(CIRCLE c, double r)                { as(c)->raio = r; }
void setCorBordaCirculo(CIRCLE c, const char* cor)  { circulo* cir=as(c); char* n=dupstr(cor); if(!cor || n){ free(cir->cor_b); cir->cor_b=n; }}
void setCorPreenchimentoCirculo(CIRCLE c, const char* cor){ circulo* cir=as(c); char* n=dupstr(cor); if(!cor || n){ free(cir->cor_p); cir->cor_p=n; }}

// --- utilitários -----------------------------------------------------------
CIRCLE clonaCirculo(CIRCLE c, int novoId) {
    circulo* o = as(c); if(!o) return NULL;
    circulo* n = (circulo*) malloc(sizeof(circulo)); if(!n) return NULL;
    *n = *o; n->id = novoId;
    n->cor_b = dupstr(o->cor_b); n->cor_p = dupstr(o->cor_p);
    if ((o->cor_b && !n->cor_b) || (o->cor_p && !n->cor_p)) { free(n->cor_b); free(n->cor_p); free(n); return NULL; }
    return (CIRCLE) n;
}

void inverteCores_cir(CIRCLE c){
    circulo* cir = as(c); if(!cir) return;
    char* tmp = cir->cor_b; cir->cor_b = cir->cor_p; cir->cor_p = tmp; // swap sem alocação
}

// --- aliases para integração com geometria.c -------------------------------
// getters equivalentes
int    get_id_circ(CIRCLE c)           { return getIdCirculo(c); }
double get_x_circ(CIRCLE c)            { return getXCirculo(c); }
double get_y_circ(CIRCLE c)            { return getYCirculo(c); }
double get_r_circ(CIRCLE c)            { return getRaioCirculo(c); }
const char* get_corb_circ(CIRCLE c)    { return getCorBordaCirculo(c); }
const char* get_corp_circ(CIRCLE c)    { return getCorPreenchimentoCirculo(c); }

double area_circulo(CIRCLE c)          { return areaCirculo(c); }

// setters equivalentes
void set_corb_circ(CIRCLE c, const char* cor) { setCorBordaCirculo(c, cor); }
void set_corp_circ(CIRCLE c, const char* cor) { setCorPreenchimentoCirculo(c, cor); }

// util equivalentes
CIRCLE clona_circulo(CIRCLE c, int novoId)  { return clonaCirculo(c, novoId); }
void   inverte_cores_circ(CIRCLE c)         { inverteCores_cir(c); }
void   destroiCirculo(CIRCLE c)             { destruir_circulo(c); } // compatibilidade com seu nome
