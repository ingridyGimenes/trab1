#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linha.h"

typedef struct {
    int id;
    double x1, y1, x2, y2;
    char* cor;
} Linha;

LINHA criaLinha(int id, double x1, double y1, double x2, double y2, const char* cor) {
    Linha* l = malloc(sizeof(Linha));
    if (!l) {
        fprintf(stderr, "Erro ao alocar memória para linha.\n");
        exit(1);
    }

    l->id = id;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    l->cor = strdup(cor);

    return (LINHA)l;
}

int getIdLinha(LINHA l) { return ((Linha*)l)->id; }
double getX1Linha(LINHA l) { return ((Linha*)l)->x1; }
double getY1Linha(LINHA l) { return ((Linha*)l)->y1; }
double getX2Linha(LINHA l) { return ((Linha*)l)->x2; }
double getY2Linha(LINHA l) { return ((Linha*)l)->y2; }
const char* getCorLinha(LINHA l) { return ((Linha*)l)->cor; }

void setIdLinha(LINHA l, int id) { ((Linha*)l)->id = id; }
void setX1Linha(LINHA l, double x) { ((Linha*)l)->x1 = x; }
void setY1Linha(LINHA l, double y) { ((Linha*)l)->y1 = y; }
void setX2Linha(LINHA l, double x) { ((Linha*)l)->x2 = x; }
void setY2Linha(LINHA l, double y) { ((Linha*)l)->y2 = y; }

void setCorLinha(LINHA l, const char* cor) {
    Linha* li = (Linha*)l;
    free(li->cor);
    li->cor = strdup(cor);
}

LINHA clonaLinha(LINHA l, int novoId) {
    Linha* orig = (Linha*)l;
    Linha* novo = malloc(sizeof(Linha));
    if (!novo) {
        fprintf(stderr, "Erro ao clonar linha.\n");
        exit(1);
    }

    novo->id = novoId;
    novo->x1 = orig->x1;
    novo->y1 = orig->y1;
    novo->x2 = orig->x2;
    novo->y2 = orig->y2;
    novo->cor = strdup(orig->cor);
    return (LINHA)novo;
}

void destruirLinha(LINHA l) {
    Linha* li = (Linha*)l;
    if (!li) return;
    free(li->cor);
    free(li);
}


double calculaAreaLinha(LINHA l) {
    double x1 = getX1Linha(l);
    double y1 = getY1Linha(l);
    double x2 = getX2Linha(l);
    double y2 = getY2Linha(l);

    double comprimento = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    return 2.0 * comprimento;
}