#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "texto.h"

/*
  TAD TEXTO (opaco)
  - struct interna só neste .c
  - corrige criação, getters/setters e área (20.0 * |t|)
  - ancora como CHAR ('i'|'m'|'f'), conforme enunciado
  - adiciona aliases para geometria.c: get_x_txt, get_y_txt, get_ancora_txt, get_len_txt,
    get_corp_txt, set_corb_txt, clona_texto, inverte_cores_txt
*/

typedef struct{
    char* family;
    char* weight;
    char* size;
} estilo;

typedef struct{
    int id;
    double x;
    double y;
    char *corB;
    char *corP;
    char ancora;   // <- armazenar como caractere
    char *txto;
    estilo txtstyle;
} texto;

static inline texto* as(TEXTO t){ return (texto*) t; }
static inline estilo* asE(Estilo e){ return (estilo*) e; }

static char* dupstr(const char* s){ if(!s) return NULL; size_t n=strlen(s)+1; char* d=(char*)malloc(n); if(d) memcpy(d,s,n); return d; }

// ---------------------------------------------------------------------------
// criação / destruição
// ---------------------------------------------------------------------------

Estilo cria_estilo(const char* Family, const char* Weight, const char* Size ){
    estilo *ts = (estilo*) malloc(sizeof(estilo));
    if(!ts) { fprintf(stderr, "erro ao alocar estilo\n"); return NULL; }
    ts->family = dupstr(Family);
    ts->weight = dupstr(Weight);
    ts->size   = dupstr(Size);
    if ((Family && !ts->family) || (Weight && !ts->weight) || (Size && !ts->size)){
        free(ts->family); free(ts->weight); free(ts->size); free(ts);
        return NULL;
    }
    return (Estilo) ts;
}

void destruir_estilo(Estilo e){
    estilo* es = asE(e);
    if(!es) return;
    free(es->family); free(es->weight); free(es->size); free(es);
}

TEXTO cria_texto(int i, double x, double y, const char *cor_b, const char *cor_p,
                 char ancora, const char *TXTO){
    texto *t = (texto*) malloc(sizeof(texto));
    if(!t){ fprintf(stderr, "erro ao alocar texto\n"); return NULL; }
    t->id = i; t->x = x; t->y = y; t->ancora = ancora;
    t->corB = dupstr(cor_b);
    t->corP = dupstr(cor_p);
    t->txto = dupstr(TXTO);
    t->txtstyle.family = dupstr("sans");
    t->txtstyle.weight = dupstr("n");
    t->txtstyle.size   = dupstr("12");
    if ((cor_b && !t->corB) || (cor_p && !t->corP) || (TXTO && !t->txto) ||
        !t->txtstyle.family || !t->txtstyle.weight || !t->txtstyle.size){
        free(t->corB); free(t->corP); free(t->txto);
        free(t->txtstyle.family); free(t->txtstyle.weight); free(t->txtstyle.size);
        free(t); return NULL;
    }
    return (TEXTO) t;
}

void destruir_texto(TEXTO t){
    texto* tx = as(t);
    if(!tx) return;
    free(tx->corB); free(tx->corP); free(tx->txto);
    free(tx->txtstyle.family); free(tx->txtstyle.weight); free(tx->txtstyle.size);
    free(tx);
}

// ---------------------------------------------------------------------------
// getters originais (alguns ajustados)
// ---------------------------------------------------------------------------

int    get_id_txt(TEXTO t){ return as(t)->id; }
double get_codx_txt(TEXTO t){ return as(t)->x; }
double get_cody_txt(TEXTO t){ return as(t)->y; }
char*  get_corB_txt(TEXTO t){ return as(t)->corB; }
char*  get_corP_txt(TEXTO t){ return as(t)->corP; }
char   get_ancora_txt(TEXTO t){ return as(t)->ancora; } // agora retorna char
char*  get_txto_txt(TEXTO t){ return as(t)->txto; }

