#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "texto.h"

/* =========================
   Tipos internos
   ========================= */
typedef struct {
    char* family;
    char* weight;
    char* size;
} Estilo;

typedef struct {
    int id;
    double x, y;
    char* cor_b;        /* stroke */
    char* cor_p;        /* fill   */
    char  ancora;       /* 'i' | 'm' | 'f' */
    char* conteudo;
    Estilo* estilo;     /* pode ser NULL */
} Texto;

/* =========================
   ESTILO
   ========================= */
ESTILO criaEstilo(const char* family, const char* weight, const char* size) {
    Estilo* e = (Estilo*)malloc(sizeof(Estilo));
    if (!e) { fprintf(stderr, "Erro ao alocar Estilo\n"); exit(1); }

    /* defaults se vier NULL */
    if (!family) family = "sans-serif";
    if (!weight) weight = "normal";
    if (!size)   size   = "16px";

    e->family = (char*)malloc(strlen(family) + 1);
    e->weight = (char*)malloc(strlen(weight) + 1);
    e->size   = (char*)malloc(strlen(size)   + 1);
    if (!e->family || !e->weight || !e->size) {
        fprintf(stderr, "Erro: malloc campos de Estilo\n");
        exit(1);
    }
    strcpy(e->family, family);
    strcpy(e->weight, weight);
    strcpy(e->size,   size);

    return (ESTILO)e;
}

void destruirEstilo(ESTILO e) {
    Estilo* est = (Estilo*)e;
    if (!est) return;
    free(est->family);
    free(est->weight);
    free(est->size);
    free(est);
}

const char* getFamily(ESTILO e) { return ((Estilo*)e)->family; }
const char* getWeight(ESTILO e) { return ((Estilo*)e)->weight; }
const char* getSize(ESTILO e)   { return ((Estilo*)e)->size; }

