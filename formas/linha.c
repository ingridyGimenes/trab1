#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "linha.h"

/*
  TAD LINHA (opaco)
  - struct interna só neste .c
  - interface compatível com seus nomes (get_codx1_linha, ...)
  - adiciona aliases usados por geometria.c (get_x1_lin, set_cor_lin, ...)
  - corrige bugs em cria_linha e clona_linha
*/

typedef struct {
    int id;
    double x1, y1, x2, y2;
    char* cor;
} linha;

static inline linha* as(LINHA L){ return (linha*) L; }
static char* dupstr(const char* s){ if(!s) return NULL; size_t n=strlen(s)+1; char* d=(char*)malloc(n); if(d) memcpy(d,s,n); return d; }

// ---------------------------------------------------------------------------
// criação / destruição
// ---------------------------------------------------------------------------

LINHA cria_linha(int id, double x1, double y1, double x2, double y2, const char* corL){
    linha* l = (linha*) malloc(sizeof(linha));
    if(!l){ fprintf(stderr, "erro ao alocar linha\n"); return NULL; }
    l->id = id; l->x1 = x1; l->y1 = y1; l->x2 = x2; l->y2 = y2;
    l->cor = dupstr(corL);
    if(corL && !l->cor){ free(l); return NULL; }
    return (LINHA) l;
}

void destruir_linha(LINHA L){
    linha* l = as(L);
    if(!l) return;
    free(l->cor);
    free(l);
}

// ---------------------------------------------------------------------------
// getters originais
// ---------------------------------------------------------------------------

int get_id_linha(LINHA L){ return as(L)->id; }

double get_codx1_linha(LINHA L){ return as(L)->x1; }
double get_cody1_linha(LINHA L){ return as(L)->y1; }
double get_codx2_linha(LINHA L){ return as(L)->x2; }
double get_cody2_linha(LINHA L){ return as(L)->y2; }

char* get_cor_linha(LINHA L){ return as(L)->cor; }

// ---------------------------------------------------------------------------
// setters originais (seguros)
// ---------------------------------------------------------------------------

void set_codx1_linha(LINHA L, double x){ as(L)->x1 = x; }
void set_cody1_linha(LINHA L, double y){ as(L)->y1 = y; }
void set_codx2_linha(LINHA L, double x){ as(L)->x2 = x; }
void set_cody2_linha(LINHA L, double y){ as(L)->y2 = y; }

void set_cor_linha(LINHA L, const char* corL){
    linha* l = as(L);
    char* n = dupstr(corL);
    if(!corL || n){ free(l->cor); l->cor = n; }
}

// ---------------------------------------------------------------------------
// util
// ---------------------------------------------------------------------------

LINHA clona_linha(LINHA L, int novo_id){
    linha* o = as(L); if(!o) return NULL;
    linha* n = (linha*) malloc(sizeof(linha)); if(!n) return NULL;
    *n = *o; n->id = novo_id;
    n->cor = dupstr(o->cor);
    if(o->cor && !n->cor){ free(n); return NULL; }
    return (LINHA) n;
}

// ---------------------------------------------------------------------------
// aliases para geometria.c
// ---------------------------------------------------------------------------

double get_x1_lin(LINHA L){ return get_codx1_linha(L); }
double get_y1_lin(LINHA L){ return get_cody1_linha(L); }
double get_x2_lin(LINHA L){ return get_codx2_linha(L); }
double get_y2_lin(LINHA L){ return get_cody2_linha(L); }

const char* get_cor_lin(LINHA L){ return get_cor_linha(L); }
void set_cor_lin(LINHA L, const char* c){ set_cor_linha(L, c); }
