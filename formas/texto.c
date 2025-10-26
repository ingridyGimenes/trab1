#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texto.h"

typedef struct {
    char* family;
    char* weight;
    char* size;
} Estilo;

typedef struct {
    int id;
    double x, y;
    char* cor_b;
    char* cor_p;
    char ancora;
    char* conteudo;
    Estilo* estilo;
} Texto;

ESTILO criaEstilo(const char* family, const char* weight, const char* size) {
    Estilo* e = malloc(sizeof(Estilo));
    if (!e) {
        fprintf(stderr, "Erro ao alocar estilo\n");
        exit(1);
    }
    e->family = strdup(family);
    e->weight = strdup(weight);
    e->size = strdup(size);
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
    free(est->family);
    est->family = strdup(f);
}
void setWeight(ESTILO e, const char* w) {
    Estilo* est = (Estilo*)e;
    free(est->weight);
    est->weight = strdup(w);
}
void setSize(ESTILO e, const char* s) {
    Estilo* est = (Estilo*)e;
    free(est->size);
    est->size = strdup(s);
}

TEXTO criaTexto(int id, double x, double y, const char* corB, const char* corP, char ancora, const char* conteudo, ESTILO estilo) {
    Texto* t = malloc(sizeof(Texto));
    if (!t) {
        fprintf(stderr, "Erro ao alocar texto\n");
        exit(1);
    }
    t->id = id;
    t->x = x;
    t->y = y;
    t->cor_b = strdup(corB);
    t->cor_p = strdup(corP);
    t->ancora = ancora;
    t->conteudo = strdup(conteudo);
    t->estilo = (Estilo*)estilo;
    return (TEXTO)t;
}

void destruirTexto(TEXTO t) {
    Texto* txt = (Texto*)t;
    if (!txt) return;
    free(txt->cor_b);
    free(txt->cor_p);
    free(txt->conteudo);
    if (txt->estilo) destruirEstilo(txt->estilo);
    free(txt);
}

int getIdTexto(TEXTO t) { return ((Texto*)t)->id; }
double getXTexto(TEXTO t) { return ((Texto*)t)->x; }
double getYTexto(TEXTO t) { return ((Texto*)t)->y; }
const char* getCorBordaTexto(TEXTO t) { return ((Texto*)t)->cor_b; }
const char* getCorPreenchimentoTexto(TEXTO t) { return ((Texto*)t)->cor_p; }
char getAncoraTexto(TEXTO t) { return ((Texto*)t)->ancora; }
const char* getConteudoTexto(TEXTO t) { return ((Texto*)t)->conteudo; }
ESTILO getEstiloTexto(TEXTO t) { return ((Texto*)t)->estilo; }

void setIdTexto(TEXTO t, int id) { ((Texto*)t)->id = id; }
void setXTexto(TEXTO t, double x) { ((Texto*)t)->x = x; }
void setYTexto(TEXTO t, double y) { ((Texto*)t)->y = y; }

void setCorBordaTexto(TEXTO t, const char* corB) {
    Texto* txt = (Texto*)t;
    free(txt->cor_b);
    txt->cor_b = strdup(corB);
}

void setCorPreenchimentoTexto(TEXTO t, const char* corP) {
    Texto* txt = (Texto*)t;
    free(txt->cor_p);
    txt->cor_p = strdup(corP);
}

void setAncoraTexto(TEXTO t, char ancora) {
    ((Texto*)t)->ancora = ancora;
}

void setConteudoTexto(TEXTO t, const char* conteudo) {
    Texto* txt = (Texto*)t;
    free(txt->conteudo);
    txt->conteudo = strdup(conteudo);
}

void setEstiloTexto(TEXTO t, ESTILO estilo) {
    Texto* txt = (Texto*)t;
    if (txt->estilo) destruirEstilo(txt->estilo);
    txt->estilo = (Estilo*)estilo;
}

double calculaAreaTexto(TEXTO t) {
    if (!t) return 0.0;
    Texto* txt = (Texto*)t;
    const char* s = txt->conteudo ? txt->conteudo : "";
    return 20.0 * (double)strlen(s);
}


TEXTO clonaTexto(TEXTO t, int novoId) {
    Texto* orig = (Texto*)t;
    ESTILO novoEstilo = criaEstilo(
        orig->estilo->family,
        orig->estilo->weight,
        orig->estilo->size
    );
    return criaTexto(
        novoId,
        orig->x,
        orig->y,
        orig->cor_b,
        orig->cor_p,
        orig->ancora,
        orig->conteudo,
        novoEstilo
    );
}

void inverterCoresTexto(TEXTO t) {
    Texto* txt = (Texto*)t;
    char* temp = txt->cor_b;
    txt->cor_b = txt->cor_p;
    txt->cor_p = temp;
}