char*  getFamily(Estilo e){ return asE(e)->family; }
char*  getWeight(Estilo e){ return asE(e)->weight; }
char*  getSize(Estilo e){ return asE(e)->size; }

int    get_len_txt(TEXTO t){ return (int)(as(t)->txto ? (int)strlen(as(t)->txto) : 0); }

// ---------------------------------------------------------------------------
// setters (corrigidos)
// ---------------------------------------------------------------------------

void set_codx_txt(TEXTO t, double Cx){ as(t)->x = Cx; }
void set_cody_txt(TEXTO t, double Cy){ as(t)->y = Cy; }

void set_corB_txt(TEXTO t, const char* cor_b){
    texto* T = as(t); char* n = dupstr(cor_b);
    if(!cor_b || n){ free(T->corB); T->corB = n; }
}
void set_corP_txt(TEXTO t, const char* cor_p){
    texto* T = as(t); char* n = dupstr(cor_p);
    if(!cor_p || n){ free(T->corP); T->corP = n; }
}

void set_ancora_txt(TEXTO t, char anc){ as(t)->ancora = anc; }

void set_txto_txt(TEXTO t, const char* cont){
    texto* T = as(t); char* n = dupstr(cont);
    if(!cont || n){ free(T->txto); T->txto = n; }
}

void setFamily(Estilo e, const char* familia){
    estilo* E = asE(e); char* n = dupstr(familia);
    if(!familia || n){ free(E->family); E->family = n; }
}
void setWeight(Estilo e, const char* wgt){
    estilo* E = asE(e); char* n = dupstr(wgt);
    if(!wgt || n){ free(E->weight); E->weight = n; }
}
void setSize(Estilo e, const char* sz){
    estilo* E = asE(e); char* n = dupstr(sz);
    if(!sz || n){ free(E->size); E->size = n; }
}

// ---------------------------------------------------------------------------
// util
// ---------------------------------------------------------------------------

double calcula_area_txt(TEXTO t){
    return 20.0 * (double) get_len_txt(t); // conforme enunciado
}

TEXTO clona_texto(TEXTO t, int novoId){
    texto* o = as(t); if(!o) return NULL;
    texto* n = (texto*) malloc(sizeof(texto)); if(!n) return NULL;
    *n = *o; n->id = novoId; // copia campos escalares e ancora
    n->corB = dupstr(o->corB);
    n->corP = dupstr(o->corP);
    n->txto = dupstr(o->txto);
    n->txtstyle.family = dupstr(o->txtstyle.family);
    n->txtstyle.weight = dupstr(o->txtstyle.weight);
    n->txtstyle.size   = dupstr(o->txtstyle.size);
    if ((o->corB && !n->corB) || (o->corP && !n->corP) || (o->txto && !n->txto) ||
        (o->txtstyle.family && !n->txtstyle.family) || (o->txtstyle.weight && !n->txtstyle.weight) ||
        (o->txtstyle.size && !n->txtstyle.size)){
        free(n->corB); free(n->corP); free(n->txto);
        free(n->txtstyle.family); free(n->txtstyle.weight); free(n->txtstyle.size);
        free(n); return NULL;
    }
    return (TEXTO) n;
}

void inverte_cores_txt(TEXTO t){
    texto* tx = as(t); if(!tx) return;
    char* tmp = tx->corB; tx->corB = tx->corP; tx->corP = tmp; // swap sem alocação
}

// ---------------------------------------------------------------------------
// aliases para geometria.c
// ---------------------------------------------------------------------------

double      get_x_txt(TEXTO t){ return get_codx_txt(t); }
double      get_y_txt(TEXTO t){ return get_cody_txt(t); }
char        get_anc_txt(TEXTO t){ return get_ancora_txt(t); } // alias de char
int         get_len_txt_alias(TEXTO t){ return get_len_txt(t); }
const char* get_corp_txt(TEXTO t){ return get_corP_txt(t); }
void        set_corb_txt(TEXTO t, const char* c){ set_corB_txt(t, c); }