void setFamily(ESTILO e, const char* f) {
    Estilo* est = (Estilo*)e;
    if (!f) f = "sans-serif";
    size_t need = strlen(f) + 1;
    if (!est->family || strlen(est->family) + 1 < need) {
        char* tmp = (char*)realloc(est->family, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc family\n"); exit(1); }
        est->family = tmp;
    }
    strcpy(est->family, f);
}
void setWeight(ESTILO e, const char* w) {
    Estilo* est = (Estilo*)e;
    if (!w) w = "normal";
    size_t need = strlen(w) + 1;
    if (!est->weight || strlen(est->weight) + 1 < need) {
        char* tmp = (char*)realloc(est->weight, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc weight\n"); exit(1); }
        est->weight = tmp;
    }
    strcpy(est->weight, w);
}
void setSize(ESTILO e, const char* s) {
    Estilo* est = (Estilo*)e;
    if (!s) s = "16px";
    size_t need = strlen(s) + 1;
    if (!est->size || strlen(est->size) + 1 < need) {
        char* tmp = (char*)realloc(est->size, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc size\n"); exit(1); }
        est->size = tmp;
    }
    strcpy(est->size, s);
}

/* =========================
   TEXTO
   ========================= */
TEXTO criaTexto(int id, double x, double y,
                const char* corB, const char* corP,
                char ancora, const char* conteudo,
                ESTILO estilo)
{
    Texto* t = (Texto*)malloc(sizeof(Texto));
    if (!t) { fprintf(stderr, "Erro ao alocar Texto\n"); exit(1); }

    t->id = id;
    t->x  = x;
    t->y  = y;

    if (ancora != 'i' && ancora != 'm' && ancora != 'f') ancora = 'i';
    t->ancora = ancora;

    if (!corB) corB = "#000000";
    if (!corP) corP = "none";
    if (!conteudo) conteudo = "";

    t->cor_b = (char*)malloc(strlen(corB) + 1);
    t->cor_p = (char*)malloc(strlen(corP) + 1);
    t->conteudo = (char*)malloc(strlen(conteudo) + 1);
    if (!t->cor_b || !t->cor_p || !t->conteudo) {
        fprintf(stderr, "Erro: malloc campos de Texto\n"); exit(1);
    }
    strcpy(t->cor_b, corB);
    strcpy(t->cor_p, corP);
    strcpy(t->conteudo, conteudo);

    /* deep copy do estilo se vier */
    if (estilo) {
        Estilo* es = (Estilo*)estilo;
        t->estilo = (Estilo*)criaEstilo(es->family, es->weight, es->size);
    } else {
        t->estilo = NULL;
    }

    return (TEXTO)t;
}

void destruirTexto(TEXTO t) {
    Texto* txt = (Texto*)t;
    if (!txt) return;
    free(txt->cor_b);
    free(txt->cor_p);
    free(txt->conteudo);
    if (txt->estilo) destruirEstilo((ESTILO)txt->estilo);
    free(txt);
}

/* Getters */
int         getIdTexto(TEXTO t)               { return ((Texto*)t)->id; }
double      getXTexto(TEXTO t)                { return ((Texto*)t)->x; }
double      getYTexto(TEXTO t)                { return ((Texto*)t)->y; }
const char* getCorBordaTexto(TEXTO t)         { return ((Texto*)t)->cor_b; }
const char* getCorPreenchimentoTexto(TEXTO t) { return ((Texto*)t)->cor_p; }
char        getAncoraTexto(TEXTO t)           { return ((Texto*)t)->ancora; }
const char* getConteudoTexto(TEXTO t)         { return ((Texto*)t)->conteudo; }
ESTILO      getEstiloTexto(TEXTO t)           { return (ESTILO)((Texto*)t)->estilo; }

/* Setters simples */
void setIdTexto(TEXTO t, int id)     { ((Texto*)t)->id = id; }
void setXTexto(TEXTO t, double x)    { ((Texto*)t)->x = x; }
void setYTexto(TEXTO t, double y)    { ((Texto*)t)->y = y; }
void setAncoraTexto(TEXTO t, char a) { ((Texto*)t)->ancora = (a=='i'||a=='m'||a=='f')?a:'i'; }

/* Setters com realloc seguro */
void setCorBordaTexto(TEXTO t, const char* corB) {
    Texto* txt = (Texto*)t;
    if (!corB) corB = "#000000";
    size_t need = strlen(corB) + 1;
    if (!txt->cor_b || strlen(txt->cor_b) + 1 < need) {
        char* tmp = (char*)realloc(txt->cor_b, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor_b texto\n"); exit(1); }
        txt->cor_b = tmp;
    }
    strcpy(txt->cor_b, corB);
}

void setCorPreenchimentoTexto(TEXTO t, const char* corP) {
    Texto* txt = (Texto*)t;
    if (!corP) corP = "none";
    size_t need = strlen(corP) + 1;
    if (!txt->cor_p || strlen(txt->cor_p) + 1 < need) {
        char* tmp = (char*)realloc(txt->cor_p, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc cor_p texto\n"); exit(1); }
        txt->cor_p = tmp;
    }
    strcpy(txt->cor_p, corP);
}

void setConteudoTexto(TEXTO t, const char* conteudo) {
    Texto* txt = (Texto*)t;
    if (!conteudo) conteudo = "";
    size_t need = strlen(conteudo) + 1;
    if (!txt->conteudo || strlen(txt->conteudo) + 1 < need) {
        char* tmp = (char*)realloc(txt->conteudo, need);
        if (!tmp) { fprintf(stderr, "Erro: realloc conteudo texto\n"); exit(1); }
        txt->conteudo = tmp;
    }
    strcpy(txt->conteudo, conteudo);
}

void setEstiloTexto(TEXTO t, ESTILO estilo) {
    Texto* txt = (Texto*)t;
    if (txt->estilo) destruirEstilo((ESTILO)txt->estilo);
    txt->estilo = NULL;

    if (estilo) {
        Estilo* es = (Estilo*)estilo;
        txt->estilo = (Estilo*)criaEstilo(es->family, es->weight, es->size);
    }
}

/* Métrica simples de “área” proporcional ao comprimento do texto */
double calculaAreaTexto(TEXTO t) {
    if (!t) return 0.0;
    Texto* txt = (Texto*)t;
    const char* s = txt->conteudo ? txt->conteudo : "";
    return 20.0 * (double)strlen(s);
}

/* Clone profundo */
TEXTO clonaTexto(TEXTO t, int novoId) {
    Texto* orig = (Texto*)t;
    ESTILO novoEstilo = NULL;
    if (orig->estilo) {
        novoEstilo = criaEstilo(orig->estilo->family,
                                orig->estilo->weight,
                                orig->estilo->size);
    }
    return criaTexto(novoId,
                     orig->x, orig->y,
                     orig->cor_b, orig->cor_p,
                     orig->ancora,
                     orig->conteudo,
                     novoEstilo);
}

/* Inverte cores trocando os ponteiros */
void inverterCoresTexto(TEXTO t) {
    Texto* txt = (Texto*)t;
    char* tmp = txt->cor_b;
    txt->cor_b = txt->cor_p;
    txt->cor_p = tmp;
}
